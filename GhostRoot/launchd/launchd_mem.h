

#ifndef LAUNCHD_MEM_H
#define LAUNCHD_MEM_H

// Simplified representation of launchd's internal job structure
struct job {
    struct job *next;
    char *label;
    // Add more fields as needed for introspection
};

// Global reference to job list head (resolved at runtime or via symbol)
extern struct job *launchd_job_list;

#endif // LAUNCHD_MEM_H