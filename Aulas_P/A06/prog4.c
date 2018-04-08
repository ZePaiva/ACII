#include<detpic32.h>
#include<my_funcs.h>

#define SAMPLES 8

void IOConfig(void);

volatile unsigned char voltage = 0;

int main(void)
{
  IOConfig();
  int i = 0;
  while(1){
    delay(10);
    if(++i == 25)
    {
      // start conversion
      AD1CON1bits.ASAM = 1;
      i = 0;
    }
    send2displays(voltage, 1);
  }
  return 0;
}

void IOConfig(void)
{
  // A/D configs
  TRISB = TRISB & 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON2bits.SMPI = SAMPLES-1;
  AD1CON3bits.SAMC = 16;
  AD1CON1bits.ON = 1;
  
  // ISR configs
  IPC6bits.AD1IP = 2; // A/D interrupts priority 
  IEC1bits.AD1IE = 1; // A/D interrupts enabled
  IFS1bits.AD1IF = 0; // clear A/D flag

  EnableInterrupts();
}

void _int_(27) isr_adc()
{
  double buf_avg = 0;
  int *bp = (int *)(&ADC1BUF0);
  int i;
  for(i = 0; i < SAMPLES; i++)
  {
    buf_avg += bp[i*4];
  }
  buf_avg /= SAMPLES;
  buf_avg = VOLTAGE(buf_avg);
  buf_avg *= 10;
  voltage = toBcd((char)buf_avg);

  IFS1bits.AD1IF = 0;
}
