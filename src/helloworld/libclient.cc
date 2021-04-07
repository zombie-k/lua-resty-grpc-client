//
// Created by xiangqian5 on 2021/4/7.
//
#include "greeter_client.h"

extern "C" {
    void* new_client(const char* target) {
        assert(nullptr != target);
        auto* client = new GreeterClient(target);
        return reinterpret_cast<GreeterClient*>(client);
    }
    void delete_client(GreeterClient* client) {
        delete client;
    }
    char* say_hello(GreeterClient* client, const char* word) {
        return strdup(client->SayHello(word).c_str());
    }
}
