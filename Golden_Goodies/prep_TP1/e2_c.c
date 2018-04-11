#include<detpic32.h>

void delay(int ms);
void IOconf();
void sendisp(unsigned char value);
unsigned char toBcd(unsigned char value);

volatile int volt= 0;

int main()
{
  IOconf();
  unsigned char sent = 0;
  int i = 0;
  while(1)
  {
    delay(12.5);
    if (++i == 16)
    {
      AD1CON1bits.ASAM = 1;
      i = 0;
    }
    sent = toBcd((unsigned char)volt);
    sendisp(sent);
  }
  return 0;
}

void _int_(27) isr_adc(void)
{
  int *bp = (int *)(&ADC1BUF0);
  printStr("NOME: Jose Miguel Pinho Paiva NMEC:84972\n");
  volt = (*bp * 70 + 511) / 1023;
  IFS1bits.AD1IF = 0;
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
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD6 = 1;
  LATDbits.LATD5 = 1;
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

  IPC6bits.AD1IP = 2;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;
  EnableInterrupts();
}

void sendisp(unsigned char value)
{
  static const char display7Scodes[] = { 
    0x3F, 0x06, 0x5B, 0X4F, 0x66, 0x6D, 0X7D, 0X07, 
    0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71
  };

  static char displayFlag;
  int high = (value & 0xF0) >> 4;
  int low = value & 0x0F;
  if (displayFlag)
  {
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;
    PORTB = (PORTB & 0x00FF) | ((int)display7Scodes[low] << 8);
  }
  else
  {
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    PORTB = (PORTB & 0x00FF) | ((int)display7Scodes[high] << 8);
  }
  displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
  return ((value / 10) << 4) + value % 10;
}
