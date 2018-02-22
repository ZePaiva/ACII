#void main(void)
#{
#  int value;
#  while(1)
#  {
#    printStr("\nIntroduza um numero (sinal e módulo): ");
#    value = readInt10();
#    printStr("\nValor lido em base 2: ");
#    printInt(value, 2);
#    printStr("\nValor lido em base 16: ");
#    printInt(value, 16);
#    printStr("\nValor lido em base 10 (unsigned): ");
#    printInt(value, 10);
#    printStr("\nValor lido em base 10 (signed): ");
#    printInt(value);
#  }
#}
  .equ readInt10, 5
  .equ printInt, 6
  .equ printInt10, 7
  .equ printStr, 8
  .data
msg1:  .asciiz "\nIntroduza um numero(sinal e modulo): "
msg2:  .asciiz "\nValor lido em base 2: "
msg3:  .asciiz "\nValor lido em base 16: "
msg4:  .asciiz "\nValor lido em base 10 (unsigned): "
msg5:  .asciiz "\nValor lido em base 10 (signed): "
  .text
  .globl main

main:
while:
  la $a0, msg1
  ori $v0, $0, printStr
  syscall

  ori $v0, $0, readInt10
  syscall

  move $t0, $v0
  la $a0, msg2
  ori $v0, $0, printStr
  syscall
  move $a0, $t0
  li $a1, 2
  ori $v0, $0, printInt
  syscall

  la $a0, msg3
  ori $v0, $0, printStr
  syscall
  move $0, $t0
  move $a0, $t0
  li $a1, 16
  ori $v0, $0, printInt
  syscall

  la $a0, msg4
  ori $v0, $0, printStr
  syscall
  move $a0, $t0
  li $a1, 10
  ori $v0, $0, printInt
  syscall

  la $a0, msg5
  ori $v0, $0, printStr
  syscall
  move $a0, $t0
  ori $v0, $0, printInt
  syscall

end:
  jr $ra
