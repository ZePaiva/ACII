#ifndef I2C_H_
#define I2C_H_

#define I2C_READ  1
#define I2C_WRITE 0
#define I2C_ACK   0
#define I2C_NACK  1
#define SENS_ADDRESS  0X4D
#define ADDR_WR       ((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD       ((SENS_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ 100000
#define RTR 0

void i2c_init(unsigned int clk_freq);
void i2c_start();
void i2c_stop();
int i2c_send(unsigned char value);
char i2c_receive(char ack_bit);

#endif
