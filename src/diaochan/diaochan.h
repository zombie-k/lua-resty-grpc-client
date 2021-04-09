//
// Created by xiangqian5 on 2021/4/7.
//

#ifndef CGRPC_DIAOCHAN_H
#define CGRPC_DIAOCHAN_H

#include <iostream>
#include <any>
#include <grpcpp/grpcpp.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <google/protobuf/util/json_util.h>
#include "src/pb/diaochan.grpc.pb.h"


using ::google::protobuf::RepeatedPtrField;
using ::google::protobuf::string;
using ::google::protobuf::RepeatedField;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using diaochan::RecallRequest;
using diaochan::RecallReply;
using diaochan::Diaochan;
using namespace rapidjson;

class CDiaochan {
public:
    CDiaochan(const std::string& target, int timeout)
    : stub_(Diaochan::NewStub(grpc::CreateChannel(target, grpc::InsecureChannelCredentials()))),timeout_(timeout)
    {}

    std::string Recall(const std::string& param);

    int generate_reply(RecallReply* reply, std::string& result);
    int generate_request(RecallRequest& req, const std::string& input);

private:
    std::unique_ptr<Diaochan::Stub> stub_;
    int timeout_;
};

#endif //CGRPC_DIAOCHAN_H
