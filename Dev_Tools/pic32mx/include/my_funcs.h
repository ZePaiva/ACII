#ifndef MY_FUNCS_H_
#define MY_FUNCS_H_

unsigned char toBcd(unsigned char value);                          // convert from hexadecimal to BCD
void send2displays(unsigned char value, unsigned char point_pip);  // send a byte to hex displays
void delay(int ms);                                                // delays n miliseconds

#endif
