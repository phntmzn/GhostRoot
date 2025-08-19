

#ifndef HOST_PRIVILEGE_H
#define HOST_PRIVILEGE_H

#include <mach/mach.h>

// Retrieve host-priv port from realhost structure (if available)
mach_port_t get_host_priv_port(void);

#endif // HOST_PRIVILEGE_H