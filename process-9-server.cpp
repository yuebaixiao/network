#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define FILEPATH "/tmp/afunix_text"

int main(){
  int s0, sock;
  sockaddr_un s_un;
  sockaddr_un s_un_accept;
  socklen_t addrlen;

  s0 = socket(AF_UNIX, SOCK_STREAM, 0);
  if(s0 < 0){
    perror("socket");
    return 1;
  }

  s_un.sun_family = AF_UNIX;
  strcpy(s_un.sun_path, FILEPATH);

  if(bind(s0, (sockaddr*)&s_un, sizeof(s_un)) != 0){
    perror("bind");
    return 1;
  }

  if(listen(s0, 5) != 0){
    perror("listen");
    return 1;
  }

  addrlen = sizeof(s_un_accept);
  sock = accept(s0, (sockaddr*)&s_un_accept, &addrlen);
  if(sock < 0){
    perror("accept");
    return 1;
  }

  printf("after accept\n");

  write(sock, "the msg is send from server", 27);
  close(sock);
  close(s0);

  unlink(FILEPATH);

  return 0;
}
