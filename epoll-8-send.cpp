#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

int main(){
  sockaddr_in server;
  int sock;
  char buf[65536];
  int n;

  sock = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(12345);

  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  n = connect(sock, (sockaddr*)&server, sizeof(server));
  if(n != 0){
    perror("connect");
    return 1;
  }

  int cnt = 0;
  while(1){
    ++cnt;
    printf("[%d]write %ld bytes\n", cnt, sizeof(buf));
    n = write(sock, buf, sizeof(buf));
    if(n <= 0){
      printf("write error:%d\n", n);
      break;
    }
  }

  close(sock);

  return 0;
  
}
