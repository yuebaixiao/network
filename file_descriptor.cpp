#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(){
  int sock;
  cout << "fileno(stdin): " << fileno(stdin) << endl;
  //close(0);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  cout << sock << endl;
  
  int sock1 = socket(AF_INET, SOCK_DGRAM, 0);
  cout << sock1 << endl;
}
