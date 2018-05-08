#ifndef MY_FUNCS_H_
#define MY_FUNCS_H_

#define VOLTAGE(V) (V*33)/1023                                                // get voltage value 

unsigned char toBcd(unsigned char value);                                     // convert from hexadecimal to BCD
void send2displays(unsigned char value, unsigned char dec_point);             // send a byte to hex displays, with option of having a decimal point
void delay(int ms);                                                           // delays n miliseconds
void setPWM(unsigned int duty, int PR, int sync);                             // varies the duty cycle of PWM signals with given PR value and if it is synchronous or asynchronous clock
void UART1Config(unsigned int baudrate, char parity, char data, char stop);   // configs UART1 with given baudrate, parity and data selection bits and stop bits 
void putString(char *strin);                                                  // send a string to UART1
void putc(char toSend);                                                       // send a charachter to UART1
char getc();                                                                   // get a character from serial

#endif
