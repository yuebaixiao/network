#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
  int msgid;

  msgid = msgget(IPC_PRIVATE, 0600);
  if(msgid < 0){
    perror("msgget");
    return 1;
  }

  printf("%d\n", msgid);
  return 0;
}
