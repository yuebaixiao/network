#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
  ifaddrs *ifa_list;
  ifaddrs *ifa;
  int n;
  char addrstr[256], netmaskstr[256];

  n = getifaddrs(&ifa_list);
  if(n != 0){
    return 1;
  }

  for(ifa = ifa_list; ifa != NULL; ifa = ifa_list->ifa_next){
    printf("ifa_name:%s\n", ifa->ifa_name);
    printf("ifa_flags:  0x%.8x\n", ifa->ifa_flags);

    memset(addrstr, 0, sizeof(addrstr));
    memset(netmaskstr, 0, sizeof(netmaskstr));

    //IPv4
    if(ifa->ifa_addr->sa_family == AF_INET){
      //IP
      inet_ntop(AF_INET,
		&((sockaddr_in*)ifa->ifa_addr)->sin_addr,
		addrstr, sizeof(addrstr));
      //mask
      inet_ntop(AF_INET,
		&((sockaddr_in*)ifa->ifa_netmask)->sin_addr,
		netmaskstr, sizeof(netmaskstr));

      printf("   IPv4: %s netmask : %s\n", addrstr, netmaskstr);
    }
    //IPv6
    else if(ifa->ifa_addr->sa_family == AF_INET6){
      //IP
      inet_ntop(AF_INET6,
		&((sockaddr_in6*)ifa->ifa_addr)->sin6_addr,
		addrstr, sizeof(addrstr));
      //mask
      inet_ntop(AF_INET6,
		&((sockaddr_in6*)ifa->ifa_netmask)->sin6_addr,
		netmaskstr, sizeof(netmaskstr));

      printf("   IPv6: %s netmask : %s\n", addrstr, netmaskstr);
    }
    else if(ifa->ifa_addr->sa_family == AF_PACKET){
      printf("  af_packet:%d\n", ifa->ifa_addr->sa_family);
    }
    else{
      printf("  else:%d\n", ifa->ifa_addr->sa_family);
    }
    printf("\n");
  }
  freeifaddrs(ifa_list);

  return 0;
}
