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
  int sock1, sock2;
  sockaddr_in addr1, addr2;
  int epfd;
  epoll_event ev, ev_ret[EVENTS];
  char buf[2048];
  int i;
  int nfds;
  int n;

  //创建2个接受消息的socket
  sock1 = socket(AF_INET, SOCK_DGRAM, 0);
  sock2 = socket(AF_INET, SOCK_DGRAM, 0);
  addr1.sin_family = AF_INET;
  addr2.sin_family = AF_INET;

  inet_pton(AF_INET, "127.0.0.1", &addr1.sin_addr.s_addr);
  inet_pton(AF_INET, "127.0.0.1", &addr2.sin_addr.s_addr);

  addr1.sin_port = htons(11111);
  addr2.sin_port = htons(22222);

  bind(sock1, (sockaddr*)&addr1, sizeof(addr1));
  bind(sock2, (sockaddr*)&addr2, sizeof(addr2));

  //参数不小于0就行
  epfd = epoll_create(1);
  if(epfd < 0){
    perror("epoll_create");
    return 1;
  }

  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;//只读
  ev.data.fd = sock1;//把sock1加到epoll
  if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock1, &ev) != 0){
    perror("epoll_ctl");
    return 1;
  }

  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;//只读
  ev.data.fd = sock2;//把sock2加到epoll
  if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock2, &ev) != 0){
    perror("epoll_ctl");
    return 1;
  }

  while(1){
    printf("before epoll_wait\n");
    //在这里会阻塞，直到有socket进来.
    nfds = epoll_wait(epfd, ev_ret, EVENTS, -1);
    if(nfds <= 0){
      perror("epoll_wait");
      return 1;
    }

    printf("after epoll_wait\n");

    for(i = 0; i < nfds; ++i){
      //判断进来的socket是哪个socket
      if(ev_ret[i].data.fd == sock1){
	//从sock1读取数据，并写入到标准输出
	n = recv(sock1, buf, sizeof(buf), 0);
	write(fileno(stdout), buf, n);
      }
      //判断进来的socket是哪个socket
      else if(ev_ret[i].data.fd == sock2){
	//从sock1读取数据，并写入到标准输出
	n = recv(sock2, buf, sizeof(buf), 0);
	write(fileno(stdout), buf, n);
      }
    }
  }

  close(sock1);
  close(sock2);
  return 0;
}
