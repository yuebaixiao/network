#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  int shm;
  char* ptr;

  if(argc != 2){
    return 1;
  }

  shm = atoi(argv[1]);

  ptr = (char*)shmat(shm, NULL, 0);
  if(atoi(ptr) == -1){
    perror("shmat");
    return 1;
  }

  printf("string from shared memory : %s\n", ptr);

  shmdt(ptr);

  return 0;
}
