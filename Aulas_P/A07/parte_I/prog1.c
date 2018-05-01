#include<detpic32.h>
#include<my_funcs.h>

void IOConf(void);

int main()
{
  IOConf();
  while(1)
  {
    while (IFS0bits.T3IF == 0);
    putChar('.');
    IFS0bits.T3IF = 0;

  }
}

void IOConf()
{
  TRISDbits.TRISD5 = 0;
  // Timer General Configs 
  /*
   * T2CONbits.TCKPS = 5;   // TxCONbits.TKPCS = n (prescaler: [1:2^n])
   * PR2 = 62499;           // PRx = n (Fout = PBCLK / (prescaler_max * (n + 1)))
   * TMR2 = 0;              // reset Tx count register
   * T2CONbits.TON = 1;     // enable timer Tx
   * OC1CONbits.OCM = 6     // PWM mode on OCy
   * OC1CONbits.OCTSEL = 0; // Use timer x for time base PWM generation
   * OC1RS = 12500;         // Ton constant
   * OC1CONbits.ON = 1;     // Enable OCy module
   */

  // Timer Interruption Configs
  /*
   * IPC2bits.T2IP = 2; // Tx Interrupt priority (ranges [1:6])
   * IEC0bits.T2IE = 1; // Tx Interrupt enable
   * IFS0bits.T2IF = 0; // reset Tx interrupt flag
   */


  // config T3 for f = 2Hz, no interrupts
  T3CONbits.TCKPS = 7;
  PR3 = 39062;
  TMR3 = 0;
  T3CONbits.TON = 1;

}
