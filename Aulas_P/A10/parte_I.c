#include<detpic32.h>
#include<my_funcs.h>

#define I2C_READ  1
#define I2C_WRITE 0
#define I2C_ACK   0
#define I2C_NACK  1

#define SENS_ADDRESS  0X4D
#define ADDR_WR       ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD       ((SENS_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ 100000
#define RTR           0

void i2c_init(unsigned int clk_freq);
void i2c_start();
void i2c_stop();
int i2c_send(unsigned char value);
char i2c_receive(char ack_bit);

int main()
{
  int ack, temperature;
  i2c_init(TC74_CLK_FREQ);
  while(1)
  {
    i2c_start();
    ack = i2c_send(ADDR_WR);
    ack += i2c_send(RTR);
    i2c_start();
    ack += i2c_send(ADDR_RD);
    if (ack != 0)
    {
      i2c_stop();
      printStr("Ocorreu um erro!\nA sair!\n");
      break;
    }

    temperature = (int)i2c_receive(I2C_NACK);
    i2c_stop();
    printStr("Temperatura -> ");
    printInt10(temperature);
    putChar('\n');
    delay(250);
  }
  return 0;
}

void i2c_init(unsigned int clk_freq)
{
  I2C1BRG = (PBCLK + clk_freq) / (2 * clk_freq) - 1;
  I2C1CONbits.ON = 1;
}

void i2c_start()
{
  while((I2C1CON & 0x001F) != 0);
  I2C1CONbits.SEN = 1;
  while(I2C1CONbits.SEN == 1);
}

void i2c_stop()
{
  while((I2C1CON & 0x001F) != 0);
  I2C1CONbits.PEN = 1;
  while(I2C1CONbits.PEN == 1);
}

int i2c_send(unsigned char value)
{
  I2C1TRN = value;
  while(I2C1STATbits.TRSTAT == 1);
  return I2C1STATbits.ACKSTAT;
}

char i2c_receive(char ack_bit)
{
  while((I2C1CON & 0x001F) != 0);
  I2C1CONbits.RCEN = 1;
  while(I2C1STATbits.RBF != 1);
  
  if (ack_bit != 0 && ack_bit != 1)
    ack_bit = 0;
  
  I2C1CONbits.ACKDT = ack_bit; 
  
  while((I2C1CON & 0x001F) != 0);
  I2C1CONbits.ACKEN = 1;

  while(I2C1CONbits.ACKEN == 1);
  return I2C1RCV;
}


