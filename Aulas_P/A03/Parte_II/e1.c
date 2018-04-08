#include<detpic32.h>

void delay(int ms);
void IOconf(void);
void up_counter(void);
void down_counter(void);
void mixed_counter(void);
void left_johnson(void);
void right_johnson(void);
void mixed_johnson(void);
void mixed_ring(void);

int counter = 0, mlsb = 0;

int main(void)
{
  IOconf();
  // uncomment only when testing ring counter
  counter = 1;
  while(1)
  {
    // a)
    // up_counter();

    // b)
    // down_counter();

    // c)
    // mixed_counter();
    
    // d)
    // left_johnson();

    // e)
    // right_johnson();

    // f)
    // mixed_johnson();
  
    // g)
    mixed_ring();
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

void IOconf()
{
  TRISE = TRISE & 0xFFF0; // REx -> output allowed
  TRISB = TRISB | 0x000F; // RBx -> input allowed
  LATE = LATE & 0xFFF0;   // REx -> output 
  PORTB = PORTB & 0xFFF0; // RBx -> input 
}

void up_counter()
{
  LATE = (LATE & 0xFFF0) | counter;
  counter++;
  delay(1000);
}

void down_counter()
{
  LATE = (LATE & 0xFFF0) | counter;
  counter--;
  delay(250);
}

void mixed_counter()
{
  LATE = (LATE & 0xFFF0) | counter;
  if (PORTBbits.RB3 == 0)
    counter--;
  else
    counter++;
  delay(500);
}

void left_johnson()
{
  LATE = (LATE & 0xFFF0) | counter;
  mlsb = (counter & 0x0008);         // isolate MSB
  mlsb >>= 3;                        // make it LSB
  counter <<= 1;                     // shift left counter
  counter &= 0x0F;                   // get only bits 3 to 0, ignore all the others
  counter |= mlsb ^ 1;               // replace LSB for nem value of LSB
  delay(1000/1.5);
}

void right_johnson()
{
  LATE = (LATE & 0xFFF0) | counter;
  mlsb = (counter & 0x0001);         // isolate LSB
  mlsb <<= 3;                        // make it MSB
  counter >>= 1;                     // shift left counter
  counter &= 0x0F;                   // get only bits 3 to 0, ignore all the others
  counter |= mlsb ^ 8;               // replace MSB for nem value of MSB
  delay(1000/1.5);
}

void mixed_johnson()
{
  LATE = (LATE & 0xFFF0) | counter;
  if (PORTBbits.RB2 == 0)
    right_johnson();
  else
    left_johnson();
}

void mixed_ring()
{
  LATE = (LATE & 0xFFF0) | counter;
  if (PORTBbits.RB1 == 0)
    if (counter == 1)
      counter = 8;
    else
      counter >>=1;
  else
    if (counter == 8)
      counter = 1;
    else
      counter <<= 1;
  delay(1000/3);
}
