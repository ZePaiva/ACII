#include<detpic32.h>
#include<my_funcs.h>

int main(void)
{
  //B0-3 in
  TRISB = TRISB | 0x000F;
  PORTB = PORTB & 0XFFF0;

  //B8-14 out
  TRISB = TRISB & 0x00FF;
  LATB = LATB & 0x00FF;

  //D5-6 out
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 1;

  unsigned char val = 0;
  unsigned char decVal;
  char restart = 0;
  char onOff = 0;
  int i, j;
  while(1)
  {
    i = 0;
    if (!restart)
    {
      do{
        delay(10);
        decVal = toBcd(val);
        send2displays(decVal, 1);
      } while (++i < 100);
      val++;
      if (val % 60 == 0){
        val = 0;
        restart = 1;
      }
    }
    else
    {
      do
      {
        j = 0;
        if (onOff)
        {
          do{
            delay(10);
            decVal = toBcd(val);
            send2displays(decVal, 1);
          } while (++j < 50);
        }
        else
        {
          do{
            delay(10);
            LATB = LATB & 0x00FF;
          } while (++j < 50);
        }
        onOff = !onOff;
      }while(++i < 10);
      restart = 0;
      val++;
    }
  }
}
