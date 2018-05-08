#include<detpic32.h>
#include<my_funcs.h>

#define SAMPLES 8

void IOConf(void);

volatile unsigned char voltage = 0x00;
volatile unsigned char voltMin = 0x33;
volatile unsigned char voltMax = 0x00;
volatile int i = 0;
int K1 = 19530;

int main()
{
  UART1Config(115200, 'N', 8, 1);
  IOConf();
  int duty, val;
  while(1)
  {
    val = PORTB & 0x00003;
    switch(val)
    {
      case 0:
        IEC0bits.T1IE = 1;
        setPWM(0, K1, 1);
        break;
      
      case 1:
        IEC0bits.T1IE = 1;
        setPWM(100, K1, 1);
        break;

      default:
        IEC0bits.T1IE = 1;
        duty = voltage * 3;
        setPWM(duty, K1, 1);
        break;
    }
  }
  return 0;
}

void IOConf()
{
  // hexDisp configs
  LATB = LATB & 0x00FF;
  TRISB = TRISB & 0x00FF;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // ADC
  TRISBbits.TRISB4 = 1;
  AD1PCFGbits.PCFG4 = 0;
  AD1CON1bits.SSRC = 7;
  AD1CON1bits.CLRASAM = 1;
  AD1CHSbits.CH0SA = 4;
  AD1CON2bits.SMPI = SAMPLES-1;
  AD1CON3bits.SAMC = 16;
  AD1CON1bits.ON = 1;
  IPC6bits.AD1IP = 2;
  IEC1bits.AD1IE = 1;
  IFS1bits.AD1IF = 0;

  // Timer 1 configs
  T1CONbits.TCKPS = 7;
  PR1 = K1;
  TMR1 = 0;
  T1CONbits.TON = 1;
  IPC1bits.T1IP = 2;
  IFS0bits.T1IF = 0;

  // Timer 3 configs
  T3CONbits.TCKPS = 2;
  PR3 = 49999;
  TMR3 = 0;
  T3CONbits.TON = 1;
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;

  // PWM configs
  OC1CONbits.OCM = 6;
  OC1CONbits.OCTSEL = 1;
  OC1CONbits.ON = 1;

  EnableInterrupts();
}

void _int_(27) isr_adc()
{
  int buf_avg = 0;
  int *bp = (int *)(&ADC1BUF0);
  int i = 0;
  for (; i < SAMPLES; i++)
    buf_avg += bp[i*4];
  buf_avg /= SAMPLES;
  buf_avg = VOLTAGE(buf_avg);
  voltage = toBcd((char)buf_avg);
  
  if (voltage > voltMax)
    voltMax = voltage;
  else if (voltage < voltMin)
    voltMin = voltage;
  
  IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3()
{
  send2displays(voltage, 1);
  if (i++ == 100)
  {
    putString("Read Voltage: ");
    char up = 0x30 | ((voltage & 0xF0) >> 4);
    putc(up);
    putc('.');
    char down = 0x30 | (voltage & 0x0F);
    putc(down);
    putc('\n');
    i = 0;
  }
  IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1()
{
  AD1CON1bits.ASAM = 1;
  IFS0bits.T1IF = 0;
}

void _int_(24) isr_UART1()
{
  char c = 0;
  if (IFS0bits.U1EIF == 1)
  {
    if (U1STAbits.OERR == 1)
    {
      U1STAbits.OERR = 0;
    }
    else
    {
      c = U1RXREG;
    }
    IFS0bits.U1EIF = 0;
  }
  
  if (U1RXREG == 'L')
  {
    putString("\n\nPeaks:\n");
    
    // send voltMin
    putString("Minimum registered voltage: ");
    char up = 0x30 | ((voltMin & 0xF0) >> 4);
    putc(up);
    putc('.');
    char down = 0x30 | (voltMin & 0x0F);
    putc(down);
    putc('\n');

    // send voltMax
    putString("Maximum registered voltage: ");
    up = 0x30 | ((voltMax & 0xF0) >> 4);
    putc(up);
    putc('.');
    down = 0x30 | (voltMax & 0x0F);
    putc(down);
    putc('\n');
    putc('\n');
    putc('\n');
    putc('\n');
  }
  IFS0bits.U1RXIF = 0;
}
