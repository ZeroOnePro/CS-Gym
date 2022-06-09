#include <unistd.h>
#include <stdio.h>

int main(void){
	pid_t pid;
	pid = fork();
  char *some_args[] = {"hello", "world", NULL};
	if(pid == 0) {
    printf("\nHello, I am child!\n");
    // bin에 있는 명령어들의 경우 이미 환경변수로 등록되있으므로 p계열을 쓰면 됨
    // execlp("/bin/date", "/bin/date", (char *)0);

    // 인자를 char *[]로 배열로 한번에 넘겨줌
    execv("./sample", some_args);
  }
	else if(pid > 0) {
    printf("\npid: %d\n", pid);
  }
	return 0;	
}