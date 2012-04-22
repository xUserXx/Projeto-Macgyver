//*********************************
//      exceptions.h :: By UserX ::
//*********************************

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
