#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(){
  int p[2];
  pid_t pid;
  char buf[128];

  memset(buf, 0, sizeof(buf));

  if(pipe(p) != 0){
    perror("pipe");
    return 1;
  }

  pid = fork();
  if(pid < 0){
    perror("fork");
    return 1;
  }

  if(pid == 0){
    close(p[1]);
    read(p[0], buf, sizeof(buf));
    printf("child process : data form parent process [%s]\n", buf);
    exit(0);
  }
  else{
    close(p[0]);
    write(p[1], "aaaa", 4);
    printf("parent process : child process is %d\n", pid);
    int status;
    wait(&status);
  }
  return 0;
}
