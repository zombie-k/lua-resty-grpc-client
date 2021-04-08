cmake_minimum_required(VERSION 3.15)
project(cgrpc)

set(ENV{PKG_CONFIG_PATH} /usr/local/lib/pkgconfig)

set(protobuf_MODULE_COMPATIBLE TRUE)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/pb)

if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# grpc
find_path(GRPC_INCLUDE_PATH "grpc/grpc.h")
include_directories(${GRPC_INCLUDE_PATH})
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set (OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl@1.1/1.1.1h")
find_package(OpenSSL REQUIRED)
message(STATUS ${OPENSSL_ROOT_DIR})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/local/include -L/usr/local/lib -lprotobuf -lpthread -L/usr/local/include/grpc++ -lgrpc++ -L/usr/local/include/grpc -lgrpc -g -O3 -Wall -fPIC -fno-var-tracking -DRAPIDJSON_HAS_STDSTRING")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/local/include -L/usr/local/lib -lprotobuf -lpthread -L/usr/local/include/grpc++ -lgrpc++ -L/usr/local/include/grpc -lgrpc -g -O3 -Wall -fPIC -fno-var-tracking -DRAPIDJSON_HAS_STDSTRING")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Get proto files
set (pb_path "${CMAKE_CURRENT_BINARY_DIR}/src/pb")
set (files "helloworld" "basic" "diaochan")
foreach(proto_name ${files})
    get_filename_component(proto "protos/${proto_name}.proto" ABSOLUTE)
    get_filename_component(proto_path "${proto}" PATH)
    # Generate source files
    set(proto_src "${pb_path}/${proto_name}.pb.cc")
    set(proto_hdr "${pb_path}/${proto_name}.pb.h")
    set(grpc_src "${pb_path}/${proto_name}.grpc.pb.cc")
    set(grpc_hdr "${pb_path}/${proto_name}.grpc.pb.h")
    list(APPEND proto_srcs "${pb_path}/${proto_name}.pb.cc")
    list(APPEND grpc_srcs "${pb_path}/${proto_name}.grpc.pb.cc")
    add_custom_command(
            OUTPUT "${proto_src}" "${proto_hdr}" "${grpc_src}" "${grpc_hdr}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "${pb_path}"
            --cpp_out "${pb_path}"
            -I "${proto_path}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${proto}"
            DEPENDS "${proto}"
    )
endforeach()

#add_library(helloworld SHARED main.cpp ${proto_srcs} ${grpc_srcs} src/helloworld/libclient.cc)
#add_library(cgrpc SHARED src/helloworld/libclient.cc ${proto_srcs} ${grpc_srcs} src/helloworld/libclient.cc src/diaochan/diaochan.cc src/diaochan/diaochan.h)
add_executable(cgrpc main.cpp ${proto_srcs} ${grpc_srcs} src/diaochan/diaochan.cc src/diaochan/diaochan.h)
#add_library(cgrpc SHARED src/helloworld/libclient.cc ${proto_srcs} ${grpc_srcs} src/helloworld/libclient.cc)
#add_library(helloworld SHARED helloworld/libclient.cpp helloworld/greeter_client.h ${proto_srcs} ${grpc_srcs})
#add_library(helloworld main.cpp ${proto_srcs} ${grpc_srcs} src/helloworld/greeter_client.cc src/helloworld/greeter_client.h)
#target_link_libraries(helloworld ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
