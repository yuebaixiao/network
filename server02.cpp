#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
  int sock0;
  sockaddr_in client;
  socklen_t len;
  int sock;
  int yes = 1;
  addrinfo *res, hints;
  int err;
  char buf[2048];
  int n;
  char inbuf[2048];

  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  err = getaddrinfo(NULL, "12345", &hints, &res);
  if(err != 0){
    printf("getaddrinfo %s\n", gai_strerror(err));
    return 1;
  }

  sock0 = socket(res->ai_family, res->ai_socktype, 0);

  setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
  bind(sock0, res->ai_addr, res->ai_addrlen);
  listen(sock0, 5);

  snprintf(buf, sizeof(buf),
	   "HTTP/1.0 200 OK\r\n"
	   "Content-Length: 20\r\n"
	   "Content-Type:text/html\r\n"
	   "\r\n"
	   "HELLO\r\n");
  while(1){
    len = sizeof(client);
    sock = accept(sock0, (sockaddr*)&client, &len);
    n = read(sock, inbuf, sizeof(inbuf));
    write(fileno(stdout), inbuf, n);
    write(sock, buf, (int)strlen(buf));
    close(sock);
  }
  close(sock0);
  return 0;
}
