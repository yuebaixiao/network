#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>//write

using namespace std;

int main(){
  int sock;
  sock = socket(AF_INET, 4000, 2000);
  //write(-1, "aaa", 4);
  if(sock < 0){
    close(fileno(stdout));
    printf("%d\n", errno);
    perror("create socket");
    cout << errno << endl;
  }
}
