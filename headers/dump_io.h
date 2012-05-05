//***************************
// dump_io.h file by UserX ::
//***************************

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
	int i  = strlen(str) - 1;

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
