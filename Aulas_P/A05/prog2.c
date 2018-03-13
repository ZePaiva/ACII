#include<detpic32.h>
#include<my_funcs.h>

int main()
{
  volatile int aux;
  TRISBbits.TRISB6 = 0;
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG6 = 1;    // digital port
  AD1PCFGbits.PCFG4 = 0;    // analogical port
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON2bits.SMPI = 0;
  AD1CON3bits.SAMC = 16;
  AD1CON1bits.ON = 1;
  while(1)
  {
    LATBbits.LATB6 = 1;
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);
    LATBbits.LATB6 = 0;
    aux = ADC1BUF0;
    IFS1bits.AD1IF = 0;
  }
}

/*
 * tempo ≃ 3.5 microsegundos
 * freq ≃ 28.5 kHz
 */
