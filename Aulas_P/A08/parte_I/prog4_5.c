#include<detpic32.h>
#include<my_funcs.h>

void putc(char byte2send);
void putString(char *str);
void UARTConfig(unsigned int baudrate, char parity, unsigned int stopbits);

int main()
{
  UARTConfig( 115200, 'O', 1);
  while(1)
  {
    putString("AC2 - Arquitetura de Computadores 2\n");
    delay(1000);
  }
  return 0;
}

void UARTConfig(unsigned int baudrate, char parity, unsigned int stopbits)
{
  if (baudrate > 115200 || baudrate < 600)
    baudrate = 115200;

  if (parity != 'N' && parity != 'E' && parity != 'O')
    parity = 'N';

  if (stopbits != 1 && stopbits != 2)
    stopbits = 1;

  U1BRG = (PBCLK + 8 * baudrate) / (16 * baudrate) - 1;
  U1MODEbits.BRGH = 0;

  if (parity == 'N')
    U1MODEbits.PDSEL = 0;
  else if (parity == 'E')
    U1MODEbits.PDSEL = 1;
  else if (parity == 'O')
    U1MODEbits.PDSEL = 2;

  U1MODEbits.STSEL = stopbits - 1;

  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;
  U1MODEbits.ON = 1;
}

void putString(char *str)
{
  int i = 0;
  while (str[i] != '\0')
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
