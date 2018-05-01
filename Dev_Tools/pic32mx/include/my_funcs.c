#include<detpic32.h>
#include<my_funcs.h>

unsigned char toBcd(unsigned char value)
{
  return ((value / 10) << 4) + value % 10;
}

void send2displays(unsigned char value, unsigned char dec_point)
{
  static const char display7Scodes[] = { 
    0x3F, 0x06, 0x5B, 0X4F, 0x66, 0x6D, 0X7D, 0X07, 
    0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71
  };

  static char displayFlag = 0;
  unsigned char digit_low = (value & 0x0F);
  unsigned char digit_high = (value & 0xF0) >> 4;
  char hexCode;

  // left display
  if (displayFlag)
  {
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    hexCode = display7Scodes[digit_low];
    LATB = (LATB & 0x00FF) | ((int)hexCode << 8);
  }
  // right display
  else
  {
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    hexCode = display7Scodes[digit_high];
    LATB = (LATB & 0x00FF) | ((int)hexCode << 8);
  
    if(dec_point == 1)
      LATBbits.LATB15 = 1;
  }

  displayFlag = !displayFlag;
}

void delay(int ms)
{
  for (; ms > 0; ms--){
    resetCoreTimer();
    while(readCoreTimer() < PBCLK/1000);
  }
}

void setPWM(unsigned int duty, int K, int sync)
{
  if (duty > 100)
    return;
  if (sync == 1)
    OC1RS = ((K + 1) * duty) / 100;
  else
    OC1RS = (K * duty) / 100;
}
