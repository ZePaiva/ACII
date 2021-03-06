#include<detpic32.h>
#include<my_funcs.h>
#include"i2c.h"

#define SAMPLES 8

void IOConf(void);
int getTemp(int *tmp);

volatile unsigned char voltage = 0x00;
volatile unsigned int temperature = 0;
volatile char can_get = 0;
volatile char show = 0;
int K1 = 19530;

int main()
{
  IOConf();
  int duty, val;
  i2c_init(TC74_CLK_FREQ);
  while(1)
  {
    val = PORTB & 0x0003;
    show = (char)(val == 0x0003);
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

      case 2:
        IEC0bits.T1IE = 1;
        duty = voltage * 3;
        setPWM(duty, K1, 1);
        break;

      case 3:
        IEC0bits.T1IE = 1;
        duty = voltage * 3;
        if (can_get)
        {
          getTemp(&temperature);
          printStr("Temperatura -> ");
          printInt10(temperature);
          putChar('\n');
          can_get = 0;
        }
        setPWM(duty, K1, 1);
    }
  }
  return 0;
}

int getTemp(int *tmp)
{
  int ack;
  i2c_start();
  ack = i2c_send(ADDR_WR);
  ack += i2c_send(RTR);
  i2c_start();
  ack += i2c_send(ADDR_RD);
  if (ack == 0)
    *tmp = (int)i2c_receive(I2C_NACK);
  else
    i2c_stop();
  return ack;
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
  IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3()
{
  if (show)
  {
    unsigned char tmp = toBcd((unsigned char)temperature); 
    send2displays(tmp, 0);
  }
  else
    send2displays(voltage, 1);
  IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1()
{
  AD1CON1bits.ASAM = 1;
  can_get = 1;
  IFS0bits.T1IF = 0;
}
