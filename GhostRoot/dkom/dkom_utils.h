#ifndef DKOM_UTILS_H
#define DKOM_UTILS_H

#include <mach/mach_types.h>
#include <sys/types.h>

// Hide process from allproc list using its PID
void hide_process_by_pid(pid_t target_pid);

#endif // DKOM_UTILS_H
