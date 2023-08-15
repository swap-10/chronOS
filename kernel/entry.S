.section .text
.global _entry
_entry:
    # set up a stack for C
    # stack0 is declared in start.c

    la sp, stack0
    li a0, 1023*4
    cssr a1, mhartid
    addi a1, a1, 1
    mul a0, a0, a1
    add sp, sp, a0
    # jump to start() in start.c
    call start

spin:
    j spin