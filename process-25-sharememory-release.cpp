#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  int shm;
  shmid_ds sds;

  if(argc != 2){
    printf("argc is wrong");
    return 1;
  }

  shm = atoi(argv[1]);

  if(shmctl(shm, IPC_RMID, &sds) != 0){
    perror("shmctl");
    return 1;
  }

  return 0;
}
