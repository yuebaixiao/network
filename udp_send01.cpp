#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
  int sock;
  sockaddr_in addr;
  int n;

  if(argc != 2){
    return 1;
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr);

  n = sendto(sock, "aaaaa", 5, 0, (sockaddr*)&addr, sizeof(addr));
  close(sock);
  return 0;
}
