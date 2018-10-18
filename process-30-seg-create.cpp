#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NSEMS 16

int main(){
  int semid;
  unsigned short semun_array[NSEMS];
  int i;

  semid = semget(IPC_PRIVATE, NSEMS, 0600);
  if(semid < 0){
    perror("semget");
    return 1;
  }

  for(i =0; i < NSEMS; ++i){
    semun_array[i] = 2;
  }

  if(semctl(semid, 1000, SETALL, &semun_array) != 0){
    perror("semctl");
    return 1;
  }

  printf("semid:%d\n", semid);
  return 0;
}
