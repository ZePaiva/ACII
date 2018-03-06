#include<detpic32.h>

void delay(int ms);

int main()
{
  TRISE = TRISE & 0xFFF0;
  LATE = LATE & 0xFFF0;

  while(1)
  {
    LATE = LATE + 1;
    if (LATE > 15)
      LATE = 0;
    delay(250);
  }

  return 0;
}

void delay (int ms)
{
  for (; ms > 0; ms--){
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}
