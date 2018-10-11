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
  sockaddr_in senderinfo;
  socklen_t senderinfolen;
  int n;
  char buf[2048];
  sockaddr_in myname;
  
  if(argc != 2){
    return 1;
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);

  
  myname.sin_family = AF_INET;
  myname.sin_addr.s_addr = INADDR_ANY;
  myname.sin_port = htons(34567);
  bind(sock, (sockaddr*)&myname, sizeof(myname));

  
  n = sendto(sock, "HELLO", 5, 0, (sockaddr*)&addr, sizeof(addr));
  if(n < 1){
    perror("sendto");
    return 1;
  }

  print_port_num(sock);

  memset(buf, 0, sizeof(buf));
  senderinfolen = sizeof(senderinfo);
  recvfrom(sock, buf, sizeof(buf), 0,
	   (sockaddr*)&senderinfo, &senderinfolen);

  printf("%s\n", buf);

  close(sock);

  return 0;
}
