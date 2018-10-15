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

  strncpy(ifr.ifr_name, "lo", IFNAMSIZ - 1);
  strncpy(ifr.ifr_newname, "newloname", IFNAMSIZ - 1);
  if(ioctl(fd, SIOCSIFNAME, &ifr) != 0){
    perror("ioctl");
    return 1;
  }
  close(fd);
  return 0;
}
