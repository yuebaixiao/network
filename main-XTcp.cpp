#include <stdlib.h>
#include "XTcp.h"
#include <thread>
#include <iostream>
#include <string.h>

class TcpThread{
public:
  void main(){
    char buf[1024] = {0};
    while(true){
      //虽然指定了接收数据的大小为sizeof(buf) - 1，
      //但实际能接收到多少，是不一定的,
      //len1是实际接到的数据的大小
      int len1 = client.Recv(buf, sizeof(buf) - 1);
      std::cout << "recv len is : " << len1 << std::endl;
      if(len1 <= 0) break;
      buf[len1] = '\0';
      if(strstr(buf, "quit") != NULL){
	char re[] = "quit sucess\n";
	client.Send(re, strlen(re) + 1);
	break;
      }
      //给客户端发送信息，如果发送的数据大，会自动被切割车成很多小块
      //分多次发出去
      //虽然指定了发送数据的大小为3，
      //但实际能发出去多少，是不一定的,
      //sendlen是实际发送出去的数据的大小
      int sendlen = client.Send("OK\n", 4);
      std::cout << "send len is : " << sendlen << std::endl;
      std::cout << "recv is : " << buf << std::endl;

    }
    client.Close();
    delete this;
  }
  XTcp client;
};
int main(int argc, char* argv[]){

  unsigned short port = 8080;
  if(argc > 1){
    port = atoi(argv[1]);
  }

  XTcp server;
  server.createSocket();
  server.Bind(port);
  while(true){
    //accept是阻塞函数，如果没有连接进来就会停在这里
    XTcp client = server.Accept();
    TcpThread* th = new TcpThread();
    th->client = client;
    //开启多线程
    std::thread sth(&TcpThread::main, th);
    //让子线程脱离父线程的监管a
    sth.detach();
  }
  server.Close();
}
