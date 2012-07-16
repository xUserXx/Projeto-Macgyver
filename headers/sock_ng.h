/***************************************************************************
* File: sockng.h                                                           *
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef SOCK_NG_H
#define SOCKNG_H

#define BACKLOG 5
#define TRUE    1

int Socket(int * sockfd, struct sockaddr_in * host, unsigned short int port){
	*sockfd = socket(PF_INET, SOCK_STREAM, 0);
	int yes = 1;
	if(sockfd < 0){
		fatal("Creating a socket");
	}
	printf("[%s+%s] socket() is OK...\n", T_BLUE, NOTHING);	
	
	if(setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0){
		fatal("on sock_create() calling setsockopt()");
	}
	printf("[%s+%s] setsockopt() is OK...\n", T_BLUE, NOTHING);		
	
	host->sin_family = AF_INET;
	host->sin_port = htons(port);
	host->sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(host->sin_zero), '\0', 8);
		
	if(bind(*sockfd, (struct sockaddr *)host, sizeof(struct sockaddr_in)) < 0){
		fatal("binding");
	}
	printf("[%s+%s] bind() is OK...\n", T_BLUE, NOTHING);	
	
	if(listen(*sockfd, BACKLOG)){
		fatal("Listening from socket");
	}
	printf("[%s+%s] listen() is OK...\n", T_BLUE, NOTHING);	
	return sizeof(struct sockaddr_in);
}

void Accept(int * nsockfd, int * sockfd, struct sockaddr_in * client, socklen_t * sin_size){
	*nsockfd = accept(*sockfd, (struct sockaddr *)client, sin_size);
	if(*nsockfd < 0){
		fatal("in accept()");
	}
	printf("[%s+%s] Accept() is OK...\n", T_BLUE, NOTHING);
}

#endif
