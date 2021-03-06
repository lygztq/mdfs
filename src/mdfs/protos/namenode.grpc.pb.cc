// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: mdfs/protos/namenode.proto

#include "mdfs/protos/namenode.pb.h"
#include "mdfs/protos/namenode.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace mdfs {
namespace message {

static const char* NameNodeService_method_names[] = {
  "/mdfs.message.NameNodeService/ClientCommand",
  "/mdfs.message.NameNodeService/Heartbeat",
  "/mdfs.message.NameNodeService/ReportBlock",
};

std::unique_ptr< NameNodeService::Stub> NameNodeService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< NameNodeService::Stub> stub(new NameNodeService::Stub(channel));
  return stub;
}

NameNodeService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_ClientCommand_(NameNodeService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Heartbeat_(NameNodeService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ReportBlock_(NameNodeService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status NameNodeService::Stub::ClientCommand(::grpc::ClientContext* context, const ::mdfs::message::ClientCommandMessage& request, ::mdfs::message::ClientCommandReturnMessage* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_ClientCommand_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::mdfs::message::ClientCommandReturnMessage>* NameNodeService::Stub::AsyncClientCommandRaw(::grpc::ClientContext* context, const ::mdfs::message::ClientCommandMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::mdfs::message::ClientCommandReturnMessage>(channel_.get(), cq, rpcmethod_ClientCommand_, context, request);
}

::grpc::Status NameNodeService::Stub::Heartbeat(::grpc::ClientContext* context, const ::mdfs::message::HeartbeatMessage& request, ::mdfs::message::HeartbeatReturnMessage* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_Heartbeat_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::mdfs::message::HeartbeatReturnMessage>* NameNodeService::Stub::AsyncHeartbeatRaw(::grpc::ClientContext* context, const ::mdfs::message::HeartbeatMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::mdfs::message::HeartbeatReturnMessage>(channel_.get(), cq, rpcmethod_Heartbeat_, context, request);
}

::grpc::Status NameNodeService::Stub::ReportBlock(::grpc::ClientContext* context, const ::mdfs::message::ReportBlockMessage& request, ::mdfs::message::ReportBlockReturnMessage* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_ReportBlock_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::mdfs::message::ReportBlockReturnMessage>* NameNodeService::Stub::AsyncReportBlockRaw(::grpc::ClientContext* context, const ::mdfs::message::ReportBlockMessage& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::mdfs::message::ReportBlockReturnMessage>(channel_.get(), cq, rpcmethod_ReportBlock_, context, request);
}

NameNodeService::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      NameNodeService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< NameNodeService::Service, ::mdfs::message::ClientCommandMessage, ::mdfs::message::ClientCommandReturnMessage>(
          std::mem_fn(&NameNodeService::Service::ClientCommand), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      NameNodeService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< NameNodeService::Service, ::mdfs::message::HeartbeatMessage, ::mdfs::message::HeartbeatReturnMessage>(
          std::mem_fn(&NameNodeService::Service::Heartbeat), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      NameNodeService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< NameNodeService::Service, ::mdfs::message::ReportBlockMessage, ::mdfs::message::ReportBlockReturnMessage>(
          std::mem_fn(&NameNodeService::Service::ReportBlock), this)));
}

NameNodeService::Service::~Service() {
}

::grpc::Status NameNodeService::Service::ClientCommand(::grpc::ServerContext* context, const ::mdfs::message::ClientCommandMessage* request, ::mdfs::message::ClientCommandReturnMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NameNodeService::Service::Heartbeat(::grpc::ServerContext* context, const ::mdfs::message::HeartbeatMessage* request, ::mdfs::message::HeartbeatReturnMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status NameNodeService::Service::ReportBlock(::grpc::ServerContext* context, const ::mdfs::message::ReportBlockMessage* request, ::mdfs::message::ReportBlockReturnMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace mdfs
}  // namespace message

