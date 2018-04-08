#include<detpic32.h>
#include<my_funcs.h>

int main(void)
{
  // config as analogical input
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;

  // trigger config for auto conversion
  AD1CON1bits.SSRC = 7;
  
  // if process occurs only when given order
  AD1CON1bits.CLRASAM = 1;
  
  // select input/channel to convert
  AD1CHSbits.CH0SA = 4;
  
  // select amount of consecutive conversions
  AD1CON2bits.SMPI = 0;
  
  // sample time
  AD1CON3bits.SAMC = 16;
  
  // enable converter
  AD1CON1bits.ON = 1;
  
  while(1)
  {
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);
    printInt(ADC1BUF0, 16 | 3 << 16);
    printStr("\n");
    IFS1bits.AD1IF = 0;
    delay(100);
  }

  return 0;
}
