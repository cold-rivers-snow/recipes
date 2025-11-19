#include <brpc/server.h>
#include <brpc/channel.h>
#include <thread>
#include <atomic>
#include "registry_service.pb.h"
#include "echo_service.pb.h"  // 假设已定义Echo服务

using namespace std;
using namespace brpc;

// 示例Echo服务实现（增加地址信息打印）
class EchoServiceImpl : public example::EchoService {
public:
    // 构造函数：保存服务自身地址信息
    EchoServiceImpl(const string& ip, int port) : _self_ip(ip), _self_port(port) {}

    void Echo(google::protobuf::RpcController* cntl,
              const example::EchoRequest* request,
              example::EchoResponse* response,
              google::protobuf::Closure* done) override {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller* brpc_cntl = static_cast<brpc::Controller*>(cntl);

        // 1. 获取服务自身地址端口（初始化时传入）
        string self_addr = _self_ip + ":" + to_string(_self_port);

        // 2. 获取客户端地址端口（从Controller中提取）
        const butil::EndPoint& client_endpoint = brpc_cntl->remote_side();
        string client_ip = butil::ip2str(client_endpoint.ip).c_str();
        int client_port = client_endpoint.port;
        string client_addr = client_ip + ":" + to_string(client_port);

        // 3. 打印地址信息
        cout << "[Request Received]" << endl;
        cout << "  Service Address: " << self_addr << endl;
        cout << "  Client Address:  " << client_addr << endl;
        cout << "  Request Message: " << request->message() << endl << endl;

        // 4. 构造响应
        response->set_message("Received: " + request->message());
    }

private:
    string _self_ip;  // 服务自身IP
    int _self_port;   // 服务自身端口
};

// 向注册中心注册服务并发送心跳
class ServiceRegistrar {
public:
    ServiceRegistrar(const string& registry_ip, int registry_port,
                     const string& service_name, const string& self_ip, int self_port)
        : _service_name(service_name), _self_ip(self_ip), _self_port(self_port) {
        // 初始化注册中心channel
        ChannelOptions options;
        options.protocol = "baidu_std";
        std::string registry_addr = registry_ip + ":" + std::to_string(registry_port);
        if (_channel.Init(registry_addr.c_str(), &options) != 0) {
            cerr << "Failed to initialize registry channel" << endl;
            exit(-1);
        }

        // 注册服务
        registry::RegisterRequest req;
        registry::RegisterResponse res;
        req.set_service_name(service_name);
        auto* addr = req.mutable_address();
        addr->set_ip(self_ip);
        addr->set_port(self_port);
        addr->set_weight(1);

        Controller cntl;
        registry::RegistryService_Stub stub(&_channel);
        stub.Register(&cntl, &req, &res, nullptr);

        if (cntl.Failed()) {
            cerr << "Register failed: " << cntl.ErrorText() << endl;
            exit(-1);
        }
        if (res.status() != registry::RegisterResponse::SUCCESS) {
            cerr << "Register failed: " << res.message() << endl;
            exit(-1);
        }
        cout << "Service registered successfully" << endl;

        // 启动心跳线程
        _heartbeat_thread = thread(&ServiceRegistrar::send_heartbeat, this);
    }

    ~ServiceRegistrar() {
        // 注销服务
        registry::UnregisterRequest req;
        registry::UnregisterResponse res;
        req.set_service_name(_service_name);
        auto* addr = req.mutable_address();
        addr->set_ip(_self_ip);
        addr->set_port(_self_port);

        Controller cntl;
        registry::RegistryService_Stub stub(&_channel);
        stub.Unregister(&cntl, &req, &res, nullptr);

        _stop_heartbeat = true;
        if (_heartbeat_thread.joinable()) {
            _heartbeat_thread.join();
        }
    }

private:
    // 定期发送心跳（每5秒一次）
    void send_heartbeat() {
        while (!_stop_heartbeat) {
            this_thread::sleep_for(chrono::seconds(5));

            registry::HeartbeatRequest req;
            registry::HeartbeatResponse res;
            req.set_service_name(_service_name);
            auto* addr = req.mutable_address();
            addr->set_ip(_self_ip);
            addr->set_port(_self_port);

            Controller cntl;
            registry::RegistryService_Stub stub(&_channel);
            stub.Heartbeat(&cntl, &req, &res, nullptr);

            if (cntl.Failed() || res.status() != registry::HeartbeatResponse::SUCCESS) {
                cerr << "Heartbeat failed: " << cntl.ErrorText() << endl;
            }
        }
    }

    string _service_name;
    string _self_ip;
    int _self_port;
    Channel _channel;
    thread _heartbeat_thread;
    atomic<bool> _stop_heartbeat{false};
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <self_ip> <self_port> <registry_ip>" << endl;
        return -1;
    }

    const string self_ip = argv[1];
    const int self_port = stoi(argv[2]);
    const string registry_ip = argv[3];
    const int registry_port = 18888;  // 注册中心端口
    const string service_name = "example.EchoService";

    // 启动Echo服务（传入自身地址信息供打印）
    Server server;
    EchoServiceImpl echo_service(self_ip, self_port);  // 关键：传入服务IP和端口
    if (server.AddService(&echo_service, SERVER_DOESNT_OWN_SERVICE) != 0) {
        cerr << "Failed to add echo service" << endl;
        return -1;
    }

    ServerOptions options;
    if (server.Start(self_port, &options) != 0) {
        cerr << "Failed to start echo server" << endl;
        return -1;
    }

    // 向注册中心注册服务
    ServiceRegistrar registrar(registry_ip, registry_port, service_name, self_ip, self_port);

    cout << "Echo service started on " << self_ip << ":" << self_port << endl;
    server.RunUntilAskedToQuit();
    return 0;
}
