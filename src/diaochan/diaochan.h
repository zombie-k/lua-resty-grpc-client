//
// Created by xiangqian5 on 2021/4/7.
//

#ifndef CGRPC_DIAOCHAN_H
#define CGRPC_DIAOCHAN_H

#include <grpcpp/grpcpp.h>
#include "src/pb/diaochan.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using diaochan::RecallRequest;
using diaochan::RecallReply;
using diaochan::Diaochan;

class CDiaochan {
public:
    CDiaochan(const std::string& target, int timeout)
    : stub_(Diaochan::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),timeout_(timeout)
    {}

    std::string Recall(const std::string& user) {
        std::cout << "Recall" << std::endl;
        // Data we are sending to the server.
        RecallRequest request;
        request.set_bid("123456700000");
        request.set_uid("5014976811");
        request.set_mid("4622872562107300");
        request.set_channel("profile_merge_v4");
        request.set_version("pro_s");
        request.set_num(100);
        FeatList* context_fl = request.mutable_context();
        Feature* ft = context_fl->add_feat();
        ft->set_fid("page");
        ft->set_val("1");

        ft = context_fl->add_feat();
        ft->set_fid("uicode");
        ft->set_val("1008");

        ft = context_fl->add_feat();
        ft->set_fid("network");
        ft->set_val("4g");

        ft = context_fl->add_feat();
        ft->set_fid("pid");
        ft->set_val("824");

        ft = context_fl->add_feat();
        ft->set_fid("source");
        ft->set_val("100000");
        // Container for the data we expect from the server.
        RecallReply reply;

        // Container for the data we expect from the server.
        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;
        std::chrono::time_point<std::chrono::system_clock> deadline = std::chrono::system_clock::now()+std::chrono::milliseconds(this->timeout_);
        context.set_deadline(deadline);

        // The actual RPC.
        Status status = stub_->Recall(&context, request, &reply);

        if (status.ok()) {
            return reply.SerializeAsString();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Diaochan::Stub> stub_;
    int timeout_;
};

#endif //CGRPC_DIAOCHAN_H
