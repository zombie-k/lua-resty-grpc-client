//
// Created by xiangqian5 on 2021/4/7.
//

#include "diaochan.h"

std::string CDiaochan::Recall(const std::string& global_param, const std::string& context_param) {
    // Data we are sending to the server.
    RecallRequest request;
    int ok = generate_request(request, global_param, context_param);
    if (ok != 0) {
        return "Param parse error.";
    }
#ifdef DEBUG
    std::string req;
    google::protobuf::util::MessageToJsonString(request, &req);
    std::cout << req << std::endl;
#endif

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
        generate_reply(&reply, result);
        return result;
    } else {
        char buff[100];
        snprintf(buff, sizeof(buff), "%d:%s", status.error_code(), status.error_message().c_str());
        std::string buffAsStdStr = buff;
        return buffAsStdStr;
    }
}

int CDiaochan::generate_request(RecallRequest& req, const std::string& global_param, const std::string& context_param) {
    Document doc;
    doc.Parse(global_param);
    if (!doc.IsObject()) {
        return -1;
    }

    auto itr = doc.FindMember("bid");
    if (itr != doc.MemberEnd()) {
        req.set_bid(itr->value.GetString());
    }

    itr = doc.FindMember("mid");
    if (itr != doc.MemberEnd()) {
        req.set_mid(itr->value.GetString());
    }

    itr = doc.FindMember("uid");
    if (itr != doc.MemberEnd()) {
        req.set_uid(itr->value.GetString());
    }

    itr = doc.FindMember("channel");
    if (itr != doc.MemberEnd()) {
        req.set_channel(itr->value.GetString());
    }

    itr = doc.FindMember("version");
    if (itr != doc.MemberEnd()) {
        req.set_version(itr->value.GetString());
    }

    itr = doc.FindMember("num");
    if (itr != doc.MemberEnd()) {
        req.set_num(itr->value.GetInt());
    }

    itr = doc.FindMember("debug");
    if (itr != doc.MemberEnd()) {
        req.set_debug(itr->value.GetBool());
    }

    Document doc_context;
    doc_context.Parse(context_param);

    if (doc_context.IsObject()) {
        FeatList *req_context = req.mutable_context();
        itr = doc_context.FindMember("id");
        if (itr != doc_context.MemberEnd()) {
            req_context->set_id(itr->value.GetString());
            doc_context.RemoveMember(itr->name.GetString());
        }

        for (Value::ConstMemberIterator iter = doc_context.MemberBegin(); iter != doc_context.MemberEnd(); ++iter) {
            const char* fid = iter->name.GetString();
            const char* val = iter->value.GetString();
            Feature *ft = req_context->add_feat();
            ft->set_fid(fid);
            ft->set_val(val);
        }
    }
    return 0;
}

int CDiaochan::generate_reply(RecallReply *reply, std::string& result)
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

    writer.Key("feat");
    writer.StartArray();
    for (const auto& feat : otherfeas_vec.feat()) {
        writer.StartObject();
        writer.Key("fid");
        writer.String(feat.fid());
        writer.Key("val");
        writer.String(std::any_cast<std::string>(feat.val()));
        writer.EndObject();
    }
    writer.EndArray();// end feat

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
