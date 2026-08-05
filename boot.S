.section ".text.boot"

.global _start

_start:
    mrs x0, mpidr_el1 // Read CPU Core ID system info into box x0
    and x0, x0, #3 // Keep the last 2 bits of Core 0, 1, 2, or 3
    cbz x0, core_zero // Compare and Branch if the output is 0 - If Core 0, jump to 'core_zero'

halt_other_cores:
    wfe // Wait for event
    b halt_other_cores // Branch back to sleep if it ever wakes up accidentally

// Give C a stack Pointer so functions can store variables - set stack to _start (0x80000) and grow downward into lower RAM
core_zero:
    ldr x0, =_start // Load memory address of _start into box x0
    mov sp, x0 // Stack Pointer and copy box x0 into it

    bl main // Branch with Link - so that we can jump into our main() C function

hang:
    b hang // If main() finishes, we trap Core 0 in a loop forever