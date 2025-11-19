#include <brpc/channel.h>
#include <brpc/controller.h>
#include <butil/endpoint.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "registry_service.pb.h"
#include "echo_service.pb.h"

// 保存初始 endpoints
static std::vector<std::string> g_providers;

static void PrintEndpoints() {
    std::cout << "    [Current Service List] ";
    for (auto& ep : g_providers) {
        std::cout << ep << " ";
    }
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <registry_ip> [registry_port=18888] [service_name=example.EchoService] [qps=1]\n";
        return -1;
    }

    std::string registry_ip = argv[1];
    int registry_port = (argc >= 3) ? atoi(argv[2]) : 18888;
    std::string service_full_name = (argc >= 4) ? argv[3] : "example.EchoService";
    int qps = (argc >= 5) ? atoi(argv[4]) : 1;
    if (qps <= 0) qps = 1;

    // === 1. 请求注册中心，拿到 provider 列表 ===
    brpc::ChannelOptions reg_opt;
    reg_opt.protocol = "baidu_std";
    brpc::Channel registry_ch;
    if (registry_ch.Init((registry_ip + ":" + std::to_string(registry_port)).c_str(), &reg_opt) != 0) {
        std::cerr << "Fail to init registry channel\n";
        return -1;
    }

    registry::DiscoverRequest req;
    registry::DiscoverResponse res;
    req.set_service_name(service_full_name);

    brpc::Controller cntl;
    registry::RegistryService_Stub reg_stub(&registry_ch);
    reg_stub.Discover(&cntl, &req, &res, nullptr);
    if (cntl.Failed()) {
        std::cerr << "Discover RPC failed: " << cntl.ErrorText() << std::endl;
        return -1;
    }
    if (res.addresses_size() == 0) {
        std::cerr << "No providers found for " << service_full_name << std::endl;
        return -1;
    }

    // === 2. 拼接成 list:// URL ===
    std::string list_url = "list://";
    for (int i = 0; i < res.addresses_size(); ++i) {
        if (i > 0) list_url += ",";
        std::string ep = res.addresses(i).ip() + ":" + std::to_string(res.addresses(i).port());
        list_url += ep;
        g_providers.push_back(ep);   // 保存
    }
    std::cout << "Discovered providers: " << list_url << std::endl;

    // === 3. 用 list:// 初始化 Channel ===
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    options.timeout_ms = 2000;
    options.connect_timeout_ms = 1000;
    options.max_retry = 1;

    brpc::Channel channel;
    if (channel.Init(list_url.c_str(), "rr", &options) != 0) {
        std::cerr << "Fail to init channel with " << list_url << std::endl;
        return -1;
    }

    example::EchoService_Stub stub(&channel);

    std::cout << "Consumer started. Service=" << service_full_name
              << " providers=" << res.addresses_size()
              << " QPS=" << qps << std::endl;

    // === 4. 轮询发请求 ===
    int seq = 0;
    while (true) {
        example::EchoRequest ereq;
        example::EchoResponse eres;
        ereq.set_message("hello #" + std::to_string(++seq));

        brpc::Controller ecntl;

        std::cout << "\n[Send #" << seq << "]" << std::endl;
        PrintEndpoints();  // 打印初始化时的服务列表

        stub.Echo(&ecntl, &ereq, &eres, nullptr);

        if (ecntl.Failed()) {
            std::cerr << "    [Echo] failed: " << ecntl.ErrorText() << std::endl;
        } else {
            std::cout << "    [Echo@" << ecntl.remote_side()
                      << "] resp=" << eres.message()
                      << " latency=" << ecntl.latency_us() << "us" << std::endl;
        }

        usleep(1000000 / qps);
    }

    return 0;
}
