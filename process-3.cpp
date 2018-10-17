#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>//wait function

int main(){
  pid_t pid;

  pid = fork();

  if(pid == 0){
    printf("child process : %d\n", getpid());
    //exit(0);
  }
  else{
    int status;
    pid_t waitpid;

    printf("parent process : childpid=%d , mypid=%d\n",pid, getpid());
    waitpid = wait(&status);
    printf("waitpid:%d\n", waitpid);
  }
  printf("end pid: %d\n", getpid());
  return 0;
}
