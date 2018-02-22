#int main(void)
#{
#  char c;
#  while(1)
#  {
#    while ((c == inkey()) == 0);
#    if (c == '\n')
#      break;
#    printStr("Key pressed\n");
#  }
#  return 0;
#}
  .equ printStr, 8
  .equ input, 1
  .data
msg:  .asciiz "Key pressed\n"	
  .text
  .globl main

main:
whileout:	
while:	
  ori $v0, $0, input
  syscall
  beq $v0, 0, while
if:
  beq $v0, '\n', endwhile
  la $a0, msg
  ori $v0, $0, printStr
  syscall
  j whileout
endwhile:
  ori $v0, $0, 0
  jr $ra
