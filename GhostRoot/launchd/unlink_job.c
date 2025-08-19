// Walk launchd job list and unlink job

// Walk launchd job list and unlink job

#include <stdio.h>
#include <string.h>

// This code is for research and demonstration purposes only.
// It assumes access to internal launchd structures (macOS ≤10.10).
// Do not use in production.

// Minimal placeholder for launchd's internal job structure
struct job {
    struct job *next;
    char *label;
    // ... other fields
};

// Assume we have access to the global job list head (symbol from launchd)
extern struct job *launchd_job_list;

void unlink_job(const char *target_label) {
    struct job *prev = NULL;
    struct job *curr = launchd_job_list;

    while (curr) {
        if (curr->label && strcmp(curr->label, target_label) == 0) {
            // Found the job to unlink
            if (prev) {
                prev->next = curr->next;
            } else {
                // Unlinking head
                launchd_job_list = curr->next;
            }
            printf("[+] Unlinked job: %s\n", target_label);
            // Optionally: free(curr); // Only if job is dynamically allocated
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("[-] Job not found: %s\n", target_label);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <job_label>\n", argv[0]);
        return 1;
    }
    unlink_job(argv[1]);
    return 0;
}