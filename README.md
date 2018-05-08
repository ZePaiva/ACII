# Arquitetura de Computadores II
Algumas resoluções dos exercícios propostos em Arquitetura de Computadores II

### Aulas_P
Resoluções dos exercícios

### Aulas_TP
Suporte teórico da UC

### Dev_Tools
Contém todas as ferramentas para compilação a partir de código c (.c) e assembly (.asm)
Contém o ficheiro my_funcs.c (diretório pic32mx/include/) com:
	
  * funções:
	  + unsigned char toBcd (unsigned char value) -> converte de binário/hexadecimal para decimal
	  + void delay (int n) -> cria um atraso de n milissegundos
	  + void send2displays( unsigned char value, unsigned char dec_point) -> envia os digitos para os displays hexadecimais com a opção de colocar o ponto decimal
    	  + void setPWM( unsgined char duty_cycle, int K, int ssync) -> controla o duty cycle de um       sinal com base no duty dado [0-100], do valor do PR (K) e se o clock é ou não síncrono 
    	  + void putString(char *str) -> reimplementação da função printStr(char *str)
    	  + void putc(char c) -> reimplementação da função putChar(char c);
    	  + char getc() -> reimplementação da função getChar();
	
  * macros:
	  + VOLTAGE(V) obtem o valor da voltagem a partir da média dos valores analógicos

### Guiões
Contém todos os guiões de exercícios fornecidos na UC

### help_compile_functions.txt
Contém algumas funções auxiliares para a correta compilação dos ficheiros com a biblioteca my_funcs

Caso detetem algum erro ou falta de algum ficheiro abram um Issue ou escrevam caso já exista um para tal efeito.
