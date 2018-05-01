#include<detpic32.h>
#include<my_funcs.h>

void IOConf();

int main()
{
  IOConf();
  while(1)
  {
    unsigned int duty_array[21] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
    int i, j; 
    for(i = 0; i < 21; i++)
    {
      setPWM(duty_array[i], 49999, 1);
      j = 0;
      while(j < 20000000)
        j++;
    }
  }
  return 0;
}

void IOConf()
{
  // Timer configs
  T3CONbits.TCKPS = 2;    // KP = 3.01 -> 4 (2)
  PR3 = 49999;            // PR = (5E6/100)-1 -> 5E4-1 
  TMR3 = 0;
  T3CONbits.TON = 1;

  // PWM configs
  OC1CONbits.OCM = 6;     
  OC1CONbits.OCTSEL = 1;
  OC1RS = 12500;          // OC1RS = ((49999 + 1) * 25)/100
  OC1CONbits.ON = 1;
}
