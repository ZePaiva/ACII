#include<detpic32.h>
#include"eeprom.h"

char eeprom_readStatus()
{
  volatile char trash;
  
  while (SPI2STATbits.SPIRBE == 0)
    trash = SPI2BUF;

  SPI2STATbits.SPIROV = 0;

  SPI2BUF = RDSR;
  SPI2BUF = 0;

  while (SPI2STATbits.SPIBUSY);
  trash = SPI2BUF;

  return SPI2BUF;
}

void eeprom_writeStatusCommand(char command)
{
  while (eeprom_readStatus() & 0x01);

  SPI2BUF = command;
  while( SPI2STATbits.SPIBUSY == 1);
}

void eeprom_writeData(int address, char value)
{
  address &= 0x01FF;
  while (eeprom_readStatus() & 0x01);
  eeprom_writeStatusCommand(WREN);
  SPI2BUF = WRITE | ((address & 0x100) >> 5);
  SPI2BUF = address & 0x00FF;
  SPI2BUF = value;
  while (SPI2STATbits.SPIBUSY == 1);
}

char eeprom_readData(int address)
{
  volatile char trash;
  while (SPI2STATbits.SPIRBE == 0)
    trash = SPI2BUF;

  SPI2STATbits.SPIROV = 0;
  
  address &= 0x01FF;

  while(eeprom_readStatus() & 0x01);

  SPI2BUF = READ | ((address & 0x100) >> 5);
  SPI2BUF = address & 0x0FF;
  SPI2BUF = 0x00;
  
  while (SPI2STATbits.SPIBUSY == 1);

  trash = SPI2BUF;
  trash = SPI2BUF;

  return SPI2BUF;
}


void spi_setClock(unsigned int clk_freq)
{
  SPI2BRG = (PBCLK + clk_freq) / (2 * clk_freq) - 1;
}

void spi_init()
{
  volatile char trash;
  SPI2CONbits.ON = 0;
  SPI2CONbits.CKP = 0;
  SPI2CONbits.CKE = 1;
  SPI2CONbits.SMP = 0;
  SPI2CONbits.MODE32 = 0;
  SPI2CONbits.MODE16 = 0;
  SPI2CONbits.ENHBUF = 1;
  SPI2CONbits.MSSEN = 1;
  SPI2CONbits.MSTEN = 1;
  
  while (SPI2STATbits.SPIRBE == 0)
    trash = SPI2BUF;

  SPI2STATbits.SPIROV = 0;
  SPI2CONbits.ON = 1;
}

