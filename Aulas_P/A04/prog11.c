#include<detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);

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

  while(1)
  {
    if (val > 0xFF)
      val = 0;
    delay(200);
    send2displays(val);
    val++;
  }
}

void send2displays(unsigned char value)
{
  static const char display7Scodes[] = { 
    0x3F, 0x06, 0x5B, 0X4F, 0x66, 0x6D, 0X7D, 0X07, 
    0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71
  };
  
  unsigned char digit_low = (value & 0x0F);
  unsigned char digit_high = (value & 0xF0) >> 4;
  char hexCode;

  // high part first
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;

  hexCode = display7Scodes[digit_high];
  LATB = (LATB & 0x0000) | hexCode;
  LATB <<= 8;
  delay(10);
  
  // low part second
  LATDbits.LATD5 = 1;
  LATDbits.LATD6 = 0;

  hexCode = display7Scodes[digit_low];
  LATB = (LATB & 0x0000) | hexCode;
  LATB <<= 8;
  delay(10);
  
}

void delay(int ms)
{
  for (; ms > 0; ms--){
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}
