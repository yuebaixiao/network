#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <errno.h>

#define EVENTS 12
#define BUFSIZE 1024

enum mystate{
  READ = 0,
  WRITE
};

int main(){
  int sock0;
  sockaddr_in addr;
  sockaddr_in client;
  socklen_t len;
  int sock;
  int n, i;
  epoll_event ev, ev_ret[EVENTS];
  int epfd;
  int nfds;

  struct clientinfo{
    int fd;
    char buf[1024];
    int n;
    int state;
  };
  
  sock0 = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sock0, (sockaddr*)&addr, sizeof(addr));

  listen(sock0, 5);
  epfd = epoll_create(1);
  if(epfd < 0){
    perror("epoll_create");
    return 1;
  }

  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.ptr = malloc(sizeof(clientinfo));
  if(ev.data.ptr == NULL){
    perror("malloc");
    return 1;
  }
  memset(ev.data.ptr, 0, sizeof(clientinfo));

  ((clientinfo*)ev.data.ptr)->fd = sock0;

  if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock0, &ev) != 0){
    perror("epoll_ctl");
    return 1;
  }

  while(1){
    nfds = epoll_wait(epfd, ev_ret, EVENTS, -1);
    if(nfds < 0){
      perror("epoll_wait");
      return 1;
    }

    printf("after epoll_wait : nfds=%d\n", nfds);

    for(i = 0; i < EVENTS; ++i){
      clientinfo* ci = (clientinfo*)ev_ret[i].data.ptr;
      printf("fd=%d\n", ci->fd);

      if(ci->fd == sock0){
	len = sizeof(client);
	sock = accept(sock0, (sockaddr*)&client, &len);
	if(sock < 0){
	  perror("accept");
	  return 1;
	}
	printf("accept sock=%d\n", sock);

	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN | EPOLLONESHOT;
	ev.data.ptr = malloc(sizeof(clientinfo));
	if(ev.data.ptr == NULL){
	  perror("malloc");
	  return 1;
	}
	memset(ev.data.ptr, 0, sizeof(clientinfo));

	((clientinfo*)ev.data.ptr)->fd = sock;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) != 0){
	  perror("epoll_ctl");
	  return 1;
	}
      }
      else{
	if(ev_ret[i].events & EPOLLIN){
	  ci->n = read(ci->fd, ci->buf, BUFSIZE);
	  if(ci->n < 0){
	    perror("read");
	    return 1;
	  }
	  ci->state = WRITE;
	  ev_ret[i].events = EPOLLOUT;

	  if(epoll_ctl(epfd, EPOLL_CTL_MOD, ci->fd, &ev_ret[i]) != 0){
	    perror("epoll_ctl");
	    return 1;
	  }
	}
	else if(ev_ret[i].events & EPOLLOUT){
	  n = write(ci->fd, ci->buf, ci->n);
	  if(n < 0){
	    perror("write");
	    return 1;
	  }

	  if(epoll_ctl(epfd, EPOLL_CTL_DEL, ci->fd, &ev_ret[i]) != 0){
	    perror("epoll_ctl");
	    return 1;
	  }

	  close(ci->fd);
	  free(ev_ret[i].data.ptr);
	}
      }
    }
  }

  close(sock0);

  return 0;
}
