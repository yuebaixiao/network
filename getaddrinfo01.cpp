#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>//结构体addrinfo, in_addr
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(){
  char* hostname = "www.cnblogs.com";
  addrinfo hints, *res;
  in_addr addr;
  int err;

  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  if((err = getaddrinfo(hostname, NULL, &hints, &res)) != 0){
    printf("error %d : %s\n", err, gai_strerror(err));
    return 1;
  }
  addr.s_addr = ((sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;
  printf("ip addresss: %s\n", inet_ntoa(addr));
  
  freeaddrinfo(res);

  return 0;
}
