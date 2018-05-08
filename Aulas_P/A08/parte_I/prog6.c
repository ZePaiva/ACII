#include<detpic32.h>
#include<my_funcs.h>

void IOConfig();

int main()
{
  UART1Config(115200, 'N', 8, 1);
  IOConfig();
  while(1)
  {
    while (U1STAbits.TRMT == 0);
    LATBbits.LATB6 = 1;
    putString("123456789AB");
    LATBbits.LATB6 = 0;
  }

  return 0;
}

void IOConfig()
{
  TRISBbits.TRISB6 = 0;
}


/*
 * String -> 12345        | tempo ~= 3   microssegundos
 * String -> 123456789    | tempo ~= 5   microssegundos 
 * String -> 123456789A   | tempo ~= 88  microssegundos
 * String -> 123456789AB  | tempo ~= 170 microssegundos
 */
