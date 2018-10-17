#include <stdio.h>
#include <unistd.h>//sleep function
#include <pthread.h>

int global_val = 0;

void* sub_thread(void *data){
  int* val = (int*)data;

  printf("sub_thread : val=%d\n", *val);

  for(int i = 0; i < 10; ++i){
    global_val++;
    printf("sub_thread : i=%d, g=%d\n", i, global_val);
    sleep(1);
  }
  return NULL;
}

int main(){
  pthread_t th;
  void* th_ret;
  int arg = 200;
  
  if(pthread_create(&th, NULL, sub_thread, (void*)&arg) != 0){
    perror("pthread_create");
    return 1;
  }

  for(int i = 0; i < 10; ++i){
    global_val++;
    printf("main: i=%d, g=%d\n", i, global_val);
    sleep(1);
  }

  if(pthread_join(th, &th_ret) != 0){
    perror("pthread_join");
    return 1;
  }

  return 0;
}
