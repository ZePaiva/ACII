/*
** detpic32.h
**
** TOS, January 2011, February 2012, January 2013, October 2013
*/
#ifndef DETPIC32_H
#define DETPIC32_H

#define NULL   ((void *)0)
#define FREQ   (40000000)
#define PBCLK  (FREQ/2)

#include <stdarg.h>
#include <p32mx795f512h.h>


/*
** syscall C interface
*/

#ifdef __cplusplus
extern "C" {
#endif
  /*  1 */ char inkey(void);
  /*  2 */ char getChar(void);
  /*  3 */ void putChar(char ch);
  /*  4 */ unsigned int readInt(int base);
  /*  5 */ int readInt10(void);
  /*  6 */ void printInt(unsigned int val,int base);
  /*  7 */ void printInt10(int val);
  /*  8 */ void printStr(char *s);
  /*  9 */ void readStr(char *buffer,unsigned char nc);  // "nc" does not include '\0' terminador
  /* 10 */ void exit(int return_code) __attribute__((__noreturn__));
  /* 11 */ unsigned int readCoreTimer(void);
  /* 12 */ void resetCoreTimer(void);
#ifdef __cplusplus
}
#endif


/*
** printf() replacement, called xprintf(); printf() is also available as a macro
**
** the first argument is a pointer to the output function
**   the output function takes two arguments:
**     the first argument is the character number (0 for the first character, 1 for the second, ...)
**     the second argument is the character code (0x20 for a space, etc.)
**   if the first argument is a NULL pointer it is replaced by a function that calls an inlined
**   version of putChar (with the second argument)
** the second argument is the format string
**   accepted formats (W is the width, P is the precision, +, -, 0, W, and .P are optional):
**     %c         (character)
**     %-W.Ps     (string)
**     %+-0Wd     (integer)
**     %+-0Wu     (unsigned integer)
**     %+-0Wx     (integer or unsigned integer)
**     %+-0WX     (integer or unsigned integer)
**     %+-0W.Pf   (double, will not print numbers larger than 1e10)
**   unrecognized formats
**     %ld, %lu, %lld, %llu, %e, %g
** the remaining arguments (if any) must match the format string data
** the function returns the number of characters written
**
** for example
**   xprintf(NULL,"A%7.3fB",1.2);
** sends to the terminal the string "A  1.200B"
*/

typedef void output_function(int n,char c);
#ifdef __cplusplus
extern "C" {
#endif
  int xprintf(output_function OUT,char *message,...) __attribute__((__format__(printf,2,3)));
#ifdef __cplusplus
}
#endif
#define printf(format...)  xprintf((void (*)(int,char))NULL, ## format)


/*
** to link C++ programs the symbol __gxx_personality_v0 has to be defined
*/

#ifdef __cplusplus
  extern "C" void __gxx_personality_v0(void) { exit(1); }
#endif

/*
** useful macros
*/

#define _int_(v)  __attribute__((vector(v))) __attribute__((interrupt))
#define _ramf_    __attribute__((longcall,section (".ramfunc")))

#define DisableInterrupts()  asm volatile("di")
#define EnableInterrupts()   asm volatile("ei")
#define NOP()                asm volatile("nop")


/*
** useful escape sequences (for more information run "man console_codes" on a terminal)
**
** "\e[K" ........ clear the rest of the line
** "\e[H\e[J" .... clear the entire screen
** "\e[G" ........ move cursor to the first column
** "\e[l;cH" ..... move cursor (replace l by the line number in ascii)
**                             (replace c by the column number in ascii)
**                             (for example "\e[12;40H" puts the cursor in line 12 and column 40)
** "\e[0m" ....... reset color attributes to their default values
** "\e[1;31m" .... bright red foreground color
** "\e[1;32m" .... bright green foreground color
** "\e[1;33m" .... bright yellow foreground color
** "\e[1;36m" .... bright cyan foreground color
** "\e[1;37m" .... bright white foreground color
*/
#endif

