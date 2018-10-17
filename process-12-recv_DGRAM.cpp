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
  char buf[1024];
  int n;

  sock = socket(AF_UNIX, SOCK_DGRAM, 0);

  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/afu_dgram");

  bind(sock, (sockaddr*)&addr, sizeof(addr));

  while(1){
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf) - 1, 0);
    printf("recv:%s\n", buf);
  }

  close(sock);
  return 0;
}
