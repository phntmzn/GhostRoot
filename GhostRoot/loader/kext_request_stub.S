


.global _kextload_stub
.align 4

_kextload_stub:
    mov     $180, %rax        // SYS_kextload
    syscall
    ret