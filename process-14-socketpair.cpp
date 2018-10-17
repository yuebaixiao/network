#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <wait.h>

int main(){
  int sv[2];
  pid_t pid;
  char buf[128];

  memset(buf, 0, sizeof(buf));

  if(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0){
    perror("socketpair");
    return 1;
  }

  pid = fork();
  if(pid < 0){
    perror("fork");
    return 1;
  }
  if(pid == 0){
    close(sv[0]);
    read(sv[1], buf, sizeof(buf));
    printf("child process : data from parant process [%s]\n", buf);
    exit(0);
  }
  else {
    int status;
    close(sv[1]);
    write(sv[0], "HELLO", 5);
    printf("parent process : child process id %d\n", pid);
    wait(&status);
  }

  return 0;
}
