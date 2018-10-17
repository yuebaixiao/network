#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  int retval;
  pid_t pid = atoi(argv[1]);
  
  retval = kill(pid,SIGUSR1);
            
  if ( retval )
    {
      perror( "kill" );
    }
  else
    {
      printf( "%d killed\n", pid );
    }
}
