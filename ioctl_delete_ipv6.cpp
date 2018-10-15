#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

struct in6_ifreq{
  struct in6_addr ifr6_addr;
  u_int32_t ifr6_prefixlen;
  int ifr6_ifindex;
};
int main(){
  int fd;
  in6_ifreq ifr;
  int n;

  fd = socket(AF_INET6, SOCK_DGRAM, 0);

  memset(&ifr, 0, sizeof(ifr));
  inet_pton(AF_INET6, "2003::1", &ifr.ifr6_addr);
  ifr.ifr6_prefixlen = 64;

  ifr.ifr6_ifindex = if_nametoindex("enp0s3");

  n = ioctl(fd, SIOCDIFADDR, &ifr);
  if(n < 0){
    perror("ioctl");
    return 1;
  }

  close(fd);
  return 0;
}
