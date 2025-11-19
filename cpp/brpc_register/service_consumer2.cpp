#include <brpc/channel.h>
#include <brpc/controller.h>
#include <brpc/naming_service.h>                  // NamingServiceExtension
#include <brpc/periodic_naming_service.h>         // 继承用
#include <butil/endpoint.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unistd.h>

#include "registry_service.pb.h"
#include "echo_service.pb.h"

// ===================== 自定义 NamingService =====================
class RegistryNamingService : public brpc::PeriodicNamingService {
public:
    RegistryNamingService(const std::string& registry_ip, int registry_port)
        : _registry_ip(registry_ip), _registry_port(registry_port) {}

    int GetServers(const char* service_name,
                   std::vector<brpc::ServerNode>* servers) override {
        brpc::ChannelOptions opt;
        opt.protocol = "baidu_std";
        brpc::Channel registry_ch;
        std::string reg_addr = _registry_ip + ":" + std::to_string(_registry_port);
        if (registry_ch.Init(reg_addr.c_str(), &opt) != 0) {
            std::cerr << "Fail to init registry channel" << std::endl;
            return -1;
        }

        registry::RegistryService_Stub stub(&registry_ch);
        registry::DiscoverRequest req;
        registry::DiscoverResponse res;
        req.set_service_name(service_name);

        brpc::Controller cntl;
        stub.Discover(&cntl, &req, &res, nullptr);
        if (cntl.Failed()) {
            std::cerr << "Discover RPC failed: " << cntl.ErrorText() << std::endl;
            return -1;
        }

        servers->clear();
        std::set<std::string> new_set;
        for (int i = 0; i < res.addresses_size(); ++i) {
            brpc::ServerNode node;
            if (butil::str2ip(res.addresses(i).ip().c_str(), &node.addr.ip) != 0) {
                continue;
            }
            node.addr.port = res.addresses(i).port();
            servers->push_back(node);

            std::string ip_str = butil::ip2str(node.addr.ip).c_str();
            std::string addr_str = ip_str + ":" + std::to_string(node.addr.port);
            new_set.insert(addr_str);
        }

        // === 比较差异，打印新增/移除节点 ===
        for (auto& addr : new_set) {
            if (_last_set.find(addr) == _last_set.end()) {
                std::cout << "[RegistryNamingService] 新增节点: " << addr << std::endl;
            }
        }
        for (auto& addr : _last_set) {
            if (new_set.find(addr) == new_set.end()) {
                std::cout << "[RegistryNamingService] 移除节点: " << addr << std::endl;
            }
        }

        // === 打印完整服务列表 ===
        std::cout << "[RegistryNamingService] 当前服务列表: ";
        for (auto& addr : new_set) {
            std::cout << addr << " ";
        }
        std::cout << std::endl;

        _last_set.swap(new_set);
        return 0;
    }

    // 必须实现的纯虚函数
    brpc::NamingService* New() const override {
        return new RegistryNamingService(_registry_ip, _registry_port);
    }
    void Destroy() override {
        delete this;
    }

private:
    std::string _registry_ip;
    int _registry_port;
    std::set<std::string> _last_set;  // 记录上一次的节点列表
};

// ===================== Consumer main =====================
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <registry_ip> [registry_port=8888] [service_name=example.EchoService] [qps=1]\n";
        return -1;
    }

    std::string registry_ip = argv[1];
    int registry_port = (argc >= 3) ? atoi(argv[2]) : 8888;
    std::string service_full_name = (argc >= 4) ? argv[3] : "example.EchoService";
    int qps = (argc >= 5) ? atoi(argv[4]) : 1;
    if (qps <= 0) qps = 1;

    // 注册自定义 NamingService
    brpc::NamingServiceExtension()->RegisterOrDie(
        "registry", new RegistryNamingService(registry_ip, registry_port));

    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    options.timeout_ms = 2000;
    options.connect_timeout_ms = 1000;
    options.max_retry = 1;

    brpc::Channel channel;
    std::string ns_url = "registry://" + service_full_name;
    if (channel.Init(ns_url.c_str(), "rr", &options) != 0) {
        std::cerr << "Fail to init channel with " << ns_url << std::endl;
        return -1;
    }

    example::EchoService_Stub stub(&channel);

    std::cout << "Consumer started. Service=" << service_full_name
              << " registry=" << registry_ip << ":" << registry_port
              << " QPS=" << qps << std::endl;

    int seq = 0;
    while (true) {
        example::EchoRequest ereq;
        example::EchoResponse eres;
        ereq.set_message("hello #" + std::to_string(++seq));

        brpc::Controller ecntl;
        stub.Echo(&ecntl, &ereq, &eres, nullptr);

        if (ecntl.Failed()) {
            std::cerr << "RPC failed: " << ecntl.ErrorText() << std::endl;
        } else {
            std::cout << "[Echo] sent to " << butil::endpoint2str(ecntl.remote_side())
                      << " resp=" << eres.message()
                      << " latency=" << ecntl.latency_us() << "us" << std::endl;
        }

        usleep(1000000 / qps);
    }

    return 0;
}
