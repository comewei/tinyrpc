#include <google/protobuf/service.h>
#include "../src/net/tcp/tcp_server.h"
#include "../src/net/tcp/tcp_client.h"
#include "../src/net/net_address.h"
#include "../src/net/tinypb/tinypb_rpc_dispatcher.h"
#include "../src/net/tinypb/tinypb_rpc_channel.h"
#include "../src/net/tinypb/tinypb_rpc_closure.h"
#include "../src/net/tinypb/tinypb_rpc_controller.h"
#include "../src/comm/log.h"
#include "tinypb.pb.h"

tinyrpc::Logger* gRpcLogger = nullptr;

int main(int argc, char* argv[]) {


  gRpcLogger = new tinyrpc::Logger();
  gRpcLogger->init("./", "test_reactor", 5*1024*1024);
  tinyrpc::IPAddress::ptr self_addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 39999);
  tinyrpc::TcpServer server(self_addr, 1);

  tinyrpc::IPAddress::ptr addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 39999);
  
  tinyrpc::TinyPbRpcChannel channel(addr);
  tinyrpc::TinyPbRpcController rpc_controller;
  DebugLog << "input an integer to set count that send tinypb data";
  int n;
  std::cin >> n;

  while (n--) {

  queryNameReq req_name;
  req_name.set_req_no(20220315);
  req_name.set_id(1234);
  req_name.set_type(1);

  queryNameRes res_name;

  queryAgeReq req_age;
  req_age.set_req_no(00001111);
  req_age.set_id(6781);

  queryAgeRes res_age;

  tinyrpc::TinyPbRpcClosure cb([]() {
    DebugLog << "==========================";
    DebugLog << "succ call rpc";
    DebugLog << "==========================";
  });

  QueryService_Stub stub(&channel);
  stub.query_name(&rpc_controller, &req_name, &res_name, &cb);
  
  stub.query_age(&rpc_controller, &req_age, &res_age, &cb);

  DebugLog << "get res_name.age = " << res_name.name();
  DebugLog << "get res_age.age = " << res_age.age();

  }

  DebugLog << "================";
  return 0;
}
