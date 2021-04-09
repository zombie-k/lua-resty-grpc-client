//
// Created by xiangqian5 on 2021/4/7.
//

#include "diaochan.h"

std::string CDiaochan::Recall(const std::string& param) {
    // Data we are sending to the server.
    RecallRequest request;
    int ok = generate_request(request, param);
    if (ok != 0) {
        return "Param parse error.";
    }

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

int CDiaochan::generate_request(RecallRequest& req, const std::string& input) {
    Document doc;
    doc.Parse(input);
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

    itr = doc.FindMember("context");
    if (itr != doc.MemberEnd()) {
        FeatList *context_fl = req.mutable_context();
        auto context_obj = itr->value.GetObject();
        auto itr_context_id = context_obj.FindMember("id");
        if (itr_context_id != context_obj.MemberEnd()) {
            context_fl->set_id(itr_context_id->value.GetString());
        }

        auto itr_context_feat = context_obj.FindMember("feat");
        if (itr_context_feat != context_obj.MemberEnd()) {
            for (auto& f : itr_context_feat->value.GetArray()) {
                Feature *ft = context_fl->add_feat();
                ft->set_fid(f["fid"].GetString());
                ft->set_val(f["val"].GetString());
            }
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
