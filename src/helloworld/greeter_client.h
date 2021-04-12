//
// Created by xiangqian5 on 2021/4/7.
//

#ifndef CGRPC_GREETER_CLIENT_H
#define CGRPC_GREETER_CLIENT_H
#include <memory>
#include <grpcpp/grpcpp.h>
#include "src/pb/helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientAsyncResponseReader;
using grpc::CompletionQueue;
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

class AsyncGreeterClient {
public:
    AsyncGreeterClient(const std::string& target)
    :stub_(Greeter::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials())))
    {}
    std::string AsyncSayHello(const std::string& user) {
        // Data we are sending to the server.
        HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        Status status;

        // stub_->PrepareAsyncSayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // Because we are using the asynchronous API, we need to hold on to
        // the "call" instance in order to get updates on the ongoing RPC.
        std::unique_ptr<ClientAsyncResponseReader<HelloReply> > rpc(
                stub_->PrepareAsyncSayHello(&context, request, &cq));

        // StartCall initiates the RPC call
        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&reply, &status, (void*)1);
        void* got_tag;
        bool ok = false;
        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        cq.Next(&got_tag, &ok);

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        if (ok && got_tag == (void*)1) {
            std::cout << "// check reply and status" << std::endl;
        }
        // Act upon the status of the actual RPC.
        if (status.ok()) {
            return reply.message();
        } else {
            return "RPC failed";
        }
    }
private:
    std::unique_ptr<Greeter::Stub> stub_;
};

#endif //CGRPC_GREETER_CLIENT_H
