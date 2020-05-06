#!/bin/bash
protoc -Isrc/ --cpp_out=src/ src/mdfs/protos/block_info.proto
protoc -Isrc/ --grpc_out=src/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` src/mdfs/protos/datanode.proto
protoc -Isrc/ --grpc_out=src/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` src/mdfs/protos/namenode.proto

mkdir include/mdfs/protos

mv ./src/mdfs/protos/*.pb.h ./include/mdfs/protos
