#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>

int main(){
  int fd;
  ifreq ifr;

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;

  ifr.ifr_hwaddr.sa_data[0] = 0xAA;
  ifr.ifr_hwaddr.sa_data[1] = 0xBB;
  ifr.ifr_hwaddr.sa_data[2] = 0xCC;
  ifr.ifr_hwaddr.sa_data[3] = 0xDD;
  ifr.ifr_hwaddr.sa_data[4] = 0xEE;
  ifr.ifr_hwaddr.sa_data[5] = 0xFF;
  
  strncpy(ifr.ifr_name, "enp0s3", IFNAMSIZ - 1);
  if(ioctl(fd, SIOCSIFHWADDR, &ifr) != 0){
    perror("ioctl");
    return 1;
  }
}
