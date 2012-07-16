/***************************************************************************
* File: dump_io.h                                                          *
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

// dump_io.h file ::


#ifndef DUMP_IO_H
#define DUMP_IO_H

/* Função dump_line
*  This function reads and dumps any remaining characters on the current input
*  line of a file.
*/

#include <string.h>
#include <termios.h>
#include <unistd.h>
void dump_line( FILE * fp ){
  int ch;

  while( (ch = fgetc(fp)) != EOF && ch != '\n' )
    /* sem corpo */;
  //printf("\n");
}

void str_format( char *str ){
	int i  = strlen(str);

	str[i] = '\0';
}
 
int getch( ) {
  struct termios oldt;
  struct termios newt;
  int	ch;

  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}

#endif
