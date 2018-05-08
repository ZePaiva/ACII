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

void UART1Config(unsigned int baudrate, char parity, char data, char stopbits)
{
  if (baudrate > 115200 || baudrate < 600 ) baudrate = 115200;

  if (data != 8 && data != 9) data = 8;

  if (parity != 'N' && parity != 'E' && parity != 'O') parity = 'N';

  if (stopbits != 1 && stopbits != 2) stopbits = 1;

  U1BRG = (PBCLK + 8 * baudrate) / ( 16 * baudrate) - 1;
  U1MODEbits.BRGH = 0;

  if (data == 9) U1MODEbits.PDSEL = 3;
  else
  {
    if (parity == 'N') U1MODEbits.PDSEL = 0;
    else if (parity == 'E') U1MODEbits.PDSEL = 1;
    else U1MODEbits.PDSEL = 2;
  }

  U1MODEbits.STSEL = stopbits - 1;

  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;

  U1MODEbits.ON = 1;
  
  IEC0bits.U1RXIE = 1;
  IPC6bits.U1IP = 2;
  IFS0bits.U1RXIF = 0;
}

void putString(char *strin)
{
  int i = 0;
  while (strin[i] != '\0')
  {
    putc(strin[i++]);
  }
}

void putc(char toSend)
{
  while(U1STAbits.UTXBF == 1);
  U1TXREG = toSend;
}

char getc()
{
  if (U1STAbits.OERR == 1) U1STAbits.OERR = 0;
  while(U1STAbits.URXDA == 0);
  
  char toDisc;
  if (U1STAbits.FERR || U1STAbits.PERR)
  {
    toDisc = U1RXREG;
    return 0;
  }

  return U1RXREG;
}
