#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(){
  int sock;
  sockaddr_un addr;
  socklen_t addrlen;

  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/afu_dgram");

  int n = sendto(sock, "HELLO\n", 6, 0, (sockaddr*)&addr, sizeof(addr));
  printf("send data\n");
  close(sock);
  return 0;
}
