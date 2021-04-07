#!/usr/bin/env bash

PROTOC=`which protoc`
echo `${PROTOC} --version`
#${PROTOC} -I ./protos --cpp_out=. ./protos/helloworld.proto
#${PROTOC} -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/helloworld.proto

${PROTOC} -I ./protos --cpp_out=. ./protos/diaochan.proto
${PROTOC} -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/diaochan.proto
