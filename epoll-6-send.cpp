#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

int main(){
  int sock;
  sockaddr_in dest1, dest2;
  char buf[1024];

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  dest1.sin_family = AF_INET;
  dest2.sin_family = AF_INET;

  inet_pton(AF_INET, "127.0.0.1", &dest1.sin_addr.s_addr);
  inet_pton(AF_INET, "127.0.0.1", &dest2.sin_addr.s_addr);

  dest1.sin_port = htons(11111);
  dest2.sin_port = htons(22222);

  strcpy(buf, "data to port 11111\n");
  //给地址1(dest1)送信
  sendto(sock, buf, strlen(buf), 0, (sockaddr*)&dest1, sizeof(dest1));
  
  strcpy(buf, "data to port 22222\n");
  //给地址2(dest2)送信
  sendto(sock, buf, strlen(buf), 0, (sockaddr*)&dest2, sizeof(dest1));

  close(sock);

  return 0;
}
