#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>//getaddrinfo

int main(int argc, char* argv[]){
  int sock;
  addrinfo hints, *res;
  int err;
  int n;
  printf("argv[1]: %s\n", argv[1]);
  if(argc !=2)return 1;

  //IP地址和网站都可以识别
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;///IP地址和网站都可以识别
  hints.ai_socktype = SOCK_DGRAM;
  err = getaddrinfo(argv[1], "12345", &hints, &res);
  if(err != 0){
    printf("getaddrinfo: %s\n", gai_strerror(err));
  }

  sock = socket(res->ai_family, res->ai_socktype, 0);
  if(sock < 0){
    perror("socket");
    return 1;
  }
  
  {
    const char* ipver;
    switch(res->ai_family){
    case AF_INET:
      ipver = "IPv4";
    case AF_INET6:
      ipver = "IPv6";
    default:
      ipver = "unknown";
    }
    printf("%s\n", ipver);
  }

  n = sendto(sock, "bbbb\n", 5, 0, res->ai_addr, res->ai_addrlen);
  if(n < 0){
    perror("sendto");
    return 1;
  }
  close(sock);
  freeaddrinfo(res);

  return 0;
}
