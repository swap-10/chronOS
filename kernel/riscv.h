#ifndef __ASSEMBLER__

typedef unsigned long uint64;

// which hart core is this?
static inline uint64
r_mhartid() {
    uint64 x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
}

// Machine Status Register, mstatus

#define MSTATUS_MPP_MASK (3L << 11)  // previous mode
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)  // machine mode interrupt enable


static inline uint64
r_mstatus() {
    uint64 x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

static inline void
w_mstatus(uint64 x) {
    asm volatile("csrw mstatus, %0" : : "r" (x));
}

// machine exception program counter
// holds the instruction address to which a return from
// exception will go
static inline void
w_mepc(uint64 x) {
    asm volatile("csrw mepc, %0" : : "r" (x));
}

// Supervisor Status Register, sstatus
#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5)  // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4)  // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

static inline uint64
r_sstatus() {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r" (x));
    return x;
}

static inline void
w_sstatus(uint64 x) {
    asm volatile("csrw sstatus, %0" : : "r" (x));
}

// Supervisor Interrupt Pending
static inline uint64
r_sip() {
    uint64 x;
    asm volatile("csrr %0, sip" : "=r" (x));
    return x;
}

static inline void
w_sip(uint64 x) {
    asm volatile("csrw sip, %0" : : "r" (x));
}

// Supervisor mode Interrupt Enable
#define SIE_SEIE (1L << 9)  // external interrupt enable
#define SIE_STIE (1L << 5)  // timer interrupt enable
#define SIE_SSIE (1L << 1)  // software interrupt enable, one core to another

static inline uint64
r_sie() {
    uint64 x;
    asm volatile("csrr %0, sie" : "=r" (x));
    return x;
}

static inline void
w_sie(uint64 x) {
    asm volatile("csrw sie, %0" : : "r" (x));
}


// Machine mode Interrupt Enable
#define MIE_MEIE (1L << 11)  // external interrupt enable
#define MIE_MTIE (1L << 7)  // timer interrupt enable
#define MIE_MSIE (1L << 3)  // software interrupt enable, one core to another

static inline uint64
r_mie() {
    uint64 x;
    asm volatile("csrr %0, mie" : "=r" (x));
    return x;
}

static inline void
w_mie(uint64 x) {
    asm volatile("csrw mie, %0" : : "r" (x));
}


// Supervisor exception program counter
// Holds the instruction address to which a return from
// exception will go
static inline uint64
r_sepc() {
    uint64 x;
    asm volatile("csrr %0, sepc" : "=r" (x));
    return x;
}

static inline void
w_sepc(uint64 x) {
    asm volatile("csrw sepc, %0" : : "r" (x));
}


// Machine Exception Delegation
// medeleg register specifies whether particular machine level exceptions
// can be delegated to lower privilege levels
static inline uint64
r_medeleg() {
    uint64 x;
    asm volatile("csrr %0, medeleg" : "=r" (x));
    return x;
}

static inline void
w_medeleg(uint64 x) {
    asm volatile("csrw medeleg, %0" : : "r" (x));
}


// Machine Interrupt Delegation
// mideleg register specifies whether particular machine level interrupts
// can be delegated to lower privilege levels for handling
// each bit in the register might correspond to a particular type
// of interrupt and whether or not this bit is set might indicate
// whether it can be delegated to lower privilege levels
static inline uint64
r_mideleg() {
    uint64 x;
    asm volatile("csrr %0, mideleg" : "=r" (x));
    return x;
}

static inline void
w_mideleg(uint64 x) {
    asm volatile("csrw mideleg, %0" : : "r" (x));
}


// Supervisor Trap-Vector Base Address

// low two bits are mode - direct mode or vectored mode
// the stvec register is used to store the base address of the trap vector
// the trap vector stores the addresses of the exception and interrupt
// handlers for supervisor mode
// in direct mode, the base address is used, in vectored mode
// the base address + some offset based on the cause of the interrupt or
// exception might be used.
static inline uint64
r_stvec() {
    uint64 x;
    asm volatile("csrr %0, stvec" : "=r" (x));
    return x;
}

static inline void
w_stvec(uint64 x) {
    asm volatile("csrw stvec, %0" : : "r" (x));
}


// Machine-mode interrupt vector
static inline uint64
r_mtvec() {
    uint64 x;
    asm volatile("csrr %0, mtvec" : "=r" (x));
    return x;
}

static inline void
w_mtvec(uint64 x) {
    asm volatile("csrw mtvec, %0" : : "r" (x));
}


// Physical Memory Protection
// PMP - Physical Memory Protection
// pmpcfg0 - PMP Configuration Register 0
static inline void
w_pmpcfg0(uint64 x) {
    asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}


// PMP Addr Register 0
static inline void
w_pmpaddr0(uint64 x) {
    asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}


// use riscv's sv39 page table scheme
#define SATP_SV39 (8L << 60)

#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64)pagetable) >> 12))

// supervisor address translation and protection;
// holds the address of the page table
static inline uint64
r_satp() {
    uint64 x;
    asm volatile("csrr %0, satp" : : "=r" (x));
}

static inline void
w_satp(uint64 x) {
    asm volatile("csrw satp, %0" : : "r" (x));
}


// write to the mscratch register
// The mscratch register is typically used for temporary storage
// during exception handling and context switching
static inline void
w_mscratch(uint64 x) {
    asm volatile("csrw mscratch, %0" : : "r" (x));
}

// Supervisor Trap Cause
static inline uint64
r_scause() {
    uint64 x;
    asm volatile("csrr %0, scause" : "=r" (x));
    return x;
}


// Supervisor Trap Value
static inline uint64
r_stval() {
    uint64 x;
    asm volatile("csrr %0, stval" : "=r" (x));
    return x;
}

// Machine-mode Counter-Enable
// Sets access to difference performance counters
static inline uint64
r_mcounteren() {
    uint64 x;
    asm volatile("csrr %0, mcounteren" : "=r" (x));
    return x;
}

static inline void
w_mcounteren(uint64 x) {
    asm volatile("csrw mcounteren, %0" : : "r" (x));
}

// Machine-mode cycle counter
static inline uint64
r_time() {
    uint64 x;
    asm volatile("csrr %0, time" : "=r" (x));
    return x;
}

// Enable device interrupts
static inline void
intr_on() {
    w_sstatus(r_sstatus() | SSTATUS_SIE);
}

// Disable device interrupts
static inline void
intr_off() {
    w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

// Check if device interrupts are enabled
static inline int
intr_get() {
    uint64 x = r_sstatus();
    return (x & SSTATUS_SIE) != 0;
}


// Read the stack pointer
static inline uint64
r_sp() {
    uint64 x;
    asm volatile("mv %0, sp" : "=r" (x));
    return x;
}

// Read thread pointer
// The thread pointer is used to hold this core's hartid
// hardware thread id, or core number, the index into cpus[]
static inline uint64
r_tp() {
    uint64 x;
    asm volatile("mv %0, tp" : "=r" (x));
    return x;
}

static inline void
w_tp(uint64 x) {
    asm volatile("mv tp, %0" : : "r" (x));
}


// Return Address register - stores the address of the location to
// return to after completing a function call
static inline uint64
r_ra() {
    uint64 x;
    asm volatile("mv %0, ra" : "=r" (x));
    return x;
}

// flush the TLB - Translation Lookaside Buffer
// TLB is a hardware cache used to accelerate virtual to physical
// address translation
// Flushing the TLB ensures any outdated translations are removed
// from the cache
// uses the sfence.vma RISC-V instruction
// the sfence.vma instruction takes two arguments: the source register
// and the destination register
// In this case both source and destination registers are specified as zero
// indicating that all TLB entries are flushed
static inline void
sfence_vma() {
    asm volatile("sfence.vma zero, zero");
}




// A page table entry contains information about the mapping between a 
// virtual page and its corresponding physical page frame
// pte_t refers to page table entry type
// *pagetable_t is a pointer type that represents a pointer to a page-table
typedef uint64 pte_t;
typedef uint64 *pagetable_t;  // 512 PTE




#endif  // __ASSEMBLER__





#define PGSIZE 4096  // bytes per page
#define PGSHIFT 12  // bits of offset within a page


// round up a size to the nearest multiple of the PGSIZE
// and round down an address to the nearest multiple of the PGSIZE
#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(OGSIZE-1))


// Bit masks related to page table entries, relating to permissions
// and attributes of virtual memory pages in the page table
// Is used to control how different parts of the memory is accessed
// on a page by page level and this helps in memory protection
#define PTE_V (1L << 0)  // sets whether the PTE is valid or not, if invalid - page fault
#define PTE_R (1L << 1)  // sets whether the PTE can be read from or not
#define PTE_W (1L << 2)  // sets whether the PTE can be written to or not
#define PTE_X (1L << 3)  // sets whether code execution can be done from the page
#define PTE_U (1L << 4)  // sets whether the PTE can be accessed in user mode



// Shift a physical address to the right place for a PTE
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

#define PTE2PA(pte) (((pte) >> 10) << 12)

#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// extract the three 9-bit page table indices from a virtual address
#define PXMASK 0x1FF  // 9 bits
#define PXSHIFT(level) (PGSHIFT + (9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)





// one beyond the highest possible virtual address/
// MAXVA is actually one bit less than the max allowed by SV39
// to avoid having to sign-extend virtual addresses
// that have the high bit set

#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))