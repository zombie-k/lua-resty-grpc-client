cmake_minimum_required(VERSION 3.15)
project(cgrpc)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/pb)

find_package(Threads REQUIRED)

#rapidjson
find_package(RapidJSON)
message(STATUS "RapidJSON ${RapidJSON_VERSION}")

# --protobuf
set (PROTOBUF_LIBRARY_PATH /usr/local/protobuf/lib)
find_package(Protobuf REQUIRED protobuf)
include_directories(${PROTOBUF_INCLUDE_DIRS})
find_library(PROTOBUF_LIBRARY_PATH NAMES libprotobuf.a protobuf)
message(STATUS "protobuf ${Protobuf_VERSION}")
message(STATUS "Found probobuf: " ${PROTOBUF_LIBRARY_PATH})

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
#find_library(GRPCCPP_LIBRARY_PATH NAMES libgrpc++.a grpc++)
#message(STATUS "Found libgrpc++: " ${GRPCCPP_LIBRARY_PATH})
#find_library(GRPC_LIBRARY_PATH NAMES libgrpc.a grpc)
#message(STATUS "Found libgrpc: " ${GRPC_LIBRARY_PATH})

if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -I/usr/local/include -L/usr/local/lib -lprotobuf -lpthread -L/usr/local/include/grpc++ -lgrpc++ -L/usr/local/include/grpc -lgrpc -g -O3 -Wall -fopenmp -fPIC -fno-var-tracking -DRAPIDJSON_HAS_STDSTRING")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -I/usr/local/include -L/usr/local/lib -lprotobuf -lpthread -L/usr/local/include/grpc++ -lgrpc++ -L/usr/local/include/grpc -lgrpc -g -O3 -Wall -fPIC -fno-var-tracking -DRAPIDJSON_HAS_STDSTRING")
endif()


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

set (cgrpc_src
    src/libcgrpc.cc
    src/diaochan/diaochan.cc
    src/helloworld/greeter_client.cc
    ${proto_srcs}
    ${grpc_srcs})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/src)

add_library(cgrpc SHARED ${cgrpc_src})
