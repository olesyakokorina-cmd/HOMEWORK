#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output-file>\n";
        return 1;
    }

    const char* filepath = argv[1];
    int fd1 = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd1 == -1) {
        perror("open");
        return 2;
    }

    int fd2 = dup(fd1);
    if (fd2 == -1) {
        perror("dup");
        close(fd1);
        return 3;
    }

    const char* line1 = "first line\n";
    const char* line2 = "second line\n";

    if (write(fd1, line1, strlen(line1)) == -1 ||
        write(fd2, line2, strlen(line2)) == -1) {
        perror("write");
        close(fd1);
        close(fd2);
        return 4;
    }

    close(fd1);
    close(fd2);
    return 0;
}
