# README

## 实现说明

该示例基于 brpc 框架实现了一个简单的服务注册中心，核心功能包括：

1. **注册中心服务（registry_server）**
   - 提供服务注册（Register）、注销（Unregister）、发现（Discover）接口
   - 内置健康检查机制（通过心跳检测服务存活状态，超时自动移除）
   - 使用哈希表维护服务名到地址列表的映射关系

2. **服务提供者（service_provider）**
   - 实现具体业务服务（示例中为Echo服务）
   - 启动时向注册中心注册自身地址
   - 定期发送心跳包维持在线状态
   - 退出时主动注销服务

3. **服务消费者（service_consumer）**
   - 实现自定义 NameService（RegistryNameService），集成到 brpc 框架
   - 通过注册中心动态发现服务地址
   - 使用 brpc 内置的负载均衡（轮询策略）调用服务

4. **关键技术点**
   - 基于 brpc 的 Server/Channel 实现 RPC 通信
   - 自定义 NameService 实现服务发现逻辑
   - 线程安全的数据结构（互斥锁保护服务列表）
   - 心跳机制实现服务健康检查


## 使用方法

1. 编译protobuf文件：
```bash
protoc --cpp_out=. registry_service.proto echo_service.proto
```

2. 编译各模块（需链接brpc和protobuf库）大于gcc4 编译：
```bash
g++ -o registry_server registry_server.cpp registry_service.pb.cc -lbrpc -lprotobuf -lpthread
g++ -o service_provider service_provider.cpp registry_service.pb.cc echo_service.pb.cc -lbrpc -lprotobuf -lpthread
g++ -o service_consumer service_consumer.cpp registry_service.pb.cc echo_service.pb.cc -lbrpc -lprotobuf -lpthread
g++ -std=c++11 service_consumer.cpp registry_service.pb.cc echo_service.pb.cc -lbrpc  -lprotobuf -lgflags -lpthread -o service_consumer
```

3. 启动流程：
```bash
# 启动注册中心（监听8888端口）
./registry_server

# 启动服务提供者（IP、端口、注册中心IP）, 可启动多个实例
./service_provider 127.0.0.1 8080 127.0.0.1
./service_provider 127.0.0.1 8081 127.0.0.1
./service_provider 127.0.0.1 8082 127.0.0.1

# 启动服务消费者（注册中心IP和端口）
./service_consumer 127.0.0.1 18888
```

## 测试说明

service_consumer.cpp 使用 list 得到服务列表，service_consumer1.cpp 增加了客户端发送到服务列表以及正常下线，kill 掉，service_consumer 都不会再往已经掉线的 service_provider 中发送，可以通过 socket 感知到，但是列表中的内容不会改变。

service_consumer2.cpp 使用自定义 NamingService 得到服务列表，service_consumer2.cpp 增加了客户端发送到服务列表以及正常下线，kill 掉，service_consumer 都不会再往已经掉线的 service_provider 中发送，可以通过 socket 感知到，客户端服务列表中的内容也会更新，正常下线更新较快，kill 掉会等注册中心确定 30 秒后才会更新，可以调节健康检测时间与列表清理时间。


## 生产环境扩展建议

1. **高可用**：将注册中心集群化，避免单点故障，使用 braft 实现一组，保证服务高可用
2. **数据持久化**：将服务列表持久化到磁盘（如RocksDB，Dynamodb），支持重启恢复
3. **安全性**：添加认证机制，防止恶意注册/注销
4. **性能优化**：服务列表缓存、批量操作接口、压缩传输
5. **更完善的负载均衡**：支持权重、地域亲和性等策略

该示例仅展示核心原理，实际生产环境需根据业务需求进行扩展。