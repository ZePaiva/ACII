  .equ PRINT_STR,8
  .data
msg:  .asciiz "AC2 - DETPIC32\n"
      .text
      .globl main

main: 
  la $a0, msg
  ori $v0, $0, PRINT_STR
  syscall
  ori $v0, $0, 0
  jr $ra
