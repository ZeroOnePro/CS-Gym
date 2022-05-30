#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 3490
#define BACKLOG 10
#define BUFFER_SIZE 4096

int main(void){
  int sock_fd, new_fd;
  struct sockaddr_in my_addr;
  struct sockaddr_in their_addr;
  unsigned int sin_size;

  char buffer[BUFFER_SIZE] = { 0 };
  char* message = "server response ok\n";

  if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket creation failure");
    exit(1);
  }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(PORT);

  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
    perror("bind failure");
    exit(1);
  }

  if(listen(sock_fd, BACKLOG) == -1){
    perror("listen failure");
    exit(1);
  }

  while(1){
    sin_size = sizeof(struct sockaddr_in);
    if((new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
      perror("accept failure");
      continue;
    }
    int len = 0;
    while ((len = recv(new_fd, buffer, BUFFER_SIZE, 0))) {
			printf("%s", "from client: ");
  		fputs(buffer, stdout);
			send(new_fd, message, strlen(message), 0);  //send data back to client
			memset(buffer, 0, BUFFER_SIZE);  // flush buffer to 0
		}
		close(new_fd);
  }

  close(sock_fd);

  return 0;
}