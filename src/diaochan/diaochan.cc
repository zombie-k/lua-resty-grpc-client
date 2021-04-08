//
// Created by xiangqian5 on 2021/4/7.
//

#include "diaochan.h"

std::string CDiaochan::Recall(const std::string& user) {
    // Data we are sending to the server.
    RecallRequest request;
    request.set_bid("123456700000");
    request.set_uid("5014976811");
    request.set_mid("4622872562107300");
    request.set_channel("profile_merge_v4");
    request.set_version("pro_s");
    request.set_num(5);
    FeatList *context_fl = request.mutable_context();
    Feature *ft = context_fl->add_feat();
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
    std::chrono::time_point<std::chrono::system_clock> deadline =
            std::chrono::system_clock::now() + std::chrono::milliseconds(this->timeout_);
    context.set_deadline(deadline);

    // The actual RPC.
    Status status = stub_->Recall(&context, request, &reply);

    if (status.ok()) {
        std::string result = "";
        generate_diaochan(&reply, result);
        //google::protobuf::util::MessageToJsonString(reply, &reply_str);
        //std::cout << reply_str << std::endl;
        return result;
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }
}

int CDiaochan::generate_diaochan(RecallReply *reply, std::string& result)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartObject();

    writer.Key("bid");
    writer.String(reply->bid());

    writer.Key("status");
    writer.Int(reply->status());

    writer.Key("errmsg");
    writer.String(reply->errmsg());

    // mid
    writer.Key("mid");
    writer.StartArray();
    for (auto & itr : reply->mid()) {
        writer.String(itr);
    }
    writer.EndArray();
    // mid end

    // mscore
    writer.Key("mscore");
    writer.StartArray();
    for (float itr : reply->mscore()) {
        writer.Double(itr);
    }
    writer.EndArray();
    // mscore end

    // mfeas
    writer.Key("mfeas");
    writer.StartArray();
    for (const auto& mfeas : reply->mfeas()) {
        writer.StartObject();
        writer.Key("id");
        writer.String(mfeas.id());

        writer.Key("feat");
        writer.StartArray();
        for (const auto& feat : mfeas.feat()) {
            writer.StartObject();
            writer.Key("fid");
            writer.String(feat.fid());
            writer.Key("val");
            writer.String(std::any_cast<std::string>(feat.val()));
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
    // mfeas end

    //otherfeas
    FeatList otherfeas_vec = reply->otherfeas();
    writer.Key("otherfeas");
    writer.StartObject();
    writer.Key("id");
    writer.String(otherfeas_vec.id());
    for (const auto& feat : otherfeas_vec.feat()) {
        writer.StartObject();
        writer.Key("fid");
        writer.String(feat.fid());
        writer.Key("val");
        writer.String(std::any_cast<std::string>(feat.val()));
        writer.EndObject();
    }
    writer.EndObject();
    // otherfeas end

    // debuginfo
    writer.Key("debuginfo");
    writer.StartArray();
    for (auto & itr : reply->debuginfo()) {
        writer.String(itr);
    }
    writer.EndArray();
    // debuginfo end

    writer.EndObject();
    result = buffer.GetString();
    return 0;
}
