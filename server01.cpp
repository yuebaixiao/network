#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <netinet/in.h>//结构体sockaddr_in

using namespace std;

int main(){
  int sock0;
  sockaddr_in addr;
  sockaddr_in client;
  socklen_t len;
  int sock;

  //socket 创建
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if(sock0 < 0){return -1;}

  //socket 设定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, (sockaddr*)&addr,sizeof(addr));

  //进入等待客户端请求的状态
  listen(sock0, 5);

  //接收来自客户端的请求
  len = sizeof(client);
  sock = accept(sock0, (sockaddr*)&client, &len);

  //给客户端发送5个文字
  write(sock, "hello", 5);

  //关闭和客户端的连接
  close(sock);

  //关闭监听的socket
  close(sock0);
}
