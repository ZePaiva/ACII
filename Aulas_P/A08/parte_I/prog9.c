#include<detpic32.h>
#include<my_funcs.h>

char this_getc();

int main()
{
  UART1Config(115200, 'N', 8, 1);
  while(1)
  {
    putc(this_getc());
  }
}

char this_getc()
{
  if (U1STAbits.OERR == 1) U1STAbits.OERR = 0;
  while(U1STAbits.URXDA == 0);
  
  char toDisc;
  if (U1STAbits.FERR || U1STAbits.PERR)
  {
    toDisc = U1RXREG;
    return 0;
  }

  return U1RXREG;
}
