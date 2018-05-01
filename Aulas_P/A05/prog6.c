#include<detpic32.h>
#include<my_funcs.h>

#define SAMPLES 4

void IOconf(void);
void send2displays_this(unsigned char value, unsigned char dec_flag);

int main()
{
  IOconf();
  double sum = 0;
  unsigned char to_send_volt;
  double voltage = 0;
  int i;
  int j = 0;
  int *bp;
  char decimal_flag = 0;
  
  while(1)
  {
    delay(10);
    if (++j == 25)
    {
      sum = 0;
      AD1CON1bits.ASAM = 1;
      while(IFS1bits.AD1IF == 0);
      bp = (int *)(&ADC1BUF0);
      for (i = 0; i < SAMPLES; i++)
        sum += bp[i*4];
      sum /= SAMPLES;
      voltage = VOLTAGE(sum);
      voltage /= 10;
      j = 0;
      IFS1bits.AD1IF = 1;
    }
    
    if (voltage < 10)
    {
      to_send_volt = (char)(voltage * 10);
      decimal_flag = 1;
    }
    else
    {
      to_send_volt = (char)(voltage);
      decimal_flag = 0;
    }
  
    to_send_volt = toBcd(to_send_volt);
    send2displays_this(to_send_volt, decimal_flag);
  }
  
  return 0;
}

void IOconf()
{
  // config ADC
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON2bits.SMPI = SAMPLES-1;
  AD1CON3bits.SAMC = 16;
  AD1CON1bits.ON = 1;

  // config digital IO
  TRISB = TRISB & 0x00FF;
  LATB = LATB & 0x00FF;

  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;
}

void send2displays_this(unsigned char value, unsigned char dec_flag)
{
  static const char display7Scodes[] = { 
    0x3F, 0x06, 0x5B, 0X4F, 0x66, 0x6D, 0X7D, 0X07, 
    0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71
  };

  static char displayFlag = 0;
  unsigned char digit_low = (value & 0x0F);
  unsigned char digit_high = (value & 0xF0) >> 4;
  char hexCode;

  if (displayFlag)
  {
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    hexCode = display7Scodes[digit_low];
    LATB = (LATB & 0x00FF) | ((int)hexCode << 8);
  }
  else
  {
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;

    hexCode = display7Scodes[digit_high];
    LATB = (LATB & 0x00FF) | ((int)hexCode << 8);
    
    if (dec_flag == 1)
      LATBbits.LATB15 = 1;
  }

  displayFlag = !displayFlag;
}
