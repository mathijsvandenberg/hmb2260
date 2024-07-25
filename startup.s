.text
.globl _Reset
_Reset:
lui $sp, %hi(stack_top)
addiu $sp, $sp, %lo(stack_top)
lui $t9, %hi(c_entry)
addiu $t9, %lo(c_entry)
jalr $t9
nop
hang:
b hang
