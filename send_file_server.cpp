#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>//结构体addrinfo, in_addr
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  int sock0;
  sockaddr_in client;
  socklen_t len;
  int sock;
  addrinfo hints, *res;
  int err;
  int fd;
  int n, ret;
  char buf[65536];

  if(argc != 2){return 1;}

  fd = open(argv[1], O_WRONLY | O_CREAT, 0600);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;
  err = getaddrinfo(NULL, "12345", &hints, &res);
  if(err != 0){
    printf("error %d:%s\n", err, gai_strerror(err));
    return 1;
  }

  sock0 = socket(res->ai_family, res->ai_socktype, 0);

  bind(sock0, res->ai_addr, res->ai_addrlen);

  freeaddrinfo(res);

  listen(sock0, 5);

  len = sizeof(client);
  sock = accept(sock0, (sockaddr*)&client, &len);

  while((n = read(sock, buf, sizeof(buf))) > 0){
    ret = write(fd, buf, n);
  }

  close(sock);
  close(sock0);

  return 0;
}
