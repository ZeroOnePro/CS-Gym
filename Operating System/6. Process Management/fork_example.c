#include <unistd.h>
#include <stdio.h>

int main(void){
	pid_t pid;
	pid = fork();
	if(pid == 0) {
    printf("\nHello, I am child!\n");
    // sleep(10);
    // printf("\nHello, I am child!\n");
  }
	else if(pid > 0) {
    printf("\npid: %d\n", pid);
  }
	return 0;	
}