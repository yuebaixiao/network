#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/wait.h>

int main(){
  pthread_mutex_t *m;
  pthread_mutexattr_t mat;
  int shmid;
  pid_t pid;

  shmid = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), 0600);
  if(shmid < 0){
    perror("shmget");
    return 1;
  }

  m = (pthread_mutex_t*)shmat(shmid, NULL, 0);

  //准备设定mutex的attribute
  pthread_mutexattr_init(&mat);

  //利用mutex进行进程间的通信
  //底下这句没有的话，这个mutex只在本进程间有作用
  if(pthread_mutexattr_setpshared(&mat, PTHREAD_PROCESS_SHARED) != 0){
    perror("pthread_mutexattr_setpshared");
    return 1;
  }
  pthread_mutex_init(m, &mat);

  pid = fork();

  printf("[%s] before pthread_mutex_lock()\n",
	 pid == 0 ? "child" : "parent");

  if(pthread_mutex_lock(m) != 0){
    perror("pthread_mutex_lock");
    return 1;
  }

  printf("[%s] press enter\n", pid == 0 ? "child" : "parent");
  getchar();

  if(pthread_mutex_unlock(m) != 0){
    perror("pthread_mutex_unlock");
    return 1;
  }

  printf("[%s] after pthread_mutex_lock()\n",
	 pid == 0 ? "child" : "parent");

  shmdt(m);

  if(pid != 0){
    wait(NULL);//wait child process to complete
    printf("[%s] after wait()\n", pid == 0 ? "child" : "parent");

    //delete shared memery
    if(shmctl(shmid, IPC_RMID, NULL) != 0){
      perror("shmctl");
      return 1;
    }
  }

  return 0;
}
