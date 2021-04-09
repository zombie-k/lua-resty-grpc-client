local ffi = require("ffi")

ffi.cdef[[
typedef struct CDiaochan CDiaochan;
CDiaochan* new_diaochan(const char* target, int timeout);
void delete_diaochan(CDiaochan* client);
char *recall(CDiaochan* client, const char* params);

void free(void *);
]]

local cgrpc
local system = io.popen("uname -s"):read("*l")
if system == "Darwin" then
    cgrpc = ffi.load("libcgrpc.dylib")
else
    cgrpc = ffi.load("libcgrpc.so")
end


local function new()
    local target = "i.diaochan.recom.weibo.com:5001"
    local timeout = 200
    local self = {super = cgrpc.new_diaochan(target, timeout)}
    ffi.gc(self.super, cgrpc.delete_diaochan)
    return self
end

local function recall()
    local test = new()
    local params = "{\"num\":5,\"bid\":\"0000000000000000161793518822426129908645\",\"uid\":\"6129908645\",\"mid\":\"\",\"channel\":\"profile_merge_v4\",\"debug\":true,\"version\":\"pro_l\",\"context\":{\"id\":\"\",\"feat\":[{\"fid\":\"page\",\"val\":\"1\"},{\"fid\":\"pid\",\"val\":\"827\"},{\"fid\":\"network\",\"val\":\"4g\"}]}}"
    local name = cgrpc.recall(test.super, params)
    ffi.gc(name, ffi.C.free)
    local ret = ffi.string(name)
    print(ret)
end

recall()
