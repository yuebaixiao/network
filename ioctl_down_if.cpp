#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

int main(){
  int fd;
  ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ - 1);

  //get current status
  if(ioctl(fd, SIOCGIFFLAGS, &ifr) != 0){
    perror("ioctl");
    return 1;
  }

  //let net work down
  ifr.ifr_flags &= ~IFF_UP;
  //change status
  if(ioctl(fd, SIOCSIFFLAGS, &ifr) != 0){
    perror("ioctl");
    return 1;
  }
  close(fd);
  return 0;
}
