#void main(void)
#{
#  int counter = 0;
#  while (1)
#  {
#    while (readCoreTimer() < 200000);
#    resetCoreTimer();
#    printInt(counter++, 10);
#    putchar(' ');
#  }
#}
  .equ readCoreTimer, 11
  .equ resetCoreTimer, 12
  .equ putchar, 3
  .equ printInt10, 7
  .data
  .text
  .globl main

# $t0 -> counter
main:
  li $t0, 0
whileOut:

# delay for 100 ms
while:
  li $v0, readCoreTimer
  syscall
  blt $v0, 2000000, while
  li $v0, resetCoreTimer
  syscall

  # increment and print
  addi $t0, $t0, 1
  li $v0, printInt10
  move $a0, $t0
  syscall
  li $a0, ' '
  li $v0, putchar
  syscall

  # make a never-ending cycle
  j whileOut

jr $ra
