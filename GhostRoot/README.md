# GhostRoot

⚠️ **WARNING: This project is for educational and research purposes only. Do not deploy on unauthorized systems.**

**GhostRoot** is a modular macOS rootkit framework targeting pre-SIP systems (macOS ≤ 10.14), showcasing multiple kernel and userland techniques for stealth, privilege escalation, and persistence.

## Features

- **DKOM Process Hiding**:  
  Unlinks processes from `allproc` and PID hash tables via direct kernel object manipulation.

- **Launchd Job Unlinking**:  
  Traverses launchd’s internal job list and removes entries by label, making daemons invisible while active.

- **Privilege Escalation**:  
  Replaces `task->itk_host` with `realhost->special[2]` to gain the `host_priv` Mach port.

- **Kernel Memory R/W Access**:  
  Provides primitives to read/write memory of arbitrary kernel tasks using Mach APIs.

- **Unsigned KEXT Loader**:  
  Injects unsigned kernel extensions directly from memory using legacy syscalls on macOS ≤10.9.

- **Root Escalation via Authorization API**:  
  Uses deprecated `AuthorizationExecuteWithPrivileges()` to gain root access from userland.

- **Dylib Injection**:  
  Spawns remote threads using `thread_set_state` to inject `.dylib` into any task.

- **Filesystem and Log Stealth**:  
  Hides files from userland tools by hooking `getdirentries64`; clears unified logs and system records.

## Directory Layout

```
GhostRoot/
├── build/                  # Compiled KEXT/dylib artifacts
├── dkom/                   # hide_process.c, dkom_utils.h
├── docs/                   # architecture.md (technical internals)
├── kernel_access/          # task_control.c, mem_rw.c
├── kext/                   # rootkit.kext/, Makefile
├── launchd/                # unlink_job.c, launchd_mem.h
├── loader/                 # mykextload.c, kext_request_stub.S
├── privilege/              # escalate.c, host_privilege.h
├── root_escalation/        # gain_root.m
├── stealth/                # hide_fs.c, clean_logs.sh
├── utils/                  # inject_dylib.m, symbols.py
└── README.md               # You're here
```

## Requirements

- macOS 10.6–10.14
- Root access for kernel module loading
- XNU headers and legacy SDK (e.g. 10.9) for compilation
- `jtool` or `nm` to resolve kernel symbols

## Disclaimer

Use of this code is strictly limited to lawful, ethical research and testing environments. The authors are not responsible for misuse or unauthorized deployment.
