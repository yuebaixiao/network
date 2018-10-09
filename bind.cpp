#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void print_ip_port(int sock){
  char buf[48];
  sockaddr_in s;
  socklen_t sz;

  sz = sizeof(s);

  //取得socket里面的ip地址和端口号
  if(getsockname(sock, (sockaddr*)&s, &sz) != 0){
    perror("getsockname");
    return;
  }

  //把bind的ip转化成字符串
  inet_ntop(AF_INET, &s.sin_addr, buf, sizeof(buf));
  std::cout << buf << ":" << ntohs(s.sin_port) << std::endl;
}
int main(){
  int s0, sock;
  sockaddr_in peer;
  socklen_t peerlen;
  int n;
  char buf[1024];

  //socket创建成功后，马上就调用监听listen
  s0 = socket(AF_INET,SOCK_STREAM,0);
  if(listen(s0, 5) != 0){
    perror("listen");
    return 1;
  }

  //打印出ip地址和端口号
  print_ip_port(s0);

  //等待客户端的连接
  peerlen = sizeof(peer);
  sock = accept(s0, (sockaddr*)&peer, &peerlen);
  if(sock < 0){
    perror("accept");
    return 1;
  }

  write(sock, "hello\n", 6);

  close(s0);
  close(sock);
  
}
