#include<detpic32.h>

// 1
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

// 2
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE-1)

typedef struct {
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;

char comDrv_getc(char *pchar);

// 3
void comDrv_flushRx()
{
  rxb.tail = 0;
  rxb.head = 0;
  rxb.count = 0;
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    rxb.data[i] = 0;
}

void comDrv_flushTx()
{
  txb.tail = 0;
  txb.head = 0;
  txb.count = 0;
  int i;
  for (i = 0; i < BUF_SIZE; i++)
    txb.data[i] = 0;
}

// 4
void comDrv_putc(char c)
{
  while (txb.count == BUF_SIZE);
  txb.data[txb.tail] = c;
  txb.tail = (txb.tail + 1) & INDEX_MASK;
  DisableUart1TxInterrupt();
  txb.count++;
  EnableUart1TxInterrupt();
}

// 5
void comDrv_puts(char *str)
{
  while((*str) != '\0')
  {
    comDrv_putc(*str);
    (str)++;
  }
}

// 6 & 9
void _int_(24) isr_UART1()
{
  // 6
  if(IFS0bits.U1TXIF)
  {
    if (txb.count > 0)
    {
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1) & INDEX_MASK;
      txb.count--;
    }
    if (txb.count == 0)
      DisableUart1TxInterrupt();
    IFS0bits.U1TXIF = 0;
  }

  // 9
  if (IFS0bits.U1RXIF)
  {
    rxb.data[rxb.tail] = U1RXREG;
    rxb.tail = (rxb.tail + 1) & INDEX_MASK;
    if (rxb.count < BUF_SIZE)
      rxb.count++;
    else
      rxb.head = (rxb.head + 1) & INDEX_MASK;
    IFS0bits.U1RXIF = 0;
  }
}

// 7
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
}

// 7 & 10
int main()
{
  comDrv_config(115200, 'N', 8, 1);
  comDrv_flushTx();
  comDrv_flushRx();
  EnableInterrupts();
  
  // 10
  char buf;
  comDrv_puts("PIC32 UART Device-driver\n");
  while(1)
  {
    /*
     * 7
     * comDrv_puts("Teste do bloco de transmissao do device driver!...\n");
     */
  
    // 10
    if (comDrv_getc(&buf))
    {
      // 11
      if (buf == 'S')
        comDrv_puts("Uma string com 30 caracteress.");
      comDrv_putc(buf);
    }
  }
  return 0;
}

// 8
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
