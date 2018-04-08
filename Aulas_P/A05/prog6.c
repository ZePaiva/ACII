#include<detpic32.h>
#include<my_funcs.h>

#define SAMPLES 4

void IOconf(void);

int main()
{
  IOconf();                       // config IO ports
  double sum = 0;                 // sum of all samples
  unsigned char to_send_volt;     // voltage in decimal
  double voltage = 0;             // voltage in hex
  int i;                          // countBuffer var
  int j = 0;                      // refresh rate of voltage var
  int *bp;                        // buffer pointer
  
  while(1)
  {
    delay(10);
    if (++j == 25)
    {
      sum = 0;
  
      // start conversion
      AD1CON1bits.ASAM = 1;

      // polling cycle
      while(IFS1bits.AD1IF == 0);
      
      bp = (int *)(&ADC1BUF0);
      for (i = 0; i < SAMPLES; i++)
        sum += bp[i*4];
      
      sum /= SAMPLES;
      voltage = VOLTAGE(sum);
      
      j = 0;
      IFS1bits.AD1IF = 1;
    }
    
    to_send_volt = toBcd(to_send_volt);
    send2displays(to_send_volt, 1);
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
