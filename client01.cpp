#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
  sockaddr_in server;
  int sock;
  char buf[3];
  int n;

  //创建socket
  sock = socket(AF_INET, SOCK_STREAM, 0);

  //设定要连接哪个ip地址和端口号
}
