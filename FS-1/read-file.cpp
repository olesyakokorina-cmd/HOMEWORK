#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        std::cout << "error code " << errno << std::endl;
        exit(errno);
    }

    char buf[1024];
    ssize_t read_bytes;
    while ((read_bytes = read(fd, buf, sizeof(buf))) > 0) {
        ssize_t written_bytes = write(STDOUT_FILENO, buf, read_bytes);
        if (written_bytes == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (read_bytes == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}
