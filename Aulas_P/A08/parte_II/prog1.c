#include<detpic32.h>
#include<my_funcs.h>

int main()
{
  UART1Config(115200, 'N', 8, 1);
  EnableInterrupts();
  while(1);
  return 0;
}

void _int_(24) isr_UART1()
{
  putc(U1RXREG);
  IFS0bits.U1RXIF = 0;
}
