#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  int msgid;
  msqid_ds mds;

  if(argc != 2){
    printf("wrong argv\n");
    return 1;
  }

  msgid = atoi(argv[1]);

  if(msgctl(msgid, IPC_RMID, &mds) != 0){
    perror("msgctl");
    return 1;
  }

  return 0;
}
