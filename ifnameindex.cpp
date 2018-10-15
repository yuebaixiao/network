#include <stdio.h>
#include <string.h>
#include <net/if.h>

int main(){
  int index;
  char buf[128];

  index = if_nametoindex("enp0s3");
  if(index == 0){
    perror("if_nametoindex");
    return 1;
  }
  printf("index:%d\n", index);

  memset(buf, 0, sizeof(buf));

  if(if_indextoname(index, buf) == NULL){
    perror("if_indextoname");
    return 1;
  }
  printf("name:%s\n", buf);
  return 0;
}
