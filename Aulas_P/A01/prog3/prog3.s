#int main(void)
#{
#  char c;
#  while(1)
#  {
#    c = getchar()
#    if(c == '\n')
#      break;
#    putchar(c);
#  }
#  return 1;
#}
  .equ getchar, 2
  .equ putchar, 3
  .data 
  .text
  .globl main

main:
while:
  ori $v0, $0, getchar
  syscall
  or $t0, $v0, $0
if:
  beq $t0, '\n', end
  or $a0, $t0, $0
  ori $v0, $0, putchar
  syscall
  j while
end:
  ori $v0, $0, 1
  jr $ra
