#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#define MTEXTSIZE 10

int main(int argc, char* argv[]){
  int msgid, msgtype;
  
  struct msgbuf{
    long mtype;
    char mtext[MTEXTSIZE];
  }mbuf;

  if(argc != 3){
    printf("wrong argc");
    return 1;
  }

  msgid = atoi(argv[1]);
  msgtype = atoi(argv[2]);

  if(msgrcv(msgid, &mbuf, MTEXTSIZE, msgtype, 0) <= 0){
    perror("msgrcv");
    return 1;
  }

  printf("%c\n", mbuf.mtext[0]);
  return 0;
}
