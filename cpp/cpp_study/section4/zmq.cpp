// Copyright (c) 2020 by Chrono
//
// sudo apt-get install libzmq3-dev
//
// g++ zmq.cpp -std=c++14 -lzmq -lpthread -o a.out;./a.out

#include <iostream>

#include <string>
#include <vector>
#include <future>

#include <zmq.hpp>

using namespace std;

const auto thread_num = 1;

zmq::context_t context(thread_num);

auto make_sock = [](auto mode)
{
    return zmq::socket_t(context, mode);
};

void case1()
{
    const auto addr = "ipc:///dev/shm/zmq.sock"s;
    //const auto addr = "inproc:///dev/shm/zmq.sock"s;

    auto receiver = [=]()
    {
        auto sock = make_sock(ZMQ_PULL);

        sock.bind(addr);
        // assert(sock.connected());  //connected() 已被废弃，socket 对象本身已支持 operator bool()。
        assert(sock);

        zmq::message_t msg;

        // sock.recv(&msg);     // 默认 flags=0  第一个参数现在是 引用（msg），不是指针（&msg）！
        if (!sock.recv(msg, zmq::recv_flags::none)) {
            // 处理错误，例如：
            std::cerr << "Failed to receive message" << std::endl;
            return; // 或 break / throw 等
        }

        cout << msg.size() << endl;

        string s = {msg.data<char>(), msg.size()};
        cout << s << endl;
    };

    auto sender = [=]()
    {
        auto sock = make_sock(ZMQ_PUSH);

        sock.connect(addr);
        // assert(sock.connected());
        assert(sock);

        string s = "hello zmq";
        //zmq::message_t msg(begin(s), end(s));
        //sock.send(msg);

        // sock.send(s.data(), s.size());   //zmq::const_buffer 是 cppzmq 提供的类型，用于封装数据指针+长度。  flag=0
        sock.send(zmq::const_buffer{s.data(), s.size()}, zmq::send_flags::none);
    };

    sender();
    receiver();
}

void case2()
{
    const auto addr = "tcp://127.0.0.1:5555"s;

    auto srv = [=]()
    {
        auto sock = make_sock(ZMQ_PULL);

        // sock.setsockopt(ZMQ_LINGER, 0);
        // sock.setsockopt(ZMQ_RCVHWM, 1000);  //所有 ZMQ_XXX 常量现在都对应 zmq::sockopt::xxx（小写）。
        sock.set(zmq::sockopt::linger, 0);
        sock.set(zmq::sockopt::rcvhwm, 1000);

        sock.bind(addr);
        // assert(sock.connected());
        assert(sock);

        zmq::message_t msg;

        for(int i = 0; i < 4; i++) {
            // sock.recv(&msg);
            if (!sock.recv(msg, zmq::recv_flags::none)) {
                // 处理错误，例如：
                std::cerr << "Failed to receive message" << std::endl;
                return; // 或 break / throw 等
            }

            string s = {msg.data<char>(), msg.size()};
            cout << s << endl;
        }
    };

    auto client = [=](int i)
    {
        auto sock = make_sock(ZMQ_PUSH);

        // sock.setsockopt(ZMQ_SNDHWM, 100);   //所有 ZMQ_XXX 常量现在都对应 zmq::sockopt::xxx（小写）。
        sock.set(zmq::sockopt::sndhwm, 100);

        sock.connect(addr);
        // assert(sock.connected());
        assert(sock);

        string s = "zmq msg " + to_string(i);
        //zmq::message_t msg(begin(s), end(s));
        //sock.send(msg);

        //sock.send(begin(s), end(s));
        // sock.send(s.data(), s.size());
        sock.send(zmq::const_buffer{s.data(), s.size()}, zmq::send_flags::none);
    };

    auto f = std::async(srv);

    for(int i = 0; i < 4; i++) {
        client(i);
    }

    f.wait();
}

//顺序	结果
//先 async(srv)（PULL bind）→ 再 client（PUSH send）	✅ 消息可靠接收
//先 client → 再 async(srv)	❌ 消息很可能丢失

int main()
{
    int a, b, c;
    std::tie(a, b, c) = zmq::version();

    cout << "version = "
         << a << b << c << endl;

    case1();
    case2();

    cout << "zmq demo" << endl;
}
