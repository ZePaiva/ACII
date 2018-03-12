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
  int i;
  while(1)
  {
    // has low flicker
    /*
    i = 0;
    do{
      delay(20);
      send2displays(val);
    } while (++i < 10);
    val++;
    val %= 256;
    */

    // has no flicker
    i = 0;
    do{
      delay(10);
      send2displays(val, 0);
    } while (++i < 20);
    val++;
    val %= 256;
  }
}
