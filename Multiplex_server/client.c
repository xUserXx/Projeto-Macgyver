#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "../headers/dump_io.h"

#define TRUE 	1
#define FALSE 	0
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define MAXLINE 1024


void multiplex(FILE * fd, int sockfd){
	int maxfd;
	int stdineof;
	int recv_l;
	int send_l;

	fd_set rset;

	char sendline[MAXLINE];
	char recvline[MAXLINE];
	
	memset(sendline, '\0', MAXLINE);
	memset(recvline, '\0', MAXLINE);
	stdineof = 0;
	FD_ZERO(&rset);
	for(;;){
		if(stdineof == 0){
			FD_SET(fileno(fd), &rset);
		}
		FD_SET(sockfd, &rset);
		if(fileno(fd) > sockfd){
			maxfd = fileno(fd) + 1;
		}else if(sockfd > fileno(fd)){
			maxfd = sockfd + 1;
		}else{
			maxfd = sockfd + 1;
		}
		select(maxfd, &rset, NULL, NULL, NULL);

		if(FD_ISSET(sockfd, &rset)){
			recv_l = recv(sockfd, recvline, sizeof(recvline), 0);
			if(recv_l < 0){
				perror("in recv()");
				exit(1);
			}else if(recv_l == 0){
				fprintf(stdin, "\nConnection closed by remote host\n");
				exit(1);
			}else{
				printf("\n%s", recvline);
			}
			memset(recvline, '\0', MAXLINE);
		}
		if(FD_ISSET(fileno(fd), &rset)){
			fgets(sendline, sizeof(sendline), stdin);
			str_format(sendline);
			if(!strcmp(sendline, "exit")){
				FD_CLR(sockfd, &rset);
				close(sockfd);
				return;
			}
			send_l = send(sockfd, sendline, strlen(sendline), 0);
			memset(sendline, '\0', MAXLINE);
		}
	}
}

int main(int argc, char ** argv){
	int sockfd;
	int conector;
	int port;

	char buff[512];

	struct sockaddr_in server;
	struct hostent * he;

	if(argc != 3){
		fprintf(stderr, "Usage: %s <hostname> <portnumber>\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[2]);			// casting char * para int :: Usarei para porta do socket ::

	sockfd = socket(PF_INET, SOCK_STREAM, 0);	// criando o socket ::
	if(sockfd < 0){
		perror("in socket()");
		exit(1);
	}
	printf("socket() is OK...\n");

	he = gethostbyname(argv[1]);		// pegando o ip do host servidor atravéz do nome ::

	server.sin_family = he->h_addrtype;			// configurando socket para conexão ::
	server.sin_port = htons(port);
	server.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(server.sin_zero), '\0', 8);

	conector = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if(conector < 0){
		perror("in connect()");
		exit(1);
	}
	printf("connect() is OK...\n");

	multiplex(stdin, sockfd);
	return 0;
}
