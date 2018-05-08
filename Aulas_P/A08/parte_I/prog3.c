#include<detpic32.h>
#include<my_funcs.h>

void puts(char *str);
void putc(char byte2send);
void UARTConfig();

int main()
{
  UARTConfig();
  while(1)
  {
    puts("AC2 - Arquitetura de computadores 2\n");
    delay(1000);
  }
  return 0;
}

void UARTConfig()
{
  // Configure baudrate generator
  U1BRG = (PBCLK + 8 * 115200) / (16 * 115200) - 1;
  
  // data bits / parity / stop bits
  U1MODEbits.BRGH = 0;   // Divisão por 16
  U1MODEbits.PDSEL = 0;  // 8 bits sem paridade
  U1MODEbits.STSEL = 0;  // 1 stop bit
  
  // enable ST & SR
  U1STAbits.UTXEN = 1;  // Transmitter enabled
  U1STAbits.URXEN = 1;  // Receiver Enabled
  
  // enable UART
  U1MODEbits.ON = 1;
}

void puts(char *str)
{
  int i = 0;
  while(str[i] != '\0')
  {
    putc(str[i]);
    i++;
  }
}

void putc(char byte2send)
{
  while(U1STAbits.UTXBF == 1);
  U1TXREG = byte2send;
}
