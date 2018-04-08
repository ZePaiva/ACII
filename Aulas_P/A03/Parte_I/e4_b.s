#void main()
#{
#  int v = 0;
#  TRISE.RE0 = 0;
#  while(1)
#  {
#    LATE.RE0 = v;
#    delay(20);
#    v ^=1;
#  }
#}
  .equ SFR_BASE_HI, 0XBF88
  .equ TRISE, 0X6100
  .equ LATE, 0X6120
  .equ resetCoreTimer, 12
  .equ readCoreTimer, 11
  .data
  .text
  .globl main

main:
  # store context in stack
  addiu $sp, $sp, -20
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)
  sw $s2, 12($sp)
  sw $s3, 16($sp)

  # $s0 -> v
  li $s0, 0
  lui $s1, SFR_BASE_HI
  lw $s2, TRISE($s1)
  andi $s2, $s2, 0xFFFE
  sw $s2, TRISE($s1)
  
while:
  # read - modify - write for LAT-E bit 0
  lw $s3, LATE($s1)
  andi $s3, $s3, 0x0000
  or $s0, $s0, $s3
  sw $s0, LATE($s1)
  li $a0, 20
  jal delay
  xor $s0, $s0, 1
  j while

end:
  # restore context from stack
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  lw $s3, 16($sp)
  addiu $sp, $sp, 20
  jr $ra

###################################################################################################
# delay
# $a0 -> miliseconds to wait
delay:

for:
  ble $a0, 0, endFor
  li $v0, resetCoreTimer
  syscall

while_d:
  li $v0, readCoreTimer
  syscall
  blt $v0, 20000, while_d
  addi $a0, $a0, -1
  j for

endFor:
  jr $ra
