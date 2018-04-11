#include<detpic32.h>

void delay(int ms);
void IOconf();

int main()
{
  IOconf();
  char display7Scodes[] = { 
    0x3F, 0x06, 0x5B, 0X4F, 0x66, 0x6D, 0X7D, 0X07, 
    0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71
  };

  int volt;
  char hexCode = display7Scodes[0];
  int *bp = (int *)(&ADC1BUF0);
  while(1)
  {
    delay(100);
    printStr("NOME: Jose Miguel Pinho Paiva NMEC:84972\n");
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);
    volt = (*bp * 15 + 511) / 1023;
    hexCode = display7Scodes[volt];
    PORTB = (PORTB & 0x00FF) | ((int)hexCode << 8);
    IFS1bits.AD1IF = 1;
  }
  return 0;
}

void delay(int ms)
{
  for(; ms > 0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

void IOconf()
{
  TRISDbits.TRISD5 = 1;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD6 = 1;
  LATB = LATB & 0X00FF;
  TRISB = TRISB & 0x00FF;
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CON2bits.SMPI = 0;
  AD1CON3bits.SAMC = 16;
  AD1CHSbits.CH0SA = 4;
  AD1CON1bits.ON = 1;
}
