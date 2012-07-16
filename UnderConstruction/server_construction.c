/***************************************************************************
* File: server_construction.c                                              *
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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/select.h>

#include "../headers/color_rset.h"
#include "../headers/dump_io.h"
#include "../headers/shell_fnct.h"
#include "../headers/exceptions.h"
#include "../headers/sock_ng.h"
#include "../headers/userList.h"

#define MAXLINE		64
#define MINLINE		16

int main(int argc, char ** argv){
	FILE * fd;		// descritor de arquivo				::
	int sockfd;		// descritor do listen socket		::
	int nsockfd;	// descritor do accepter socket		::
	int maxfd;

	sFila f;		// struct da lista					::

	int recv_l;		// tamanho do buffer recebido [DEBUG]	::
	int send_l;		// tamanho do buffer enviado [DEBUG] 	::
	int access;		// controle de acesso [login()]			::
	int cpid;		// processo filho						::
	int _cpid;		// processo neto						::

	unsigned short int port;

	char username[MAXLINE];		// buffer para o user
	char passwd[MINLINE];		// buffer para senha

	char * const sh[] = {"/bin/sh", NULL}; // puxará o shell com close(), dup2() e esceve() ::

	fd_set rset;
	socklen_t sin_size;

	struct sockaddr_in host;
	struct sockaddr_in client;

	port = verify(&argc, argv);

	startList(&f);
	if(fork()){
		sin_size = Socket(&sockfd, &host, port);
		if(fork()){
			while(TRUE){
				Accept(&nsockfd, &sockfd, &client, &sin_size);
				if(fork()){
					FD_ZERO(&rset);
					FD_SET(nsockfd, &rset);
					for(;;){
						maxfd = nsockfd;
						select(maxfd + 1, &rset, NULL, NULL, NULL);
						if(FD_ISSET(nsockfd, &rset)){
							recv_l = recv(nsockfd, username, MAXLINE, 0);
							str_format(username);
							recv_l = recv(nsockfd, passwd, MINLINE, 0);
								str_format(passwd);
							if(recv_l < 0){
								fatal("in recv()");
								}else if(recv_l == 0){
								fprintf(stderr, "\n[%s-%s] Connection interrupted by host...\n", T_RED, NOTHING);
								close(nsockfd);
								break;
								}else{
								access = login(&f, username, passwd);
								printf("%d", access);
								if(access == 1){
									printf("heheh");
									close(0);
									close(1);
									close(2);
									dup2(nsockfd, 0);
									dup2(nsockfd, 1);
									dup2(nsockfd, 2);
									execve(sh[0], sh, NULL);
									close(nsockfd);
								}else{
										printf("\n-==[WARNING] Access Denied [!!]==-\n");
											close(nsockfd);
										break;
								}
							} 
						}
					}
				}
			}
		}
	}
			
	listFree(&f);
	return 0;
}		
