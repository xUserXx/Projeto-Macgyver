//*****************************************
// Socket with select multiplex function ::
// by UserX				 ::
//*****************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

#include "../headers/color_rset.h"

#define max(a,b)	((a) > (b) ? (a) : (b))

#define TRUE	1
#define FALSE	0
#define BACKLOG	5

int main(int argc, char ** argv){
	int sockfd;		// socket declarations ::
	int nsockfd;		// the same ::
	int yes;
	int maxfd;
	int stdineof;
	int recv_l;
	int send_l;
	int port;

	char buff_r[256];
	char buff_s[256];

	fd_set rset;
	fd_set allset;

	socklen_t sin_size;

	struct sockaddr_in server;
	struct sockaddr_in client;

	yes = 1;

	memset(buff_r, '\0', sizeof(buff_r));

	if(argc < 2){
		fprintf(stderr, "%susage: %s <port>\n", BLINK,argv[0]);
		exit(1);
	}
	port = atoi(argv[1]);

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("in socket()");
		exit(1);
	}
	printf("%ssocket() is OK...\n", T_WHITE);

	if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) < 0){
		perror("in setsockopt()");
	}
	printf("%ssetsockopt() is OK...\n", T_WHITE);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	memset(&(server.sin_zero), '\0', 8);

	if((bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in))) < 0){
		perror("in bind()");
		exit(1);
	}
	printf("%sbind() is OK...\n", T_WHITE);

	if((listen(sockfd, BACKLOG)) < 0){
		perror("in listen()");
		exit(1);
	}
	printf("%slisten() is OK...\n", T_WHITE);

	sin_size = sizeof(struct sockaddr_in);
	while(TRUE){
		if((nsockfd = accept(sockfd, (struct sockaddr *)&client, &sin_size)) < 0){
			perror("in accept()");
			exit(1);
		}
		printf("accept() is OK...\n");
		stdineof = 0;
		FD_ZERO(&rset);
		FD_SET(nsockfd, &rset);
		maxfd = nsockfd;
		select(maxfd+1, &rset, NULL, NULL, NULL);
		for(;;){
			if(FD_ISSET(nsockfd, &rset)){
				recv_l = recv(nsockfd, buff_r, sizeof(buff_r), 0);
				if(recv_l < 0){
					perror("in recv()");
					continue;
				}
				printf("\n%d\n", recv_l);
				if(recv_l == 0){
//					printf(T_RED);
					printf("\n\t%s-==[!!] Connection was closed from client [!!]==-\n", T_RED);
					break;
				}
				buff_r[(strlen(buff_r)-1)] = '\0';
				printf("%s\n", buff_r);
				memset(buff_r, '\0', sizeof(buff_r));

			}
		}
	}
	return 0;
}
