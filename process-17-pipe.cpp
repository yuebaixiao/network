#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

int main(){
  int p[2];
  pid_t pid;
  char buf[1024];

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
    printf("child process : my_id=%d\n",getpid());

    close(p[0]);
    dup2(p[1], fileno(stdout));

    char *argv[ ]={"ls", "/home/ys/cpp/network"};
    if(execve("/bin/ls", argv, NULL) < 0){
      perror("exec");
      return 1;
    }
    exit(0);
  }else{
    int n;
    FILE* filep;

    close(p[1]);
    printf("parent process : child process id=%d\n", pid);

    filep = fdopen(p[0], "r");
    if(filep == NULL){
      perror("fdopen");
      return 1;
    }

    while(fgets(buf, sizeof(buf), filep) != NULL){
      printf("get:%s\n", buf);
    }
    int status;
    wait(&status);
  }
  return 0;
}
