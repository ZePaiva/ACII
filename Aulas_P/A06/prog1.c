#include<detpic32.h>
#include<my_funcs.h>

#define SAMPLES 1

void IOConfig(void);

int main(void)
{
  IOConfig();
  AD1CON1bits.ASAM = 1;
  while(1){}
  return 0;
}

void IOConfig(void)
{
  // A/D configs
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
  printStr("\nBuffers:\n");
  int *bp = (int *)(&ADC1BUF0);
  printInt(*bp, 16 | 3 << 16);
  printStr("\n");

  // reset conversion
  AD1CON1bits.ASAM = 1;
  IFS1bits.AD1IF = 0;
}
