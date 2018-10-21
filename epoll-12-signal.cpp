#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>

void sigusr1_handler(int sig){
  write(fileno(stdout), "signal called\n", 14);
}

int main(){
  int nfds;
  int epfd;

  signal(SIGUSR1, sigusr1_handler);

  epfd = epoll_create(1);
  if(epfd < 0){
    perror("epoll_crreate");
    return 1;
  }

  printf("before epoll_wait\n");

  //一直等下去
  nfds = epoll_wait(epfd, NULL, 1, -1);
  printf("after epoll_wait:%d\n", nfds);

  printf("%d\n", errno);
  perror("perror after epoll_wait");

  return 0;
}
