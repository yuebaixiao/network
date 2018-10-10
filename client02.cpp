#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[]){

  int err;
  int sock;
  char buf[32];
  char* deststr;
  addrinfo hints, *res0, *res;
  if(argc != 2){return 1;}

  deststr = argv[1];

  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = PF_UNSPEC;
  if((err = getaddrinfo(deststr, "http", &hints, &res0)) != 0){
    printf("error %d:%s\n", err, gai_strerror(err));
    return 1;
  }
  for(res = res0; res != NULL; res = res->ai_next){
    printf("%d\n", res->ai_family);
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sock < 0){continue;}
    if(connect(sock, res->ai_addr, res->ai_addrlen) != 0){
      close(sock);
      continue;
    }
    break;
  }

  freeaddrinfo(res0);

  if(res == NULL){
    printf("failed\n");
    return 1;
  }

  snprintf(buf, sizeof(buf), "GET / HTTP/1.0\r\n\r\n");

  int n = write(sock, buf, (int)strlen(buf));

  while(n > 0){
    n = read(sock, buf, sizeof(buf));
    write(fileno(stdout), buf, n);
  }

  close(sock);
  return 0;
}
