#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();

// entry.S needs one stack per CPU
// __attribute__ ((aligned(16))) is a compiler-specific attribute
// that aligns the memory allocation to a specific boundary.

// Alignment refers to the memory boundary to which a variable's address must
// be a multiple of. When data is aligned, it allows the processor to access
// the data more efficiently.

// The __attribute__ keyword helps specify additional information to the compiler
// about various aspects of variables, functions and types.

// the aligned() function is used to specify the desired alignment for a variable
// in bytes.

// Here this means that the memory used for the stack0 array must be aligned to
// a 16-byte boundary, or the address of the start of the stack0 array should
// be divisible by 16.

// stack0 is a 1D array that holds the stack for each CPU of the system
// each stack area can be accessed by offseting from the base address which
// will be of the form CPU_Num * 4096
__attribute__ ((aligned(16))) char stack0[4096 * NCPU];

// a scratch area per CPU for machine-mode timer interrupts.
uint64 timer_scratch[NCPU][5];


// assembly code in kernelvec.S for machine-mode timer interrupt
extern void timervec();

// entry.S jumps here in machine mode on stack0
void start() {
    // set M previous privilege mode to Supervisor for MRET
    uint64 x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;  // clear previous privilege mode
    x |= MSTATUS_MPP_S;  // set previous privilege mode as supervisor mode
    
    // when the system returns from MRET, it will resume in supervisor mode

    w_mstatus(x);


    // set M Exception Program Counter to main, for mret
    // requires gcc -mcmodel=medany
    // mepc holds the instruction address to which the program counter will go
    // post a return from exception
    w_mepc((uint64)main);

    // disable paging for now - set address of page table to 0
    w_satp(0);

    // delegate all interrrupts and exceptions from machine mode to
    // supervisor mode
    
    // 0xffff sets all bits - so all instructions can be delegeted
    w_medeleg(0xffff);
    w_mideleg(0xffff);

    // enable all types of interrupts for supervisor mode
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);


    // Configure Physical Memory Protection to give supervisor mode
    // access to all of physical memory

    // the 0x3fff...ULL format is used to conveniently indicate that the
    // specified address is to mean the extent of the available memory
    // the compiler ignores the non-hexadecimal digits so that
    // 0x3fff....ffuLL would be treated as 0x3fff....ff
    // this is the highest address in a 64-bit address space
    w_pmpaddr0(0x3fffffffffffffULL);
    w_pmpcfg0(0xf);

    // ask for clock interruots
    timerinit();

    // keep each CPU's hartid in its thread pointer register, for cpuid()
    uint64 id = r_mhartid();
    w_tp(id);


    // switch to supervisor mode and jump to main using MRET
    asm volatile("mret");
}



// arrange to receive timer interrupts. They will arrive in machine mode
// at timervec in kernelvec.S, which turns them into software interrupts
// for devintr() in trap.c

void timerinit() {
    // each CPU has a separate source of timer interrupts
    int id = r_mhartid();


    // ask the CLINT for a timer interrupt.
    // CLINT is the Core Local INTerrrupter, which contains the timer
    // interrupt

    int interval = 1000000;  // number of cycles;
    *(uint64*) CLINT_MTIMECMP(id) = *(uint64*) CLINT_MTIME + interval;

    // prepare information in scratch[] for timervec.
    // scratch[0..2]: space for timervec to save registers
    // scratch[3]: address of CLINT MTIMECMP register
    // scratch[4]: desired interval (in cycles) between timer interrupts
    uint64 *scratch = &timer_scratch[id][0];
    scratch[3] = CLINT_MTIMECMP(id);
    scratch[4] = interval;
    w_mscratch((uint64)scratch);

    // set the machine-mode trap handler
    w_mtvec((uint64) timervec);

    // enable machine-mode interrupts
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    // enable machine-mode timer interrupts
    w_mie(r_mie() | MIE_MTIE);
}