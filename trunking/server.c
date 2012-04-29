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

int main(int argc, char ** argv){
	FILE * fd;		// descritor de arquivo				::
	int sockfd;		// descritor do listen socket		::
	int nsockfd;		// descritor do accepter socket	::

	sFila f;		// struct da lista					::

	int yes = 1;
	unsigned short int port;
	int id;
	int op;
	
	fd_set rset;
	socklen_t sin_size;

	struct sockaddr_in host;
	struct sockaddr_in client;

	
	if(argc != 2){
		fprintf(stderr, "\n[%s-%s] Usage: %s <port>\n\n", T_RED, NOTHING, argv[0]);
		exit(-1);
	}
	port = atoi(argv[1]);

	//sock_create(int * sockfd, struct sockaddr_in * host, unsigned short int port)
	startList(&f);
	sin_size = Socket(&sockfd, &host, port);
	while(TRUE){
		Accept(&nsockfd, &sockfd, &client, &sin_size);
		send(nsockfd, "\nWELCOME\n", 10, 0);
		close(nsockfd);
	}
	
}
