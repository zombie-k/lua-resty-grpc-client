#include <iostream>
#include <src/helloworld/greeter_client.h>
#include "src/diaochan/diaochan.h"
#include "src/pb/helloworld.pb.h"
//using namespace rapidjson;
using namespace std;

void test1() {
    //std::string target("i.diaochan.recom.weibo.com:5001");
    std::string target("10.182.11.176:10810");
    CDiaochan cd(target, 50000);
    //std::string global = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"realtime_general\",\"debug\":true,\"version\":\"hql_v3\"}";
    //std::string global = "{\"num\":10,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"profile_merge_v2\",\"debug\":true,\"version\":\"hql_v3\"}";
    std::string global = "{\"num\":10,\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"profile_merge_v2\",\"debug\":true,\"version\":\"hql_v3\"}";
    std::string context = "{\"id\":\"4618656153081522\",\"page\":\"1\",\"pid\":\"824\",\"network\":\"4g\",\"source\":\"100001\",\"1197\":\"5\",\"1210\":\"5\",\"1209\":\"5\"}";
    std::string reply = cd.Recall(global, context);
    std::cout << "reply:" << reply << std::endl;
}

void async_test1() {

    std::string target("i.diaochan.recom.weibo.com:5001");
    //std::string target("10.182.11.176:10810");
    AsyncDiaochan cd(target, 20000);
    //std::string global = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"realtime_general\",\"debug\":true,\"version\":\"hql_v3\"}";
    std::string global = "{\"num\":10,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"profile_merge_v2\",\"debug\":true,\"version\":\"hql_v3\"}";
    //std::string global = "{\"num\":10,\"uid\":\"6129908645\",\"mid\":\"4618656153081522\",\"channel\":\"profile_merge_v2\",\"debug\":true,\"version\":\"hql_v3\"}";
    std::string context = "{\"id\":\"4618656153081522\",\"page\":\"1\",\"pid\":\"824\",\"network\":\"4g\",\"source\":\"100001\",\"1197\":\"5\",\"1210\":\"5\",\"1209\":\"5\"}";
    std::string reply = cd.AsyncRecall(global, context);
    std::cout << "reply:" << reply << std::endl;
}

void test2() {
    std::string json_string;
    helloworld::HelloRequest sr, sr1, sr2;
    sr.set_name(std::string("Lufy"));
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    MessageToJsonString(sr, &json_string, options);
    std::cout << json_string << std::endl;

    google::protobuf::util::JsonParseOptions options2;
    JsonStringToMessage(json_string, &sr1, options2);
    std::cout << sr1.name() << std::endl;

    google::protobuf::util::JsonParseOptions options3;
    json_string = "{\"name\":\"Lufy\"}";
    JsonStringToMessage(json_string, &sr2, options3);
    std::cout << sr2.name() << std::endl;
}
void test3() {
    std::string json_string;
    RecallRequest sr, sr1;
    google::protobuf::util::JsonParseOptions options3;
    //json_string = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"sessid\":\"5468016179351872570003918\",\"uid\":\"6129908645\",\"mid\":\"\",\"channel\":\"profile_merge_v4\",\"debug\":true,\"version\":\"pro_l\",\"context\":{\"id\":\"\",\"feat\":[{\"fid\":\"page\",\"val\":\"1\"},{\"fid\":\"uicode\",\"val\":\"10000838\"},{\"fid\":\"network\",\"val\":\"4g\"},{\"fid\":\"pid\",\"val\":\"824\"}]}}";
    json_string = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"\",\"channel\":\"profile_merge_v4\",\"debug\":true,\"version\":\"pro_l\"}";
    google::protobuf::util::Status status = JsonStringToMessage(json_string, &sr, options3);
    std::cout << status.ToString() << "," << status.error_message() << std::endl;
    std::cout << sr.num() << std::endl;
}

static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
void test4() {
    RecallRequest r1;
    Document doc;
    std::string json_string = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"\",\"channel\":\"profile_merge_v4\",\"debug\":true,\"version\":\"pro_l\",\"context\":{\"id\":\"BBBB\",\"feat\":[{\"fid\":\"page\",\"val\":\"1\"},{\"fid\":\"t1\",\"val\":\"fffff\"},{\"fid\":\"t2\",\"val\":\"asdfj134\"}]}}";
    doc.Parse(json_string);
    for (Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter) {
        std::cout << iter->name.GetString() << "  " << iter->value.GetType() << "  " << kTypeNames[iter->value.GetType()] << std::endl;
        const rapidjson::Value& name_json = iter->name;
        const rapidjson::Value& value_json = iter->value;
        switch (value_json.GetType()) {
            case rapidjson::kNullType:
                break;
            case rapidjson::kTrueType:
                std::cout << name_json.GetString() << " " << kTypeNames[iter->value.GetType()] << value_json.GetBool() << std::endl;
                break;
            case rapidjson::kFalseType:
                std::cout << name_json.GetString() << " " << kTypeNames[iter->value.GetType()] << value_json.GetBool() << std::endl;
                break;
            case rapidjson::kNumberType:
                std::cout << name_json.GetString() << " " << kTypeNames[iter->value.GetType()] << value_json.GetInt() << std::endl;
                break;
            case rapidjson::kStringType:
                std::cout << name_json.GetString() << " " << kTypeNames[iter->value.GetType()] << " "<< value_json.GetString() << std::endl;
                break;
            default:
                break;
        }
    }

    auto ibid = doc.FindMember("bid");
    if (ibid != doc.MemberEnd()) {
        std::cout << "bid:" << ibid->value.GetString() << std::endl;
    }

    auto itr = doc.FindMember("context");
    if (itr != doc.MemberEnd()) {
        FeatList *context_fl = r1.mutable_context();
        auto obj_context = itr->value.GetObject();
        auto itr_context_id = obj_context.FindMember("id");
        if (itr_context_id != obj_context.MemberEnd()) {
            //std::cout << doc["context"].GetObject()["id"].GetString() << std::endl;
            std::cout << "context id "<< itr_context_id->value.GetString() << std::endl;
            context_fl->set_id(itr_context_id->value.GetString());
        }

        auto itr_context_feat = obj_context.FindMember("feat");
        if (itr_context_feat != obj_context.MemberEnd()) {
            for (auto& f : itr_context_feat->value.GetArray()) {
                std::cout << f["fid"].GetString() << std::endl;
                std::cout << f["val"].GetString() << std::endl;
                Feature *ft = context_fl->add_feat();
                ft->set_fid(f["fid"].GetString());
                ft->set_val(f["val"].GetString());
            }
        }
    }
    std::string reply_str;
    google::protobuf::util::MessageToJsonString(r1, &reply_str);
    std::cout << reply_str << std::endl;
    //const Value& cont = doc["context1"];
}

struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, SizeType length, bool copy) {
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

void test5() {
    //const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    const char json[] = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"\",\"channel\":\"profile_merge_v4\",\"debug\":true,\"version\":\"pro_l\",\"context\":{\"id\":\"\",\"feat\":[{\"fid\":\"page\",\"val\":\"1\"},{\"fid\":\"t1\",\"val\":\"fffff\"},{\"fid\":\"t2\",\"val\":\"asdfj134\"}]}}";
    MyHandler handler;
    Reader reader;
    StringStream ss(json);
    reader.Parse(ss, handler);
}

void async_helloworld() {
    AsyncGreeterClient greeter("localhost:50051");
    std::string user("world");
    std::string reply = greeter.AsyncSayHello(user);  // The actual RPC call!
    std::cout << "Greeter received: " << reply << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    //test1();
    //test2();
    //test3();
    //test4();
    async_test1();
    //async_helloworld();
    return 0;
}

