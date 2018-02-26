  .equ SFR_BASE_HI, 0XBF88  # 16 MSB of SFR area
  .equ TRISE, 0X6100        # TRIS-E address is 0XBF886100 -> '1' is input, '0' is output (E)  
  .equ LATE,  0X6120        # LAT-E address is 0XBF886120  -> value of output port in E
  .equ TRISB, 0x6040        # TRIS-B address is 0XBF886040 -> '1' is input, '0' is output (B)
  .equ PORTB, 0X6050        # PORT-B address is 0XBF886050 -> value of input port in E
  .data
  .text
  .globl main

main:
  # make RE0 - RE3 output
  lui $t1, SFR_BASE_HI
  lw $t2, TRISE($t1)
  andi $t2, $t2, 0xFFF0
  sw $t2, TRISE($t1)

  # make RB0 - RB3 input
  lw $t2, TRISB($t1)
  ori $t2, $t2, 0x000F
  sw $t2, TRISB($t1)

inf_loop:
  # read RB0 -> bit 0
  lw $t2, PORTB($t1)
  
  # store & change bit
  move $t3, $t2
  andi $t3, $t3, 0x0006      # to change only bit 3 and 0
  nor $t2, $t2, $t2
  andi $t2, $t2, 0xFFF9
  or $t2, $t2, $t3

  # write RE0 -> bit 0
  sw $t2, LATE($t1)
  j inf_loop

  jr $ra
