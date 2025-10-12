#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <cerrno>

constexpr size_t BUF_SIZE = 4096;

bool is_hole(const char* buf, ssize_t size) {
    for (ssize_t i = 0; i < size; ++i) {
        if (buf[i] != 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " source-file destination-file\n";
        return 1;
    }

    const char* src_path = argv[1];
    const char* dst_path = argv[2];

    int src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        std::cerr << "Error opening source file: " << strerror(errno) << "\n";
        return 2;
    }

    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        std::cerr << "Error opening destination file: " << strerror(errno) << "\n";
        close(src_fd);
        return 3;
    }

    char buffer[BUF_SIZE];
    ssize_t bytes_read;
    size_t total_bytes = 0;
    size_t data_bytes = 0;
    size_t hole_bytes = 0;

    while ((bytes_read = read(src_fd, buffer, BUF_SIZE)) > 0) {
        total_bytes += bytes_read;

        if (is_hole(buffer, bytes_read)) {
            if (lseek(dst_fd, bytes_read, SEEK_CUR) == (off_t)-1) {
                std::cerr << "Error during lseek on destination file: " << strerror(errno) << "\n";
                close(src_fd);
                close(dst_fd);
                return 4;
            }
            hole_bytes += bytes_read;
        } else {
            ssize_t written = write(dst_fd, buffer, bytes_read);
            if (written != bytes_read) {
                std::cerr << "Error writing to destination file: " << strerror(errno) << "\n";
                close(src_fd);
                close(dst_fd);
                return 5;
            }
            data_bytes += written;
        }
    }

    if (bytes_read == -1) {
        std::cerr << "Error reading source file: " << strerror(errno) << "\n";
        close(src_fd);
        close(dst_fd);
        return 6;
    }

    close(src_fd);
    close(dst_fd);

    std::cout << "Successfully copied " << total_bytes << " bytes (data: " << data_bytes << ", hole: " << hole_bytes << ").\n";

    return 0;
}
