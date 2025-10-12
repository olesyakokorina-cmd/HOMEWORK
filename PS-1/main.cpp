#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

void do_command(char** argv) {

    auto start = high_resolution_clock::now();

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to fork process\n";
        std::exit(1);
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        std::perror("execvp");
        std::exit(127); 
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        std::perror("waitpid");
        std::exit(2);
    }

    auto end = high_resolution_clock::now();
    double resulr = duration<double>(end - start).count();

    int exit_code = 0;
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else {
        std::cerr << "Process terminated abnormally\n";
    }

    std::cout << "Command completed with " << exit_code << " exit code and took " << resulr << " seconds.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]\n";
        return 1;
    }

    char** cmd_argv = &argv[1];

    cmd_argv[argc - 1] = nullptr;

    do_command(cmd_argv);

    return 0;
}
