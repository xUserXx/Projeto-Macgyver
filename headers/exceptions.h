/***************************************************************************
* File: exceptions.h                                                       *
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

//      exceptions.h :: 

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string.h>
#include "color_rset.h"

// Uma função para exibir msg de erro e então sair.
void fatal(char *message) {
		char error_message[100];

		strcpy(error_message, "[!!] Fatal Error ");
		strncat(error_message, message, (100 - strlen(error_message)));
		perror(error_message);
		exit(-1);
}

// Uma Função que server como envólocro do malloc verificadora de erros ::
// ela retorna um ponteiro para o endereço alocado ::
// o malloc retorna retorna void então, é preciso fazer a coerção dos dados ::
// com esta, n, pois ela retorna um ponteiro do mesmo tipo pedido ::
void * ec_malloc(unsigned int size) {
	void *ptr;
	ptr = malloc(size);
	if(ptr == NULL){
		fatal("in ec_malloc() on memory alocation");
	}
	return ptr;
}

// esvazia a memoria raw em bytes hexadecimais e em formato dividido para impressão.
void dump(const unsigned char *data_buffer, const unsigned int length){
  unsigned char byte;
  unsigned int i, j;
  for(i = 0; i < length; i++ ){
    byte = data_buffer[i];
    printf("%02x ", data_buffer[i]); // exibindo o byte em hexadecimal.
    if(((i%16) == 15) || (i == length - 1)){
      for(j = 0; j < 15 -(i%16); j++){
        printf("  ");
      }
      printf("| ");
      for(j = (i - (i%16)); j <= i; j++){ // exibe os bytes imprimíveis na fila.
        byte = data_buffer[j];
        if((byte > 31) && (byte < 127)){ // Extenção do char a ser impresso.
          printf("%c", byte);
        }else{
          printf(".");
        }
      }
      printf("\n");
    }
  }
}

int verify(int * argc, char ** argv){
	int port;
	if(* argc != 2){
		fprintf(stderr, "\n[%s-%s] Usage: %s <port>\n\n", T_RED, NOTHING, argv[0]);
		exit(-1);
	}
	port = atoi(argv[1]);
	if(port > 65535 || port < 1024 || port <= 0){
		fprintf(stderr, "\n[%s-%s] Usage: %s <port>\n", T_RED, NOTHING, argv[0]);
		fprintf(stderr,"[%s-%s] Port Number most to be below then 65535 and higher then 1024 [ %sWARNING!!!%s ]\n", T_RED, NOTHING, T_RED, NOTHING);
		exit(-1);
	}
	return port;
}

#endif
