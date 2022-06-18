#include <stdio.h>

int main(int argc, char *argv[]){
  int num_of_args = 0; 
  while(argv[num_of_args] != NULL){
    printf("%s\n", argv[num_of_args]);
    num_of_args += 1;
  }
  return 0;
}