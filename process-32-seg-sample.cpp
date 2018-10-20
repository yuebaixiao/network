#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MYIPCKEY 0xabcdabcd
#define NSEMS 16

int sem_init(){
  int semid;
  unsigned short semun_array[NSEMS];
  sembuf sb[NSEMS];
  int i;

  semid = semget(MYIPCKEY, NSEMS, 0600 | IPC_CREAT | IPC_EXCL);
  if(semid >= 0){
    for(i = 0; i < NSEMS; ++i){
      semun_array[i] = 1;
    }
    if(semctl(semid, NSEMS, SETALL, &semun_array) != 0){
      perror("semctl");
      return 1;
    }
    printf("[pid:%d] new semaphore set, semid=%d\n", getpid(), semid);

    for(i = 0; i < NSEMS; ++i){
      sb[i].sem_num = i;
      sb[i].sem_op = -1;
      sb[i].sem_flg = SEM_UNDO;
    }

    printf("IF[pid:%d] before semop()\n", getpid());
    printf("IF[pid:%d] press enter to start semop()\n", getpid());
    getchar();

    if(semop(semid, sb, NSEMS)){
      perror("semop");
      return 1;
    }

    printf("IF[pid:%d] press enter to exit this process\n", getpid());
    getchar();

    exit(0);
  }
  else{
    if(errno != EEXIST){
      perror("semget");
      return 1;
    }
    else{
      printf("in else\n");
      semid_ds sds;
      semid = semget(MYIPCKEY, NSEMS, 0600);
      if(semid < 0){
	perror("semget 1");
	return 1;
      }

      printf("ELSE[pid:%d] before semctl()\n", getpid());

      while(true){
	//IPC_STAT的时候，忽略第二个参数
	if(semctl(semid, 0, IPC_STAT, &sds) != 0){
	  perror("semctl 1");
	  return 1;
	}
	printf("###########################\n");
	printf("sem_perm.mode:%d,sem_perm.__seq:%d\n",sds.sem_perm.mode,sds.sem_perm.__seq);
	printf("otime:%ld\n",sds.sem_otime);/* Last semop time */
	printf("ctime:%ld\n",sds.sem_ctime);/* Last change time */
	printf("sem_nsems:%ld\n",sds.sem_nsems);/* No. of semaphores in set */
	printf("###########################\n");
     
	if(sds.sem_otime != 0){
	  break;
	}

	printf("ELSE[pid:%d] waiting otime change...\n", getpid());
	sleep(2);
      }


      sb[0].sem_num = 0;
      sb[0].sem_op = -1;
      sb[0].sem_flg = SEM_UNDO;
      
      sb[1].sem_num = 0;
      sb[1].sem_op = -1;
      sb[1].sem_flg = SEM_UNDO;

      printf("ELSE[pid:%d] before semop()\n", getpid());
      if(semop(semid, sb, 2) != 0){
	perror("semop 1");
	return 1;
      }

      printf("ELSE[pid:%d] after semop()\n", getpid());
    }
  }
  return 0;
}

int main(){
  pid_t pid;
  pid = fork();

  if(sem_init() < 0){
    printf("[pid:%d] sem_init() failed\n", getpid());
  }
}
