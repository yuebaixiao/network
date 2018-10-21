#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define EVENTS 12

int main(){
  sockaddr_in server;
  epoll_event ev, ev_ret[EVENTS];
  int sock, epfd;
  char buf[65536];
  int nfds;
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

  epfd = epoll_create(2);
  if(epfd < 0){
    perror("epfd");
    return 1;
  }

  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLOUT;//可写
  ev.data.fd = sock;
  if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0){
    perror("epoll_clt");
    return 1;
  }

  int cnt = 0;
  while(1){
    cnt++;
    printf("before epoll wait\n");

    nfds = epoll_wait(epfd, ev_ret, EVENTS, -1);
    if(nfds < 0){
      perror("epoll_wait");
      return 1;
    }

    printf("after epoll_wait\n");

    if(ev_ret[0].data.fd == sock){
      printf("[%d]write %ld types\n", cnt, sizeof(buf));

      n = write(sock, buf, sizeof(buf));
      if(n <= 0){
	printf("write error:%d\n", n);
	break;
      }
    }
  }

  close(sock);
  return 0;
}
