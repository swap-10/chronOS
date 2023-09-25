#
# interrupts and exceptions while in supervisor mode come here
#
# the current stack is a kernel stack
# push all registers, call kerneltrap()
# when kerneltrap() returns, restore registers, return.

.globl kerneltrap
.globl kernelvec
.align 4
kernelvec:
    # This is basically like handling interrupt vector.
    # We're saving the state by pushing the current registers onto the stack
    # and calling the trap(). When kerneltrap() returns, we'll appropriately
    # restore the values of the registers from where we had saved them on
    # the stack and return from the interrupt vector table routine (kernelvec)


    # make room to save registers, moves down stack pointer by 256 bytes
    addi sp, sp, -256

    # save the registers
    
    # return address register
    sd ra, 0(sp)

    # stack pointer
    sd sp, 8(sp)

    # global pointer
    sd gp, 16(sp)

    # thread pointer
    sd tp, 24(sp)

    # t0-t6 Temporary registers
    sd t0, 32(sp)
    sd t1, 40(sp)
    sd t2, 48(sp)

    # s0-s11 Saved registers - values preserved across function calls
    sd s0, 56(sp)
    sd s1, 64(sp)

    # a0-a7 Argument registers - used to save function arguments and return
    # values
    sd a0, 72(sp)
    sd a1, 80(sp)
    sd a2, 88(sp)
    sd a3, 96(sp)
    sd a4, 104(sp)
    sd a5, 112(sp)
    sd a6, 120(sp)
    sd a7, 128(sp)

    sd s2, 136(sp)
    sd s3, 144(sp)
    sd s4, 152(sp)
    sd s5, 160(sp)
    sd s6, 168(sp)
    sd s7, 176(sp)
    sd s8, 184(sp)
    sd s9, 192(sp)
    sd s10, 200(sp)
    sd s11, 208(sp)

    sd t3, 216(sp)
    sd t4, 224(sp)
    sd t5, 232(sp)
    sd t6, 240(sp)

    # call the C trap handler in trap.c
    call kerneltrap

    # after returning from kerneltrap

    # restore registers
    ld ra, 0(sp)
    ld sp, 8(sp)
    ld gp, 16(sp)
    
    # not tp as it contains hartid, in case we moved across CPUs
    # so we let tp be as it is now, corresponding to the cpu core
    # we're currently on

    ld t0, 32(sp)
    ld t1, 40(sp)
    ld t2, 48(sp)
    ld s0, 56(sp)
    ld s1, 64(sp)
    ld a0, 72(sp)
    ld a1, 80(sp)
    ld a2, 88(sp)
    ld a3, 96(sp)
    ld a4, 104(sp)
    ld a5, 112(sp)
    ld a6, 120(sp)
    ld a7, 128(sp)

    ld s2, 136(sp)
    ld s3, 144(sp)
    ld s4, 152(sp)
    ld s5, 160(sp)
    ld s6, 168(sp)
    ld s7, 176(sp)
    ld s8, 184(sp)
    ld s9, 192(sp)
    ld s10, 200(sp)
    ld s11, 208(sp)

    ld t3, 216(sp)
    ld t4, 224(sp)
    ld t5, 232(sp)
    ld t6, 240(sp)

    # move the stack pointer back up by 256 bytes
    addi sp, sp, 256

    # return to whatever the program was doing in the kernel
    sret

    #
    # machine mode timer intterupt
    #

.globl timervec
.align 4
timervec:
    # start.c has set up the memory that mscratch points to:
    # scratch[0,8,16]: register save area
    # scratch[24]: address of CLINT's MTIMECMP register
    # scratch[32]: desired interval between interrupts
    # values in brackets are in bytes

    # CSR RW - basically swaps the values in a0 and mscratch
    csrrw a0, mscratch, a0

    # saves the value of these registers into the memory location pointed
    # to by a0 + the offset
    sd a1, 0(a0)
    sd a2, 8(a0)
    sd a3, 16(a0)

    # schedule the next timer interrupt
    # by adding interval to mtimecmp

    # load into a1, a2 registers the address to CLINT_MTIMECMP and interval
    ld a1, 24(a0)  # CLINT_MTIMECMP(hart)
    ld a2, 32(a0)  # interval

    # The CLINT_MTIMECMP macro also stores the address of CLINT's MTIMECMP
    # register, not the actual values in the register

    # load into a3 the value at the address in a1 (CLINT_MTIMECMP)
    ld a3, 0(a1)
    # add interval to CLINT_MTIMECMP, the time after which the next timer
    # interrupt will arise after the immediate next one
    add a3, a3, a2

    # store this updated value of CLINT_MTIMECMP back in the CLINT_MTIMECMP
    # register
    sd a3, 0(a1)

    # arrange for a supervisor software interrupt after this handler
    # returns
    li a1, 2
    csrw sip, a1

    # restores the registers a0, a1, a2, a3
    ld a3, 16(a0)
    ld a2, 8(a0)
    ld a1, 0(a0)
    csrrw a0, mscratch, a0

    mret