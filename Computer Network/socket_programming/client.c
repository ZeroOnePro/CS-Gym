#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define SERVER_PORT 3490
#define BUFFER_SIZE 4096

int main(){
  int sock_fd, new_fd;
  struct sockaddr_in their_addr;
  char send_buffer[BUFFER_SIZE], recv_buffer[BUFFER_SIZE];

  if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket creation failure");
    exit(1);
  }

  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(SERVER_PORT);
  their_addr.sin_addr.s_addr = htonl(0);

  if(connect(sock_fd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect failure");
    exit(1);
  }

  printf("Please enter a message > ");
  while (fgets(send_buffer, BUFFER_SIZE, stdin) != NULL) {
	
		send(sock_fd, send_buffer, strlen(send_buffer), 0);
		memset(recv_buffer, 0, BUFFER_SIZE);  // flush buffer to 0
  
		if (recv(sock_fd, recv_buffer, BUFFER_SIZE,0) == 0){
			perror("서버 종료 체크"); 
    }
		
		printf("%s", "from server: ");
		fputs(recv_buffer, stdout);
    printf("Please enter a message > ");
  }

  close(sock_fd);

  return 0;
}