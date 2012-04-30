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

	sFila f;		// struct da lista					::

	int yes = 1;
	int id;
	int T;

	unsigned short int port;
	
	char username[MAXLINE];
	char passwd[MINLINE];

	fd_set rset;
	socklen_t sin_size;

	struct sockaddr_in host;
	struct sockaddr_in client;

	port = verify(&argc, argv);

	startList(&f);

	sin_size = Socket(&sockfd, &host, port);
	while(TRUE){
		Accept(&nsockfd, &sockfd, &client, &sin_size);
		send(nsockfd, "\nWELCOME\n", 10, 0);
		recv(nsockfd, username, sizeof(username), 0);
		recv(nsockfd, passwd, sizeof(passwd), 0);
		str_format(username);
		str_format(passwd);
		T = menu(&f, TRUE, username, passwd);
		if(T == FALSE){
			//fprintf(stderr,"\nUser Doesn't exist\n");
			send(nsockfd, "\nUser Doesn't exist\n", 20, 0);
			close(nsockfd);
		}else{
			//printf("\nuser and password does match\n");
			send(nsockfd, "\nuser and password does match\n", 31, 0);
			close(nsockfd);
		}
		close(nsockfd);
	}
	listFree(&f);
	return 0;
}
