#ifndef MY_FUNCS_H_
#define MY_FUNCS_H_

#define VOLTAGE(V) (V*33)/1023                                     // get voltage value 

unsigned char toBcd(unsigned char value);                          // convert from hexadecimal to BCD
void send2displays(unsigned char value, unsigned char dec_point);  // send a byte to hex displays, with option of having a decimal point
void delay(int ms);                                                // delays n miliseconds
void setPWM(unsigned int duty, int PR, int sync);                  // varies the duty cycle of PWM signals with given PR value and if it is synchronous or asynchronous clock

#endif
