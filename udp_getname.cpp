#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>//getaddrinfo

void print_port_num(int sock){
  sockaddr_in s;
  socklen_t sz = sizeof(s);
  getsockname(sock, (sockaddr*)&s, &sz);
  printf("%d\n", ntohs(s.sin_port));
}
int main(int argc, char* argv[]){
  int sock;
  sockaddr_in addr;
  int n;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  print_port_num(sock);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  inet_pton(AF_INET, "127,0.0.1", &addr.sin_addr.s_addr);

  n = sendto(sock, "HELLO", 5, 0, (sockaddr*)&addr, sizeof(addr));
  if(n < 1){
    perror("sendto");
    return 1;
  }

  print_port_num(sock);

  close(sock);
  return 0;
}
