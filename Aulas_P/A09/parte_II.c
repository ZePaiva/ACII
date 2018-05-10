#include<detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct {
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
    unsigned int overrun;
} circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;

char comDrv_getc(char *pchar);

void comDrv_flushRx()
{
  rxb.overrun = 0;
  rxb.tail = 0;
  rxb.head = 0;
  rxb.count = 0;
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    rxb.data[i] = 0;
}

void comDrv_flushTx()
{
  txb.overrun = 0;
  txb.tail = 0;
  txb.head = 0;
  txb.count = 0;
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    txb.data[i] = 0;
}

void comDrv_putc(char c)
{
  while (txb.count == BUF_SIZE);
  txb.data[txb.tail] = c;
  txb.tail = (txb.tail + 1) & INDEX_MASK;
  DisableUart1TxInterrupt();
  txb.count++;
  EnableUart1TxInterrupt();
}

void comDrv_puts(char *str)
{
  while((*str) != '\0')
  {
    comDrv_putc(*str);
    (str)++;
  }
}

void _int_(24) isr_UART1()
{
  if(IFS0bits.U1TXIF)
  {
    while (txb.count > 0 && U1STAbits.UTXBF == 0)
    {
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1) & INDEX_MASK;
      txb.count--;
    }
    if (txb.count == 0)
      DisableUart1TxInterrupt();
    IFS0bits.U1TXIF = 0;
  }

  if (IFS0bits.U1RXIF)
  {
    while (rxb.count < BUF_SIZE && U1STAbits.URXDA != 0)
    {
      rxb.data[rxb.tail] = U1RXREG;
      rxb.tail = (rxb.tail + 1) & INDEX_MASK;
      rxb.count++;
      IFS0bits.U1RXIF = 0;
    }
  }

  if (IFS0bits.U1EIF)
  {
    char fg;
    if (U1STAbits.OERR)
    {
      U1STAbits.OERR = 0;
      rxb.overrun = 1;
    }

    if (U1STAbits.FERR || U1STAbits.PERR)
      fg = U1RXREG;

    IFS0bits.U1EIF = 0;
  }
}

void comDrv_config(unsigned int baudrate, char parity, char data, char stopbits)
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
  IFS0bits.U1TXIF = 0;
  IEC0bits.U1EIE = 1;
  IFS0bits.U1EIF = 0;
}

int main()
{
  char buf;
  comDrv_config(115200, 'N', 8, 1);
  comDrv_flushTx();
  comDrv_flushRx();
  EnableInterrupts();
  comDrv_puts("PIC32 UART Device-driver\n");
  while(1)
  {
    if (comDrv_getc(&buf))
    {
      if (buf == 'S')
        comDrv_puts("Uma string com 30 caracteress.");
      comDrv_putc(buf);
    }
  }
  return 0;
}

char comDrv_getc(char *pchar)
{
  if (rxb.count == 0) return 0;
  DisableUart1RxInterrupt();
  *pchar = rxb.data[rxb.head];
  rxb.count--;
  rxb.head = (rxb.head + 1) & INDEX_MASK;
  EnableUart1RxInterrupt();
  return 1;
}
