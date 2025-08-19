

#include <mach/mach_types.h>
#include <mach/mach_vm.h>
#include <kern/task.h>
#include <libkern/libkern.h>

// Read memory from target task at `address` into `buffer`, up to `size` bytes.
kern_return_t read_kernel_memory(task_t target_task, mach_vm_address_t address, void *buffer, mach_vm_size_t size) {
    mach_vm_size_t out_size = 0;
    vm_offset_t data;
    kern_return_t kr = mach_vm_read(target_task, address, size, &data, &out_size);
    if (kr != KERN_SUCCESS) {
        return kr;
    }

    memcpy(buffer, (void *)data, out_size);
    vm_deallocate(kernel_map, data, out_size);
    return KERN_SUCCESS;
}

// Write memory to target task at `address` from `buffer`, up to `size` bytes.
kern_return_t write_kernel_memory(task_t target_task, mach_vm_address_t address, void *buffer, mach_vm_size_t size) {
    return mach_vm_write(target_task, address, (vm_offset_t)buffer, (mach_msg_type_number_t)size);
}