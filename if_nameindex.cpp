#include <stdio.h>
#include <net/if.h>

int main(){
  struct if_nameindex *idxlist, *idx;

  idxlist = if_nameindex();
  if(idxlist == NULL){
    perror("if_nameindex");
    return 1;
  }

  for(idx = idxlist; idx->if_name != NULL && idx->if_index != 0;++idx){
    printf("%s\t%d\n", idx->if_name, idx->if_index);
  }

  if_freenameindex(idxlist);

  return 0;
}
