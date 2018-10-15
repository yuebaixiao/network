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
  s_in->sin_addr.s_addr = inet_addr("10.0.2.15");

  strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ - 1);

  if(ioctl(fd, SIOCSIFADDR, &ifr) != 0){
    perror("ioctl");
    return 1;
  }

  close(fd);
  return 0;
}
