  .equ resetCoreTimer, 12
  .equ readCoreTimer, 11
  .equ SFR, 0xBF88
  .equ TRISE, 0x6100
  .equ LATE, 0x6120
  .equ TRISB, 0x6040
  .equ PORTB, 0x6050
  .equ printStr, 8
  .equ CLK, 20000
  .data
name: .asciiz "NOME:Jose Miguel Pinho Paiva NMEC: 84972\n"
  .text
  .globl main

main:
# $s0 -> to read/write register
# $s2 -> amount in seq
# $s3 -> flag
  addiu $sp, $sp, -4
  sw $ra, 0($sp)
  
  li $s0, 0
  li $s1, 0
  li $s2, 0
  
  # config to read DPS4
  lui $s0, SFR
  lw $s1, TRISB($s0)
  ori $s1, $s1, 0x0008
  sw $s1, TRISB($s0)
  
  # config to write in leds
  lw $s1, TRISE($s0)
  andi $s1, $s1, 0xFFF0
  sw $s1, TRISE($s0)
  lw $s1, LATE($s0)
  andi $s1, $s1, 0xFFF0
  sw $s1, LATE($s0)

main_while:
  # imprime nome e nmec
  la $a0, name
  li $v0, printStr
  syscall
 
  # read DPS4
  lw $s1, PORTB($s0)
  andi $s1, $s1, 0x0008
  beq $s1, 0x08, ms_1
  li $a0, 800
  j to_del

ms_1:
  li $a0, 100

to_del:
  jal delay
  blt $s2, 1, swap_flag
  bgt $s2, 0x04, swap_flag

counters:
  beq $s3, $0, down_count
  addi $s2, $s2, 1
  lw $s1, LATE($s0)
  and $s1, $s1, $0
  or $s1, $s1, $s2
  sw $s1, LATE($s0)
  j end_main

down_count:
  addi $s2, $s2, -1
  lw $s1, LATE($s0)
  and $s1, $s1, $0
  or $s1, $s1, $s2
  sw $s1, LATE($s0)
  j end_main

swap_flag:
  not $s3, $s3
  j counters

end_main:
  j main_while

  lw $ra, 0($sp)
  addiu $sp, $sp, 4
  jr $ra

#delay:
#  bgt $a0, $0, end_d
#  li $v0, resetCoreTimer
#  syscall
#
#while_d:
#  li $v0, readCoreTimer
#  syscall
#  blt $v0, CLK, while_d
#  addi $a0, $a0, -1
#  j delay
#
#end_d:
#  jr $ra


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
