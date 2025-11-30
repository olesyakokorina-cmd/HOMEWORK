#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ucontext.h>
#include <cstdio>
#include <cstdlib>

char username_buf[64] = "unknown";

void signal_handler(int signo, siginfo_t *info, void *context) {
    (void)signo;

    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    ucontext_t *uc = (ucontext_t*)context;

#if defined(__x86_64__)
    long eip = uc->uc_mcontext.gregs[REG_RIP];
    long eax = uc->uc_mcontext.gregs[REG_RAX];
    long ebx = uc->uc_mcontext.gregs[REG_RBX];
#elif defined(__i386__)
    long eip = uc->uc_mcontext.gregs[REG_EIP];
    long eax = uc->uc_mcontext.gregs[REG_EAX];
    long ebx = uc->uc_mcontext.gregs[REG_EBX];
#else
    long eip = 0, eax = 0, ebx = 0;
#endif

    char buf[256];
    int n = snprintf(buf, sizeof(buf),
        "Received SIGUSR1 from PID %d, UID %d (%s)\nEIP=0x%lx, EAX=0x%lx, EBX=0x%lx\n",
        sender_pid, sender_uid, username_buf, eip, eax, ebx);
    if (n > 0) write(STDOUT_FILENO, buf, n);
}

int main() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        strncpy(username_buf, pw->pw_name, sizeof(username_buf) - 1);
        username_buf[sizeof(username_buf)-1] = '\0';
    }

    char pid_buf[64];
    int n = snprintf(pid_buf, sizeof(pid_buf), "My PID: %d\n", getpid());
    if (n > 0) write(STDOUT_FILENO, pid_buf, n);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        const char err_msg[] = "Error: sigaction failed\n";
        write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
        exit(1);
    }

    while (true) {
        sleep(10);
    }

    return 0;
}

