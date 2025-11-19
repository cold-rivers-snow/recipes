#include <brpc/server.h>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "registry_service.pb.h"

using namespace std;
using namespace brpc;

// 日志输出宏（带时间戳）
#define LOG_INFO(msg) do { \
    auto now = chrono::system_clock::now(); \
    time_t now_time = chrono::system_clock::to_time_t(now); \
    cout << "[" << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S") << "] [INFO] " << msg << endl; \
} while(0)

#define LOG_WARN(msg) do { \
    auto now = chrono::system_clock::now(); \
    time_t now_time = chrono::system_clock::to_time_t(now); \
    cerr << "[" << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S") << "] [WARN] " << msg << endl; \
} while(0)

// 服务注册中心核心类，管理服务列表和健康检查
class RegistryServiceImpl : public registry::RegistryService {
public:
    RegistryServiceImpl() {
        // 启动健康检查线程（定期清理超时服务）
        _health_check_thread = thread(&RegistryServiceImpl::health_check, this);
        LOG_INFO("Registry service initialized, health check thread started");
    }

    ~RegistryServiceImpl() {
        _stop_health_check = true;
        if (_health_check_thread.joinable()) {
            _health_check_thread.join();
        }
        LOG_INFO("Registry service stopped");
    }

    // 服务注册
    void Register(google::protobuf::RpcController* cntl,
                  const registry::RegisterRequest* request,
                  registry::RegisterResponse* response,
                  google::protobuf::Closure* done) override {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller* c = static_cast<brpc::Controller*>(cntl);

        lock_guard<mutex> lock(_mutex);
        const string& service_name = request->service_name();
        const auto& addr = request->address();
        string addr_str = addr.ip() + ":" + to_string(addr.port());

        // 验证参数
        if (service_name.empty() || addr.ip().empty() || addr.port() <= 0) {
            response->set_status(registry::RegisterResponse::INVALID_PARAM);
            response->set_message("Invalid service name or address");
            LOG_WARN("Register failed (invalid param): service=" << service_name << ", address=" << addr_str);
            return;
        }

        // 检查是否已注册
        auto& servers = _service_map[service_name];
        for (const auto& s : servers) {
            if (s.ip() == addr.ip() && s.port() == addr.port()) {
                response->set_status(registry::RegisterResponse::ALREADY_EXISTS);
                response->set_message("Service already registered");
                LOG_WARN("Register failed (already exists): service=" << service_name << ", address=" << addr_str);
                return;
            }
        }

        // 添加服务并更新心跳时间
        servers.push_back(addr);
        _heartbeat_map[get_key(service_name, addr)] = time(nullptr);

        response->set_status(registry::RegisterResponse::SUCCESS);
        response->set_message("Register success");
        LOG_INFO("Service registered: service=" << service_name << ", address=" << addr_str);
        
        // 打印当前service_map状态
        print_service_map();
    }

    // 服务注销
    void Unregister(google::protobuf::RpcController* cntl,
                    const registry::UnregisterRequest* request,
                    registry::UnregisterResponse* response,
                    google::protobuf::Closure* done) override {
        brpc::ClosureGuard done_guard(done);

        lock_guard<mutex> lock(_mutex);
        const string& service_name = request->service_name();
        const auto& addr = request->address();
        string addr_str = addr.ip() + ":" + to_string(addr.port());

        auto it = _service_map.find(service_name);
        if (it == _service_map.end()) {
            response->set_status(registry::UnregisterResponse::NOT_FOUND);
            LOG_WARN("Unregister failed (not found): service=" << service_name << ", address=" << addr_str);
            return;
        }

        // 从服务列表中移除
        auto& servers = it->second;
        bool removed = false;
        for (auto iter = servers.begin(); iter != servers.end(); ++iter) {
            if (iter->ip() == addr.ip() && iter->port() == addr.port()) {
                servers.erase(iter);
                removed = true;
                break;
            }
        }

        if (removed) {
            _heartbeat_map.erase(get_key(service_name, addr));
            response->set_status(registry::UnregisterResponse::SUCCESS);
            LOG_INFO("Service unregistered: service=" << service_name << ", address=" << addr_str);
            
            // 如果服务列表为空，从map中移除该服务名
            if (servers.empty()) {
                _service_map.erase(it);
                LOG_INFO("Service entry removed (no addresses left): service=" << service_name);
            }
            
            // 打印当前service_map状态
            print_service_map();
        } else {
            response->set_status(registry::UnregisterResponse::NOT_FOUND);
            LOG_WARN("Unregister failed (not found): service=" << service_name << ", address=" << addr_str);
        }
    }

    // 服务发现
    void Discover(google::protobuf::RpcController* cntl,
                  const registry::DiscoverRequest* request,
                  registry::DiscoverResponse* response,
                  google::protobuf::Closure* done) override {
        brpc::ClosureGuard done_guard(done);

        lock_guard<mutex> lock(_mutex);
        const string& service_name = request->service_name();

        auto it = _service_map.find(service_name);
        if (it != _service_map.end()) {
            // 返回所有可用服务地址
            for (const auto& addr : it->second) {
                *response->add_addresses() = addr;
            }
            LOG_INFO("Service discovered: service=" << service_name << ", count=" << it->second.size());
        } else {
            LOG_WARN("Service not found during discovery: service=" << service_name);
        }
    }

    // 心跳检测（更新服务存活状态）
    void Heartbeat(google::protobuf::RpcController* cntl,
                   const registry::HeartbeatRequest* request,
                   registry::HeartbeatResponse* response,
                   google::protobuf::Closure* done) override {
        brpc::ClosureGuard done_guard(done);

        lock_guard<mutex> lock(_mutex);
        const string service_name = request->service_name();
        const auto& addr = request->address();
        const string key = get_key(service_name, addr);
        string addr_str = addr.ip() + ":" + to_string(addr.port());

        if (_heartbeat_map.count(key)) {
            _heartbeat_map[key] = time(nullptr);  // 更新心跳时间
            response->set_status(registry::HeartbeatResponse::SUCCESS);
            LOG_INFO("Heartbeat received: service=" << service_name << ", address=" << addr_str);
        } else {
            response->set_status(registry::HeartbeatResponse::NOT_REGISTERED);
            LOG_WARN("Heartbeat failed (not registered): service=" << service_name << ", address=" << addr_str);
        }
    }

private:
    // 生成服务唯一标识（服务名+IP+端口）
    string get_key(const string& service_name, const registry::ServerAddress& addr) {
        return service_name + "|" + addr.ip() + ":" + to_string(addr.port());
    }

    // 打印当前service_map中的所有服务信息
    void print_service_map() {
        LOG_INFO("Current service map status:");
        if (_service_map.empty()) {
            LOG_INFO("  No services registered");
            return;
        }
        for (const auto& entry : _service_map) {
            LOG_INFO("  Service: " << entry.first << " (count=" << entry.second.size() << ")");
            for (const auto& addr : entry.second) {
                LOG_INFO("    Address: " << addr.ip() << ":" << addr.port() << ", weight=" << addr.weight());
            }
        }
    }

    // 健康检查线程：清理超过30秒未心跳的服务
    void health_check() {
        while (!_stop_health_check) {
            this_thread::sleep_for(chrono::seconds(10));  // 每10秒检查一次
            lock_guard<mutex> lock(_mutex);

            const time_t now = time(nullptr);
            vector<string> expired_keys;

            // 找出超时服务
            for (const auto& p : _heartbeat_map) {
                if (now - p.second > 30) {  // 30秒超时
                    expired_keys.push_back(p.first);
                }
            }

            // 移除超时服务
            if (!expired_keys.empty()) {
                LOG_INFO("Health check: found " << expired_keys.size() << " expired service(s)");
                for (const string& key : expired_keys) {
                    _heartbeat_map.erase(key);
                    // 解析key获取服务名和地址
                    size_t pos1 = key.find('|');
                    size_t pos2 = key.find(':', pos1 + 1);
                    if (pos1 == string::npos || pos2 == string::npos) {
                        LOG_WARN("Invalid key format during cleanup: " << key);
                        continue;
                    }

                    string service_name = key.substr(0, pos1);
                    string ip = key.substr(pos1 + 1, pos2 - pos1 - 1);
                    int port = stoi(key.substr(pos2 + 1));
                    string addr_str = ip + ":" + to_string(port);

                    // 从服务列表中删除
                    auto it = _service_map.find(service_name);
                    if (it != _service_map.end()) {
                        auto& servers = it->second;
                        for (auto iter = servers.begin(); iter != servers.end(); ++iter) {
                            if (iter->ip() == ip && iter->port() == port) {
                                servers.erase(iter);
                                LOG_INFO("Expired service removed: service=" << service_name << ", address=" << addr_str);
                                break;
                            }
                        }
                        // 如果服务列表为空，从map中移除该服务名
                        if (servers.empty()) {
                            _service_map.erase(it);
                            LOG_INFO("Service entry removed (no addresses left): service=" << service_name);
                        }
                    }
                }
                // 打印清理后的service_map状态
                print_service_map();
            } else {
                LOG_INFO("Health check: no expired services");
            }
        }
    }

    mutex _mutex;
    // 服务名 -> 服务地址列表映射
    unordered_map<string, vector<registry::ServerAddress>> _service_map;
    // 服务唯一标识 -> 最后心跳时间映射（用于健康检查）
    unordered_map<string, time_t> _heartbeat_map;
    thread _health_check_thread;
    atomic<bool> _stop_health_check{false};
};

int main(int argc, char* argv[]) {
    // 初始化服务器
    brpc::Server server;
    RegistryServiceImpl registry_service;

    // 注册服务
    if (server.AddService(&registry_service, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        cerr << "Failed to add registry service" << endl;
        return -1;
    }

    // 设置服务器地址
    brpc::ServerOptions options;
    options.idle_timeout_sec = 300;
    if (server.Start(18888, &options) != 0) {  // 注册中心监听8888端口
        cerr << "Failed to start registry server" << endl;
        return -1;
    }

    LOG_INFO("Registry server started on port 18888");
    server.RunUntilAskedToQuit();
    return 0;
}