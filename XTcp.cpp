#include "XTcp.h"
#include <sys/types.h>//socket()
#include <sys/socket.h>//socket()
#include <unistd.h>//close()
#include <arpa/inet.h>//结构体sockaddr_in
#include <string.h>//函数strstr
#include <iostream>

using namespace std;

XTcp::XTcp(){

}

XTcp::~XTcp(){

}

int XTcp::createSocket(){
  sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock == -1){
    cout << "create socket failed" << endl;
  }
  return sock;
}

bool XTcp::Bind(unsigned short port){
  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  //把本地字节序，转成网络字节序
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = htonl(0);
  if(bind(sock,(sockaddr*)&saddr, sizeof(saddr)) != 0){
    cout << "bind failed" << endl;
    return false;
  }
  cout << "success:" << port << endl;
  listen(sock, 10);
  return true;
}

XTcp XTcp::Accept(){

  XTcp tcp;
  
  sockaddr_in caddr;
  socklen_t len = sizeof(caddr);
  //根据原来的sock，生成一个新的socket，叫clinet
  //原来的sock是专门用来建立连接的
  int client = accept(sock, (sockaddr*)&caddr, &len);
  if(client <= 0)return tcp;
  cout << client << endl;
  tcp.ip = inet_ntoa(caddr.sin_addr);
  //把网络字节序，转成本地字节序
  tcp.port = ntohs(caddr.sin_port);
  tcp.sock = client;
  cout << "clinet ip is " << tcp.ip << " port is " << tcp.port << endl;

  return tcp;
}

void XTcp::Close(){
  if(sock <= 0)return;
  close(sock);
}

int XTcp::Recv(char* buf, int bufsize){
  return recv(sock, buf, bufsize, 0);
}
int XTcp::Send(const char* buf, int size){
  //send函数不能保证参数大小的数据一次就能发送成功，
  //所以必须判断send函数的返回值，
  //如果返回值和参数相当，说明都发送过去了，
  //否则需要发送没有发送过去的，直到全部发送过去。
  int s = 0;
  while(s != size){
    int len = send(sock, buf+s, size-s,0);
    if(len <= 0)break;
    s += len;
  }
  return s;
}
