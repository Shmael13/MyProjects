#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(){
  while(1){
    write(1, "hii\n", 4);
    fsync(1);
    fork();

  }

}
