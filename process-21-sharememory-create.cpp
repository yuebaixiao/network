#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(){
  int shm;
  char* ptr;

  shm = shmget(IPC_PRIVATE, 129, IPC_CREAT | 0600);
  if(shm < 0){
    perror("shmget");
    return 1;
  }

  ptr = (char*)shmat(shm, NULL , 0);
  if(atoi(ptr) == -1){
    perror("shmat");
    return -1;
  }
  strcpy(ptr, "HELLO");

  shmdt(ptr);

  return 0;
}
