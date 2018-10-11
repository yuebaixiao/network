#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>//getaddrinfo

int main(){
  int sock;
  sockaddr_in addr;
  sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[2048];
  char senderstr[16];
  int n;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock, (sockaddr*)&addr, sizeof(addr));

  while(1){
    memset(buf, 0, sizeof(buf));

    addrlen = sizeof(senderinfo);
    n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
		 (sockaddr*)&senderinfo, &addrlen);

    inet_ntop(AF_INET, &senderinfo.sin_addr, senderstr, sizeof(senderstr));
    printf("recvfrom: %s, port=%d\n", senderstr, ntohs(senderinfo.sin_port));
    
    sendto(sock, buf, n, 0, (sockaddr*)&senderinfo, addrlen);

    printf("send data to: %s, port=%d\n", senderstr, ntohs(senderinfo.sin_port));
    
  }
}
