#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

std::vector<std::string> splitCommands(const std::string& input) {
    std::vector<std::string> commands;
    std::string current;
    
    for (size_t i = 0; i < input.length(); i++) {
        if (input[i] == ';') {
            if (!current.empty()) {
                commands.push_back(current);
                current.clear();
            }
        }
        else if (i + 1 < input.length() && input[i] == '&' && input[i + 1] == '&') {
            if (!current.empty()) {
                commands.push_back(current + " &&");
                current.clear();
            }
            i++;
        }
        else if (i + 1 < input.length() && input[i] == '|' && input[i + 1] == '|') {
            if (!current.empty()) {
                commands.push_back(current + " ||");
                current.clear();
            }
            i++;
        }
        else {
            current += input[i];
        }
    }
    
    if (!current.empty()) {
        commands.push_back(current);
    }
    
    return commands;
}

bool parseCommand(const std::string& cmd, std::vector<std::string>& args, 
                  std::string& outputFile, bool& append, bool& silent) {
    std::istringstream iss(cmd);
    std::string token;
    bool foundRedirect = false;
    
    silent = false;
    append = false;
    outputFile.clear();
    
    while (iss >> token) {
        if (token == "silent" && args.empty()) {
            silent = true;
        }
        else if (token == ">") {
            foundRedirect = true;
            append = false;
        }
        else if (token == ">>") {
            foundRedirect = true;
            append = true;
        }
        else if (foundRedirect) {
            outputFile = token;
            foundRedirect = false;
        }
        else {
            args.push_back(token);
        }
    }
    
    return !args.empty();
}

bool executeCommand(const std::vector<std::string>& args, const std::string& outputFile, bool append, bool silent) {
    pid_t pid = fork();
    
    if (pid == -1) {
        std::cerr << "Error: fork failed\n";
        return false;
    }
    
    if (pid == 0) {
        std::string path = getenv("PATH");
        path = "." + std::string(":") + path;
        setenv("PATH", path.c_str(), 1);
        
        if (!outputFile.empty()) {
            int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            int fd = open(outputFile.c_str(), flags, 0644);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
        else if (silent) {
            std::string filename = std::to_string(getpid()) + ".log";
            int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }
        
        std::vector<char*> c_args;
        for (size_t i = 0; i < args.size(); i++) {
            c_args.push_back(const_cast<char*>(args[i].c_str()));
        }
        c_args.push_back(NULL);
        
        execvp(c_args[0], c_args.data());
        
        std::cerr << "Command not found: " << args[0] << std::endl;
        exit(1);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

int main() {
    std::string input;
    
    std::cout << "Simple Interactive Shell. Type 'exit' to quit.\n";
    
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, input);
        
        if (input == "exit") {
            break;
        }
        if (input.empty()) {
            continue;
        }
        
        std::vector<std::string> commands = splitCommands(input);
        bool lastSuccess = true;
        
        for (size_t i = 0; i < commands.size(); i++) {
            std::vector<std::string> args;
            std::string outputFile;
            bool append, silent;
            
            bool isAnd = (commands[i].find(" &&") != std::string::npos);
            bool isOr = (commands[i].find(" ||") != std::string::npos);
            
            std::string cleanCmd = commands[i];
            if (isAnd) cleanCmd = cleanCmd.substr(0, cleanCmd.length() - 3);
            if (isOr) cleanCmd = cleanCmd.substr(0, cleanCmd.length() - 3);
            
            if (!parseCommand(cleanCmd, args, outputFile, append, silent)) {
                continue;
            }
            
            if (isAnd && !lastSuccess) {
                continue;
            }
            if (isOr && lastSuccess) {
                continue;
            }
            
            lastSuccess = executeCommand(args, outputFile, append, silent);
        }
    }
    
    std::cout << "Goodbye!\n";
    return 0;
}
