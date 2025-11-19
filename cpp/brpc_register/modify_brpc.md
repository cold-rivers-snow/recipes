可以。你要的目标是：

* 自己实现一个轻量“注册中心”（不依赖 etcd/consul 等）。
* 客户端通过 **NamingService**（由 brpc 后台线程周期刷新）去拉取最新 provider 列表，自动感知上下线，并且能打印当前内存中的服务列表。

考虑到 **Apache brpc 开源仓库目前没有对外暴露“在应用侧注册自定义 scheme 的 API/宏”**，要做“`registry://` 自定义命名服务”，通常有两条路：

---

# 方案 A（不改 brpc 源码，最快落地）

让你的注册中心把当前 provider 列表写到一个文件（如 `/tmp/echo_service.bns`）。
客户端使用 **内置 `file://` NamingService** 动态刷新（brpc 会监控文件变化），实现“通过 NamingService 拉最新列表”。

优点：无需改动 brpc，纯应用层就能完成；后台线程会自动刷新；完整满足“通过 NamingService 拉最新列表”的需求。
缺点：服务发现是“文件驱动”的；但你可以让注册中心维护这个文件，**并不会手工编辑**。

### A1. 注册中心（示意逻辑）

* provider 上报（Register）/下线（Unregister），注册中心更新内存集合。
* 每次集合变化就 **重写** 目标文件（原子方式：写临时文件再 `rename` 覆盖），内容一行一个 `ip:port`，可选 `#` 注释权重。

```cpp
// registry_server.cpp 片段（变化时落盘）
static void DumpProvidersToFile(const std::string& path,
                                const std::vector<std::pair<std::string,int>>& ips) {
    std::string tmp = path + ".tmp";
    std::ofstream ofs(tmp, std::ios::trunc);
    for (auto& e : ips) {
        // e = { "127.0.0.1:9001", weight }
        ofs << e.first;
        if (e.second > 0) ofs << " #" << e.second;   // 可选：把权重写在注释里
        ofs << "\n";
    }
    ofs.close();
    ::rename(tmp.c_str(), path.c_str()); // 原子替换
}
```

> 你现有的 `registry_service.proto / registry_server.cpp / service_provider.cpp`
> 基本不用大改，只要在每次 Register/Unregister 后调用 `DumpProvidersToFile(...)`。

### A2. 客户端（NamingService：file://）

```cpp
// service_consumer.cpp（file://版本）
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <butil/endpoint.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "echo_service.pb.h"

static void PrintEndpoints(brpc::Channel& ch) {
    std::vector<butil::EndPoint> eps;
    ch.ListEndpoints(&eps);
    std::cout << "[Client View] ";
    for (auto& ep : eps) {
        std::cout << butil::ip2str(ep.ip) << ":" << ep.port << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " </path/to/echo_service.bns> [qps=1] [lb=rr|wrr]\n";
        return -1;
    }
    std::string bns_file = argv[1];
    int qps = (argc >= 3) ? atoi(argv[2]) : 1;
    std::string lb = (argc >= 4) ? argv[3] : "rr";
    if (qps <= 0) qps = 1;

    // file:// NamingService，brpc 会后台监控文件变化，动态刷新
    std::string url = "file://" + bns_file;

    brpc::ChannelOptions opt;
    opt.protocol = "baidu_std";
    opt.timeout_ms = 2000;
    opt.connect_timeout_ms = 1000;
    opt.max_retry = 1;

    brpc::Channel ch;
    if (ch.Init(url.c_str(), lb.c_str(), &opt) != 0) {
        std::cerr << "Fail to init channel with " << url << std::endl;
        return -1;
    }

    example::EchoService_Stub stub(&ch);
    std::cout << "Consumer started. url=" << url << " lb=" << lb << " qps=" << qps << std::endl;

    int seq = 0;
    while (true) {
        example::EchoRequest req;
        example::EchoResponse res;
        req.set_message("hello #" + std::to_string(++seq));

        brpc::Controller cntl;
        stub.Echo(&cntl, &req, &res, nullptr);
        if (cntl.Failed()) {
            std::cerr << "[Echo] failed: " << cntl.ErrorText() << std::endl;
        } else {
            std::cout << "[Echo@" << cntl.remote_side() << "] "
                      << res.message() << " latency=" << cntl.latency_us() << "us\n";
        }
        PrintEndpoints(ch);  // 打印当前客户端内存中的服务列表
        usleep(1000000 / qps);
    }
    return 0;
}
```

> 如需**权重**：把文件里行尾的 `#数字` 当作权重，brpc 的 `file://` + `"wrr"` 能识别（不同版本识别方式略有差异；若未生效，可切回方案 B 自定义 NS，明确设置 `ServerNode.tag`）。

---

# 方案 B（自定义 `registry://` NamingService，真正“拉注册中心”）

如果你**必须**是“客户端通过自定义 NamingService 去调用你自己的注册中心服务（Discover RPC）拉列表”，那就要在 brpc 里**注册一个新的 scheme**。在当前开源 brpc 版本，这通常意味着**微调 brpc 源码**（把你的 NS 放进 brpc 的命名服务表），然后重编 brpc 一次。

> 下述做法很简单：只改 2 处文件，新增 2 个文件，代价极低；之后你的业务无需任何 hack。

### B1. 新增类：`RegistryNamingService`

把下面两个文件加到 brpc 源码树（建议放 `src/brpc/policy/`），或你的业务侧但参与 brpc 的编译：

**`src/brpc/policy/registry_naming_service.h`**

```cpp
#pragma once
#include <brpc/naming_service.h>
#include <brpc/channel.h>
#include <butil/endpoint.h>
#include <vector>
#include <string>
#include "registry_service.pb.h"

namespace brpc {
namespace policy {

class RegistryNamingService : public brpc::NamingService {
public:
    // url: registry://<host>:<port>/<full_service_name>
    int RunNamingService(const char* url,
                         brpc::NamingServiceActions* actions) override;

    void Destroy() override { delete this; }
    brpc::NamingService* New() const override { return new RegistryNamingService(); }
};

} // namespace policy
} // namespace brpc
```

**`src/brpc/policy/registry_naming_service.cpp`**

```cpp
#include "brpc/policy/registry_naming_service.h"
#include <brpc/controller.h>
#include <butil/logging.h>

namespace brpc {
namespace policy {

static bool ParseUrl(const std::string& url, std::string* host, int* port, std::string* svc) {
    const std::string k = "registry://";
    if (url.compare(0, k.size(), k) != 0) return false;
    std::string rest = url.substr(k.size());
    auto pos = rest.find('/');
    if (pos == std::string::npos) return false;
    std::string hostport = rest.substr(0, pos);
    *svc = rest.substr(pos + 1);
    auto c = hostport.rfind(':');
    if (c == std::string::npos) return false;
    *host = hostport.substr(0, c);
    *port = atoi(hostport.substr(c + 1).c_str());
    return *port > 0;
}

int RegistryNamingService::RunNamingService(const char* url,
                                            brpc::NamingServiceActions* actions) {
    if (!url) return -1;

    std::string host, svc;
    int port = 0;
    if (!ParseUrl(url, &host, &port, &svc)) {
        LOG(ERROR) << "bad url: " << url
                   << " expect registry://<ip>:<port>/<full_service_name>";
        return -1;
    }

    // 建一个到注册中心的 channel
    brpc::ChannelOptions opt;
    opt.protocol = "baidu_std";
    brpc::Channel reg;
    if (reg.Init((host + ":" + std::to_string(port)).c_str(), &opt) != 0) {
        LOG(ERROR) << "fail to init registry channel";
        return -1;
    }

    // 调用 Discover RPC
    registry::DiscoverRequest req;
    registry::DiscoverResponse rsp;
    req.set_service_name(svc);

    brpc::Controller cntl;
    registry::RegistryService_Stub stub(&reg);
    stub.Discover(&cntl, &req, &rsp, nullptr);
    if (cntl.Failed()) {
        LOG(ERROR) << "Discover failed: " << cntl.ErrorText();
        return -1;
    }

    // 组装 ServerNode 列表（支持权重：tag）
    std::vector<brpc::ServerNode> nodes;
    nodes.reserve(rsp.addresses_size());

    for (const auto& a : rsp.addresses()) {
        brpc::ServerNode n;
        if (butil::str2ip(a.ip().c_str(), &n.addr.ip) != 0) {
            LOG(WARNING) << "invalid ip: " << a.ip();
            continue;
        }
        n.addr.port = a.port();
        n.tag = a.has_weight() ? a.weight() : 1; // 权重
        nodes.push_back(n);
    }

    // 打印调试
    std::string s;
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i) s += ",";
        s += butil::ip2str(nodes[i].addr.ip).c_str();
        s += ":" + std::to_string(nodes[i].addr.port);
        s += "(w=" + std::to_string(nodes[i].tag) + ")";
    }
    LOG(INFO) << "[Discover] " << svc << " -> " << s;

    // 通知 brpc：重置服务列表（brpc 内部比较差异并刷新负载均衡）
    actions->ResetServers(nodes);
    return 0;
}

} // namespace policy
} // namespace brpc
```

### B2. 在 brpc 的命名服务注册表里挂上新 scheme（改 1 行）

找到 brpc 源码里内置命名服务注册的地方（通常在 `src/brpc/policy/naming_service.cpp` 或类似初始化文件，因版本而异），添加一行把 `registry` 注册进去，例如：

```cpp
// 伪代码：与 list/file/bns 的注册方式保持一致
RegisterBuiltinNamingService("registry", new brpc::policy::RegistryNamingService());
```

> 这是唯一需要“动 brpc 源码”的地方（加一行/少量行）。重新编译 brpc 后，整个工程就能识别 `registry://`。

### B3. 客户端用法

```cpp
// service_consumer.cpp（registry://版本）
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <butil/endpoint.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "echo_service.pb.h"

static void PrintEndpoints(brpc::Channel& ch) {
    std::vector<butil::EndPoint> eps;
    ch.ListEndpoints(&eps);
    std::cout << "[Client View] ";
    for (auto& ep : eps) {
        std::cout << butil::ip2str(ep.ip) << ":" << ep.port << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <registry_ip> <registry_port> <full_service_name> [qps=1] [lb=rr|wrr]\n";
        return -1;
    }
    std::string ip = argv[1];
    int port = atoi(argv[2]);
    std::string full_svc = argv[3];
    int qps = (argc >= 5) ? atoi(argv[4]) : 1;
    std::string lb = (argc >= 6) ? argv[5] : "rr";
    if (qps <= 0) qps = 1;

    // brpc 后台线程会周期性调用 RunNamingService()，自动刷新
    std::string url = "registry://" + ip + ":" + std::to_string(port) + "/" + full_svc;

    brpc::ChannelOptions opt;
    opt.protocol = "baidu_std";
    opt.timeout_ms = 2000;
    opt.connect_timeout_ms = 1000;
    opt.max_retry = 1;

    brpc::Channel ch;
    if (ch.Init(url.c_str(), lb.c_str(), &opt) != 0) {
        std::cerr << "Fail to init channel with " << url << std::endl;
        return -1;
    }

    example::EchoService_Stub stub(&ch);
    std::cout << "Consumer started. url=" << url << " lb=" << lb << " qps=" << qps << std::endl;

    int seq = 0;
    while (true) {
        example::EchoRequest req;
        example::EchoResponse res;
        req.set_message("hello #" + std::to_string(++seq));

        brpc::Controller cntl;
        stub.Echo(&cntl, &req, &res, nullptr);
        if (cntl.Failed()) {
            std::cerr << "[Echo] failed: " << cntl.ErrorText() << std::endl;
        } else {
            std::cout << "[Echo@" << cntl.remote_side()
                      << "] " << res.message()
                      << " latency=" << cntl.latency_us() << "us\n";
        }
        PrintEndpoints(ch);  // 打印当前客户端内存中的服务列表
        usleep(1000000 / qps);
    }
    return 0;
}
```

> 选择 `"wrr"` 即可让权重生效（前提：注册中心返回的 `addresses{ ip, port, weight }`，上面我们把 `weight` 放到了 `ServerNode.tag`）。

---

## 选型建议

* **想快点跑起来**：用 **方案 A（file://）**。你的注册中心仍然是“权威源”，只是把**当前视图**同步到一个文件；客户端完全通过 **NamingService（file://）** 自动拉取最新服务列表，满足你的需求且无需修改 brpc。
* **必须是“客户端直接调注册中心 Discover”**：用 **方案 B**，在 brpc 里添加 1 次性注册（改 1 行/少量行，重编 brpc），之后你的业务无需再动 brpc。

无论哪种方案，**客户端都会自动感知 provider 上下线**，并且你可以用 `Channel::ListEndpoints()` 打印“当前客户端内存中的服务列表”。

如果你告诉我你的 brpc 版本 / 构建系统（CMake or bld.sh），我可以把 **具体改哪一个源文件、加哪一行** 和 **CMakeLists 变更** 直接给你，以便最少量变更集成方案 B。
