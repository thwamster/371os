.section .init
.option norvc
.type start, @function
.global start
start:
    .cfi_startproc
.option push
.option norelax
    la gp, global_pointer
.option pop
    la sp, stack_top
    la t5, bss_start
    la t6, bss_end
bss_clear:
    sd zero, (t5)
    addi t5, t5, 8
    bltu t5, t6, bss_clear
    la t0, handler
    csrw stvec, t0
    la t0, main
    csrw sepc, t0
    tail main
    .cfi_endproc
.end