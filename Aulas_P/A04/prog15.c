#include<detpic32.h>
#include<my_funcs.h>

int main(void)
{
  //B0-3 in
  TRISB = TRISB | 0x000F;
  PORTB = PORTB & 0XFFF0;

  //B8-14 out
  TRISB = TRISB & 0x00FF;
  LATB = LATB & 0x00FF;

  //D5-6 out
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;

  unsigned char val = 0;
  unsigned char decVal;
  int i;
  while(1)
  {
    i = 0;
    do{
      delay(10);
      decVal = toBcd(val);
      send2displays(decVal, 0);
    } while (++i < 100);
    val++;
    val %= 60;
  }

  return 0;
}
