#void main()
#{
#  while(1)
#  {
#    printStr("Switch 1: ");
#    printInt(sw1, 2);
#    printStr("Switch 2: ");
#    printInt(sw2, 2);
#    printStr("Switch 3: ");
#    printInt(sw3, 2);
#    printStr("Switch 4: ");
#    printInt(sw4, 2);
#    delay(500);
#  }
#}
  .equ resetCoreTimer, 12
  .equ readCoreTimer, 11
  .equ printInt, 6
  .equ printStr, 8
  .data
sw1: .asciiz "\nSwitch 1: "
sw2: .asciiz "\nSwitch 2: "
sw3: .asciiz "\nSwitch 3: "
sw4: .asciiz "\nSwitch 4: "
nls: .asciiz "\n\n\n"
  .text
  .globl main
# 4 lsb $t2 -> sinais dos switches
main:
  addiu $sp, $sp, -4
  sw $ra, 0($sp)

while1:
  lui $t1, 0xBF88
  lw $t2, 0x6050($t1)

  # delay fase
  li $a0, 500
  jal delay

  la $a0, nls
  li $v0, printStr
  syscall

  # print switch 1
  la $a0, sw1
  li $v0, printStr
  syscall
  la $a1, 0x00010002
  la $a0, 0x00000008
  and $a0, $a0, $t2
  srl $a0, $a0, 3
  li $v0, printInt
  syscall
  
  #print switch 2
  la $a0, sw2
  li $v0, printStr
  syscall
  la $a0, 0x00000004
  and $a0, $a0, $t2
  srl $a0, $a0, 2
  li $v0, printInt
  syscall
  
  # print switch 3
  la $a0, sw3
  li $v0, printStr
  syscall
  la $a0, 0x00000002
  and $a0, $a0, $t2
  srl $a0, $a0, 1
  li $v0, printInt
  syscall

  # print switch 4
  la $a0, sw4
  li $v0, printStr
  syscall
  la $a0, 0x00000001
  and $a0, $a0, $t2
  li $v0, printInt
  syscall

  beq $0, $0, while1
  lw $ra, 0($sp)
  addiu $sp, $sp, 4
  jr $ra

###########################################################
delay:
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
