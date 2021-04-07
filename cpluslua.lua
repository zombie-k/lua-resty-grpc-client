local ffi = require("ffi")

ffi.cdef[[
typedef struct GreeterClient GreeterClient;
GreeterClient* new_client(const char* target);
void delete_client(GreeterClient* client);
char *say_hello(GreeterClient* client, const char* word);

void free(void *);
]]

local foo = ffi.load("libhelloworld.dylib")

local function hello()
    local target = "127.0.0.1:50051"
    local self = {super = foo.new_client(target)}
    ffi.gc(self.super, foo.delete_client)
    return self
end

local function run_hello()
    local test = hello()
    local name = foo.say_hello(test.super, "client_hello_world")
    ffi.gc(name, ffi.C.free)
    print(ffi.string(name))
end

--run()
run_hello()
