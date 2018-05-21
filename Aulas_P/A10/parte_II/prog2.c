#include<detpic32.h>
#include<my_funcs.h>
#include"i2c.h"

int getTemp(int *tmp);

int main()
{
  i2c_init(TC74_CLK_FREQ);
  int temperature, error;
  while(1)
  {
    error = getTemp(&temperature);
    if (error)
    {
      i2c_stop();
      printStr("Ocorreu um erro!\nA sair!\n");
      break;
    }
    printStr("Temperatura -> ");
    printInt10(temperature);
    putChar('\n');
    delay(250);
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
