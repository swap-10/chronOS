// Mutual Exclusion lock

#include "types.h"

struct spinlock {
    uint locked;  // whether the lock is held
    
    // For debugging:
    char *name;  // name of the lock
    struct cpu *cpu;  // the cpu holding the lock
};