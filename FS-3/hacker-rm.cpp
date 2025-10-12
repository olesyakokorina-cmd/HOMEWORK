#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    const char *path = argv[1];

    struct stat st;
    if (stat(path, &st) != 0) {
        std::cerr << "stat failed: " << std::strerror(errno) << "\n";
        return 2;
    }

    if (!S_ISREG(st.st_mode)) {
        std::cerr << "Not a regular file\n";
        return 3;
    }

    off_t size = st.st_size;
    if (size == 0) {
        if (unlink(path) != 0) {
            std::cerr << "unlink failed: " << std::strerror(errno) << "\n";
            return 4;
        }
        std::cout << "Removed empty file\n";
        return 0;
    }

    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        std::cerr << "open failed: " << std::strerror(errno) << "\n";
        return 5;
    }

    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        std::cerr << "lseek failed: " << std::strerror(errno) << "\n";
        close(fd);
        return 6;
    }

    const size_t BUF = 4096;
    char buf[BUF];
    std::memset(buf, 0, sizeof(buf));

    off_t remaining = size;
    while (remaining > 0) {
        ssize_t to_write = (remaining > (off_t)BUF) ? BUF : (ssize_t)remaining;
        ssize_t written = write(fd, buf, to_write);
        if (written <= 0) {
            if (written == -1)
                std::cerr << "write error: " << std::strerror(errno) << "\n";
            else
                std::cerr << "write returned 0\n";
            close(fd);
            return 7;
        }
        remaining -= written;
    }

    fsync(fd);
    close(fd);

    if (unlink(path) != 0) {
        std::cerr << "unlink failed: " << std::strerror(errno) << "\n";
        return 8;
    }

    std::cout << "File overwritten with zeros and removed\n";
    return 0;
}
