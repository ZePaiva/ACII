#ifndef MY_FUNCS_H_
#define MY_FUNCS_H_

#define VOLTAGE(V) (V*3.3)/1023                                     // get voltage value 

unsigned char toBcd(unsigned char value);                          // convert from hexadecimal to BCD
void send2displays(unsigned char value, unsigned char dec_point);  // send a byte to hex displays, with option of having a decimal point
void delay(int ms);                                                // delays n miliseconds

#endif
