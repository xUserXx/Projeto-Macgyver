//***************************
// dump_io.h file by UserX ::
//***************************

/* Função dump_line
*  This function reads and dumps any remaining characters on the current input
*  line of a file.
*/

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
