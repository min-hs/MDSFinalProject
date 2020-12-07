#include "../inc/socket_server.h"

int listen_sock, cli_sock;
struct sockaddr_in servaddr, cliaddr;
socklen_t addrlen;
int res;

void server_init(char* port) {
	int res = 0;
	
	listen_sock = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM ->TCP/IP
	if(listen_sock < 0) { 
		perror("socket()"); 
		exit(1); 
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(0);
	servaddr.sin_port = htons(atoi(port));

	res = bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(res < 0) { 
		perror("bind()"); 
		exit(1); 
	}

	res = listen(listen_sock, 1);
	if(res < 0) { 
		perror("listen()"); 
		exit(1); 
	}
		
	printf("L298N control server running...\n");
}

void server_close() {	
	close(listen_sock);
}

void server_accept() {
	addrlen = sizeof(cliaddr);
	cli_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
	if(cli_sock < 0) { 
		perror("accept()"); 
		exit(1);
	}
	printf("L298N control client connected!    (%s/%d)\n",
		inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
}

ssize_t server_receive(char* msg) {
	return recv(cli_sock, msg, sizeof(char) * 3, MSG_WAITALL);
}

void client_close() {	
	printf("L298N control client disconnected! (%s/%d)\n",
		inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	close(cli_sock);
}