#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
  int sock;
  sockaddr_in addr;
  sockaddr_in senderinfo;
  socklen_t addrlen;
  char buf[2048];
  int n;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(sock, (sockaddr*)&addr, sizeof(addr));

  addrlen = sizeof(senderinfo);

  n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
	   (sockaddr*)&senderinfo, &addrlen);
  write(fileno(stdout), buf, n);
  close(sock);
  return 0;
}
