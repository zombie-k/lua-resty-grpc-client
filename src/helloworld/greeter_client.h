//
// Created by xiangqian5 on 2021/4/7.
//

#ifndef HELLOWORLD_GREETER_CLIENT_H
#define HELLOWORLD_GREETER_CLIENT_H
#include <memory>
#include <grpcpp/grpcpp.h>
#include "src/pb/helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

class GreeterClient {
public:
    GreeterClient(const std::string& target)
            : stub_(Greeter::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials())))
    {}

    std::string SayHello(const std::string& user) {
        // Data we are sending to the server.
        HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};


#endif //HELLOWORLD_GREETER_CLIENT_H
