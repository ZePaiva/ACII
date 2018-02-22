#void delay(int ms)
#{
#  for(; ms > 0; ms--)
#  { 
#    resetCoreTimer();
#    while(readCoreTimer() < 20000);
#  }
#}
  .equ resetCoreTimer, 12
  .equ readCoreTimer, 11
  .data
  .text
  .globl main
main:
for:
  ble $a0, 0, endFor
  li $v0, resetCoreTimer
  syscall
while:
  li $v0, readCoreTimer
  syscall
  blt $v0, 20000, while
  addi $a0, $a0, -1
  j for
endFor:
  jr $ra
