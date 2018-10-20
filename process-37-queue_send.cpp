#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define MTEXTSIZE 10

int main(int argc, char* argv[]){
  int msgid;
  struct msgbuf{
    long mtype;
    char mtext[MTEXTSIZE];
  }mbuf;

  if(argc != 2){
    printf("wrong argc");
    return 1;
  }

  msgid = atoi(argv[1]);

  mbuf.mtype = 777;
  memset(mbuf.mtext, 0, sizeof(mbuf.mtext));
  mbuf.mtext[0] = 'A';

  if(msgsnd(msgid, &mbuf, MTEXTSIZE, 0) != 0){
    perror("msgsnd");
    return 1;
  }

  return 0;
}
