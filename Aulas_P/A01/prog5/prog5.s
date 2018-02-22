##define STR_MAX_SIZE 20
#
#char *strcat(char *, char *);
#char *strcpy(char *, char *);
#int strcmp(char *, char *);
#int strlen(char *);
#
#int main(void)
#{
#  static char str1[STR_MAX_SIZE + 1];
#  static char str2[STR_MAX_SIZE + 1];
#  static char str3[2 * STR_MAX_SIZE + 1];
#
#  printStr("Introduza 2 strings: ");
#  readStr( str1, STR_MAX_SIZE );
#  readStr( str2, STR_MAX_SIZE );
#  printStr("Resultados:\n");
#  printInt( strlen(str1), 10 );
#  printInt( strlen(str2), 10 );
#  strcpy( str3, str1 );
#  printStr( strcat( str3, str2 ) );
#  printInt10( strcmp( str1, str2 ) );
#  return 0;
#}
#
#int strlen(char *s)
#{
#  int len;
#  for(len = 0; *s != 0; len++, s++);
#  return len;
#}
#
#char *strcat(char *dst, char *src)
#{
#  char *rp = dst;
#  for(; *dst != 0; dst++);
#  strcpy(dst, src);
#  return rp;
#}
#
#char *strcpy(char *dst, char *src)
#{
#  char *rp = dst;
#  for(; (*dst = *src) != 0; dst++, src++);
#  return rp;
#}
#
#int strcmp(char *s1, char *s2)
#{
#  for(; (*s1 == *s2) && (*s1 != 0); s1++, s2++);
#  return (*s1 - *s2);
#}

  .equ printStr, 8
  .equ readStr, 9
  .equ printInt10, 7
  .equ STR_MAX_SIZE, 20
  .data
msg1:  .asciiz "Introduza 2 strings:"
msg2:  .asciiz "\nResultados:\n"
msg3:  .asciiz "Len1: "
msg4:  .asciiz "\nLen2: "
msg5:  .asciiz "\n"
str1:  .space STR_MAX_SIZE + 1
str2:  .space STR_MAX_SIZE + 1
str3:  .space STR_MAX_SIZE + STR_MAX_SIZE + 1
  .text
  .globl main

main:
  addiu $sp, $sp, -4
  sw $ra, ($sp)

  # ask for strings
  la $a0, msg1
  ori $v0, $0, printStr
  syscall

  # ask for string 1
  la $a0, str1
  ori $a1, $0, STR_MAX_SIZE
  ori $v0, $0, readStr
  syscall

  # make a newline to not confuse strings
  la $a0, msg5
  ori $v0, $0, printStr
  syscall

  # ask for string 2
  la $a0, str2
  ori $v0, $0, readStr
  syscall
 
  # print results
  la $a0, msg2
  ori $v0, $0, printStr
  syscall

  # print lenght of string 1
  la $a0, msg3
  li $v0, printStr
  syscall
  la $a0, str1
  jal strlen
  move $a0, $v0
  ori $v0, $0, printInt10
  syscall

  la $a0, msg4
  li $v0, printStr
  syscall
  la $a0, str2
  jal strlen
  move $a0, $v0
  ori $v0, $0, printInt10
  syscall

  la $a0, str3
  la $a1, str1
  jal strcpy
  la $a1, str2
  jal strcat
  move $a0, $v0
  ori $v0, $0, printStr
  syscall

  la $a0, str1
  la $a1, str2
  jal strcmp
  move $a0, $v0
  ori $v0, $0, printInt10
  syscall

  lw $ra, ($sp)
  ori $v0, $0, 0
  jr $ra

####################################################################################################
# strlen
# $v0 -> len
# $t0 -> *s
strlen:
  ori $v0, $0, 0

lenFor:
  lb $t0, 0($a0)
  beq $t0, 0, endLenFor
  addi $a0, $a0, 1
  addi $v0, $v0, 1
  j lenFor

endLenFor:
  jr $ra

####################################################################################################
# strcat
# $s0 -> *rp
strcat:
  addiu $sp, $sp, -8
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  
catFor:
  lw $s0, 0($a0)
  beq $s0, 0, endCatFor
  addi $a0, $a0, 1
  j catFor

endCatFor:
  jal strcpy
  lw $ra, 0($sp)
  lw $sp, 4($sp)
  addiu $sp, $sp, 8
  jr $ra

####################################################################################################
# strcpy
# $v0 -> rp
# $t0 -> len
# $t1 -> bstr1
strcpy:
  ori $t0, $0, 0
  move $a0, $v0

doCpy:
  lb $t1, ($a1)
  sb $t1, ($a0)
  addiu $a0, $a0, 1
  addiu $a1, $a1, 1
  addiu $t0, $t0, 1
  beq $t0, 0, doCpy
  
  jr $ra

####################################################################################################
# strcmp
# $t0 -> *s1
# $t1 -> *s2
strcmp:
forcmp:
  lb $t0, ($a0)
  lb $t1, ($a1)
  bne $t0, $t1, endCmpFor
  beq $t0, 0, endCmpFor
  addiu $a0, $a0, 1
  addiu $a1, $a1, 1
  j forcmp

endCmpFor:
 sub $v0, $t0, $t1
 jr $ra
