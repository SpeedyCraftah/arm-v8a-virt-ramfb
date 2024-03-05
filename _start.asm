/*
Our program entry point which will setup the stack pointer for our C/C++ code to work properly,
then jumps to the C++ main code.
*/
.global _start
_start:
    // Set the stack pointer.
    ldr x8, =STACK_TOP
    mov sp, x8

    // Enable floating point support.
    bl enable_floating_points

    // Jump to our main C/C++ function.
    bl start

    // Runs after exit from our C/C++ code, loops and sleeps forever by waiting for an interrupt which shouldn't happen given our setup.
    sleep_loop:
        wfi
        b sleep_loop

/* 
Enables support for floating point instructions since compilers will emit floating point code
by default even if you don't use floating numbers and will cause unwanted interrupts on use,
unless you tell GCC not to emit this code with -mgeneral-regs-only compilation flag.
*/
enable_floating_points:
    mrs    x1, cpacr_el1
    mov    x0, #(3 << 20)
    orr    x0, x1, x0
    msr    cpacr_el1, x0
    ret
