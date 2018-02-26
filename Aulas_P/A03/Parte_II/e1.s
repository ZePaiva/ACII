# Uncomment sections to test each section
  .equ readCoreTimer, 11
  .equ resetCoreTimer, 12
  .equ SFR_BASE_HI, 0XBF88
  .equ TRISE, 0X6100
  .equ LATE, 0X6120
  .equ TRISB, 0X6040
  .equ PORTB, 0x6050
  .data
msg: .asciiz "\n\n"
  .text
  .globl main

main:
  addiu $sp, $sp, -20
  sw $ra, 0($sp)
  sw $s0, 4($sp)        # target address
  sw $s1, 8($sp)        # data to show
  sw $s2, 12($sp)       # counter
  sw $s3, 16($sp)       # aux for johnson counter / LATE-put

  li $s2, 0

  #RB0-3 -> input
  lui $s0, SFR_BASE_HI
  lw $s1, TRISE($s0)
  ori $s1, $s1, 0x000F
  sw $s1, TRISE($s0)

  #RE0-3 -> output
  lw $s1, TRISE($s0)
  andi $s1, $s1, 0xFFF0
  sw $s1, TRISE($s0)

while_m:

#######################################
#  a) -> up counter
#  lw $s3, LATE($s0)
#  blt $s2, 0x0F, n_reset
#  li $s2, 0
#  j cont
#
#n_reset:
#  addi $s2, $s2, 1
#
#cont:
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 1000
#  jal delay
#  j while_m

#######################################
#  b) -> down counter
#  bgt $s2, $0, n_reset
#  li $s2, 0x0F
#  j cont
#
#n_reset:
#  addi $s2, $s2, -1
#
#cont:
#  lw $s3, LATE($s0)
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 250
#  jal delay
#  j while_m

#######################################
#  c) -> up or down counter, depending on the RB3 input
#  lw $s1, PORTB($s0)         # get all RB bits
#  andi $s1, $s1, 0x0008      # isolate RB3
#  beq $s1, 0x0008, up_count  # picks counter
#  beq $s2, $0, d_reset
#  addi $s2, $s2, -1
#  j j2while
#
#d_reset:
#  li $s2, 0x0F
#  j j2while
#
#up_count:
#  bge $s2, 0x0F, u_reset
#  addi $s2, $s2, 1
#  j j2while
#
#u_reset:
#  li $s2, 0
#  j j2while
#
#j2while:
#  lw $s3, LATE($s0)
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 500
#  jal delay
#  j while_m

#######################################
#  d) -> left johnson
#  andi $s3, $s2, 0x0F       # isolate MSB
#  srl $s3, $s3, 3
#  nor $s3, $s3, $s3         # negate MSB
#  andi $s3, $s3, 0x01       # all other bits to 0 to not mess with bits 3 to 1 
#  sll $s2, $s2, 1 
#  or $s2, $s2, $s3
#  andi $s2, $s2, 0x000F     # to ignore all other bits
#  lw $s3, LATE($s0)         # $s3 is needless now, can use it for LATE
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 666
#  jal delay
#  j while_m

#######################################
#  e) -> right johnson
#  andi $s3, $s2, 0x01       # isolate LSB
#  nor $s3, $s3, $s3         # negate LSB
#  sll $s3, $s3, 3           # make LSB -> MSB
#  srl $s2, $s2, 1
#  or $s2, $s3, $s2
#  andi $s2, $s2, 0x000F     # all other bits are ignored
#  lw $s3, LATE($s0)         # $s3 is needless now, can use it for LATE
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 666
#  jal delay
#  j while_m

#######################################
#  f) -> left or right johnson, depends on RB2 input
#  lw $s1, PORTB($s0)
#  andi $s1, $s1, 0x0004
#  beq $s1, 0x0004, left_j   #
#  andi $s3, $s2, 0x01       # isolate LSB
#  nor $s3, $s3, $s3         # negate LSB
#  andi $s3, $s3, 0x01       # all other bits to 0 to not mess with bits 3 to 1
#  srl $s2, $s2, 1
#  sll $s3, $s3, 3           # make LSB -> MSB
#  j j2while_john
#
#left_j:
#  andi $s3, $s2, 0x0F       # isolate MSB
#  srl $s3, $s3, 3
#  nor $s3, $s3, $s3         # negate MSB
#  andi $s3, $s3, 0x01       # all other bits to 0 to not mess with bits 3 to 1 
#  sll $s2, $s2, 1
#
#j2while_john:
#  or $s2, $s2, $s3
#  andi $s2, $s2, 0x000F     # to ignore all other bits
#  lw $s3, LATE($s0)         # $s3 is needless now, can use it for LATE
#  andi $s3, $s3, 0xFFF0
#  or $s3, $s3, $s2
#  sw $s3, LATE($s0)
#  li $a0, 666
#  jal delay
#  j while_m

#######################################
#  g) -> left or right ring counter, depends on RB1 input
  lw $s1, PORTB($s0)         # get all RB bits
  andi $s1, $s1, 0x0002      # isolate port RB1
  beq $s1, 0x0002, left_ring
  beq $s2, $0, right_reset   # if rez = 0 then reset to 8 
  srl $s2, $s2, 1            # else rez >> 1
  j j2while_ring

right_reset:
  li $s2, 0x08
  j j2while_ring

left_ring:
  beq $s2, 0x08, left_reset
  beq $s2, $0, left_init
  sll $s2, $s2, 1
  j j2while_ring

left_reset:
  li $s2, 0
  j j2while_ring

left_init:
  li $s2, 1
  j j2while_ring

j2while_ring:
  lw $s3, LATE($s0)
  andi $s3, $s3, 0xFFF0
  or $s3, $s3, $s2
  sw $s3, LATE($s0)
  li $a0, 333
  jal delay
  j while_m

end_wm:
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  lw $s2, 12($sp)
  lw $s3, 16($sp)
  addiu $sp, $sp, 20
  jr $ra

#########################################################################################
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
