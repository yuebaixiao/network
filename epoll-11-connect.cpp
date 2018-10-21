#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define EVENTS 10

int main(){
  sockaddr_in server;
  int sock, epfd;
  char buf[32];
  int nfds, n;
  int val;
  epoll_event ev, ev_ret[EVENTS];

  sock = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons(12345);
  inet_pton(AF_INET, "127.0.0.1", &server.sin_addr.s_addr);

  epfd = epoll_create(1);
  if(epfd < 0){
    perror("epfd");
    return 1;
  }

  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = sock;

  if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0){
    perror("epoll_ctl");
    return 1;
  }

  val = 1;
  //本来下面的connect函数是阻塞的，但是用FIONBIO的ioctl函数后，就变成非阻塞的了，
  //所以不管connect函数成功与否，都立即结束了。
  ioctl(sock, FIONBIO, &val);
  
  n = connect(sock, (sockaddr*)&server, sizeof(server));

  if(n != 0){
    if(errno == EINPROGRESS){
      printf("before epoll_wait\n");
      
      nfds = epoll_wait(epfd, ev_ret, EVENTS, 1000*10);//timeout is 1 sec
      if(nfds < 0){
	perror("epoll_wait\n");
	return 1;
      }
      printf("after epoll_wait : nfds=%d\n", nfds);
    }
    else{
      perror("connect");
      return 1;
    }
  }

  n = read(sock, buf, sizeof(buf));
  write(fileno(stdout), buf, n);

  close(sock);

  return 0;
  
}
