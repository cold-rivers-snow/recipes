#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>

using namespace std;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& newMessage) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void sub_register(std::shared_ptr<Observer> observer) = 0;  //由于register为关键字，所以函数
    virtual void sub_unregister(const string& name, std::shared_ptr<Observer> observer) = 0; //为了注销提示名字，增加了name参数
    virtual void notify() = 0;
};

class GroupChat : public Subject {
private:
    vector<std::shared_ptr<Observer>> observers;
    vector<string> messages;
public:
    void sub_register(std::shared_ptr<Observer> observer) override {
        observers.push_back(observer);
    }

    void sub_unregister(const string& name, std::shared_ptr<Observer> observer) override {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
        std::cout << name << " Observer unregistered.\n";
    }

    void notify() override {
        for (auto& observer : observers) {
            observer->update(messages.back());
        }
    }

    void sendMessage(const string& msg) {
        messages.push_back(msg);
        notify();
    }
};

class Friend : public Observer {
private:
    string name;
public:
    Friend(const string& n) : name(n) {}

    void update(const string& newMessage) override {
        cout << name << " received a message: " << newMessage << endl;
    }

    string get_name() {
        return name;
    }
};

int main() {
    auto chat = std::make_shared<GroupChat>();
    auto friend1 = std::make_shared<Friend>("Alice");
    auto friend2 = std::make_shared<Friend>("Bob");
    auto friend3 = std::make_shared<Friend>("Charlie");

    chat->sub_register(friend1);
    chat->sub_register(friend2);
    chat->sub_register(friend3);

    chat->sendMessage("Hey everyone, let's plan our trip!");
    

    chat->sub_unregister(friend1->get_name(), friend1);
    chat->sendMessage("Alice get out!");

    chat->sub_unregister(friend2->get_name(), friend2);
    chat->sendMessage("Bob get out!");

    chat->sub_unregister(friend3->get_name(), friend3);
    chat->sendMessage("Charlie get out!");

    return 0;
}