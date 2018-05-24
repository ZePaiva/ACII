#include<detpic32.h>
#include"eeprom.h"
#include"i2c.h"

void IOConfig();
int getTemp(int *temp);

volatile int temperature = 0;
volatile int store_temp = 29;
volatile char can_store = 0;
volatile char address = 2;

int main(){
  IOConfig();
  char command;
  int valid_values, i, value;
  i2c_init(TC74_CLK_FREQ);
  spi_init();
  spi_setClock(EEPROM_CLOCK);
  eeprom_writeStatusCommand(WREN);
  eeprom_writeData(0x00, 0x00);
  printStr("Reset(R) / Log Temperatures (L) / Show Temperatures (S) -> ");
  command = 0;
  while(1)
  {
    if (can_store && store_temp == 30)
    {
      getTemp(&temperature);
      store_temp = 0;

      if(can_store && eeprom_readData(0x0) <= 64)
      {
        char to_store = (char)(temperature & 0x00FF);
        eeprom_writeData(address, to_store);
        address++;
        eeprom_writeData(0x0, eeprom_readData(0x0)+1);
      }
      else if (eeprom_readData(0x0) > 64)
        printStr("Storage Full!!\n");
    }


    if (command != 0)
      printStr("Reset(R) / Log Temperatures (L) / Show Temperatures (S) -> ");
    command = inkey();

    if (command == 'R')
    {
      eeprom_writeData(0x0,0x0);
      printStr("EEPROM reset\n");
      store_temp = 29;
      can_store = 0;
    }
    else if (command == 'L')
    {
      printStr("\nStarting to Log Room temperature values...\nWill log each 15 seconds\n");
      can_store = 1;
      store_temp= 29;
    }
    else if (command == 'S')
    {
      valid_values = eeprom_readData(0x0);
      printStr("\n\nValid Temperatures until now ( ");
      printInt10(valid_values);
      printStr(" )\n");
      for (i = 0; i < valid_values; i++)
      {
        value = eeprom_readData(2+i);
        printStr("Temperature ");
        printInt10(i+1);
        printStr(" -> ");
        printInt10(value);
        printStr("\n");
      }
    }
    else
      command = 0;
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

void IOConfig()
{
  // Timer 1 configs for freq ~= 6.67Hz
  T3CONbits.TCKPS = 7;
  PR1 = 39061;
  TMR3 = 0;
  T3CONbits.TON = 1;
  IPC3bits.T3IP = 2;
  IEC0bits.T3IE = 1;
  IFS0bits.T3IF = 0;

  EnableInterrupts();
}

void _int_(12) isr_T3()
{
  store_temp++;
  IFS0bits.T3IF = 0;
}
