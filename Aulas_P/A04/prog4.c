#include<detpic32.h>

void delay(int ms);

int main()
{
  unsigned char segment;
  LATDbits.LATD6 = 1;
  LATDbits.LATD5 = 0;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  TRISB = TRISB & 0x00FF;

  while(1)
  {
    LATDbits.LATD6 != LATDbits.LATD6;
    LATDbits.LATD5 != LATDbits.LATD5;
    segment = 1;
    int i;
    for(i = 0; i < 7; i++)
    {
      LATB = (LATB & 0x0000) | segment;
      LATB <<= 8;
      delay(1000);
      segment <<= 1;
    }
  }

  return 0;
}

void delay (int ms)
{
  for (; ms > 0; ms--){
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}
