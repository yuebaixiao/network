#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int main(){
  int fd;
  ifreq ifr;
  sockaddr_in *s_in;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  s_in = (sockaddr_in*)&ifr.ifr_addr;

  s_in->sin_family = AF_INET;
  inet_pton(AF_INET, "255.0.0.0", &s_in->sin_addr);

  strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ - 1);

  if(ioctl(fd, SIOCSIFNETMASK, &ifr) != 0){
    perror("ioctl");
    return 1;
  }
  close(fd);
  return 0;
}
