#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ucontext.h>

void signal_handler(int signo, siginfo_t *info, void *context) {
    if (signo != SIGUSR1) return;

    pid_t sender_pid = info->si_pid;
    uid_t sender_uid = info->si_uid;

    struct passwd *pw = getpwuid(sender_uid);
    std::string username = pw ? pw->pw_name : "unknown";

    ucontext_t *uc = (ucontext_t*)context;

#if defined(__x86_64__)
    auto eip = uc->uc_mcontext.gregs[REG_RIP];
    auto eax = uc->uc_mcontext.gregs[REG_RAX];
    auto ebx = uc->uc_mcontext.gregs[REG_RBX];
#elif defined(__i386__)
    auto eip = uc->uc_mcontext.gregs[REG_EIP];
    auto eax = uc->uc_mcontext.gregs[REG_EAX];
    auto ebx = uc->uc_mcontext.gregs[REG_EBX];
#else
#error "Unsupported architecture"
#endif

    std::cout << "Received a SIGUSR1 signal from process " 
              << sender_pid << " executed by " 
              << sender_uid << " (" << username << ").\n";
    std::cout << "State of the context: EIP = 0x" << std::hex << eip
              << ", EAX = 0x" << eax
              << ", EBX = 0x" << ebx << std::dec << ".\n";
}

int main() {
    std::cout << "My PID: " << getpid() << std::endl;

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, nullptr);

    while (true) {
        sleep(10);
    }

    return 0;
}

