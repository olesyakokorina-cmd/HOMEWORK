#include <iostream>
#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <errno.h>      // errno
#include <cstring>      // strerror
#include <cstdlib>      // exit

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Ошибка: укажите исходный и целевой файл.\n";
        std::cerr << "Использование: ./simple-copy <source> <destination>\n";
        exit(EXIT_FAILURE);
    }

    int source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("open source");
        std::cerr << "error code: " << errno << std::endl;
        exit(errno);
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("open destination");
        std::cerr << "error code: " << errno << std::endl;
        close(source_fd);
        exit(errno);
    }

    char buf[1024];
    ssize_t read_bytes;

    while ((read_bytes = read(source_fd, buf, sizeof(buf))) > 0) {
        ssize_t written_bytes = write(dest_fd, buf, read_bytes);
        if (written_bytes != read_bytes) {
            perror("write");
            close(source_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (read_bytes == -1) {
        perror("read");
        close(source_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    close(source_fd);
    close(dest_fd);

    return 0;
}
