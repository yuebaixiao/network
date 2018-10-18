#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

#define NSEMS 16

int main(int argc, char* argv[]){
  int semid;
  sembuf sb;

  if(argc != 2){
    printf("arg is wrong\n");
    return 1;
  }

  semid = atoi(argv[1]);

  sb.sem_num = 15;//指定要操作的信号量是信号集合中的号码为0信号量
  sb.sem_op = -1;
  sb.sem_flg = SEM_UNDO;

  printf("before semop()\n");
  if(semop(semid, &sb, 1) != 0){
    perror("semop");
    return 1;
  }

  printf("after semop()\n");

  printf("press enter to exti\n");
  getchar();
  return 0;
}
