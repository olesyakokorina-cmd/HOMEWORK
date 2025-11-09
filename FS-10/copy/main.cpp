#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

void print_usage() {
    std::cout << "Usage: ./copy <source-file> <destination-file>\n";
}

bool copy_file_with_holes(const std::string& source_path, const std::string& dest_path, off_t& total_data, off_t& total_holes) {
    // Open source file for reading
    int source_fd = open(source_path.c_str(), O_RDONLY);
    if (source_fd == -1) {
        std::cerr << "Error opening source file: " << strerror(errno) << std::endl;
        return false;
    }

    int dest_fd = open(dest_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        std::cerr << "Error opening destination file: " << strerror(errno) << std::endl;
        close(source_fd);
        return false;
    }

    char buffer[4096];
    off_t bytes_copied = 0;
    total_data = 0;
    total_holes = 0;

    ssize_t bytes_read;
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        bytes_copied += bytes_read;
        total_data += bytes_read;
        if (bytes_read < static_cast<ssize_t>(sizeof(buffer))) {
   		 total_holes += sizeof(buffer) - bytes_read;
	}


        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            std::cerr << "Error writing to destination file: " << strerror(errno) << std::endl;
            close(source_fd);
            close(dest_fd);
            return false;
        }
    }

    if (bytes_read == -1) {
        std::cerr << "Error reading source file: " << strerror(errno) << std::endl;
    }

    close(source_fd);
    close(dest_fd);

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage();
        return 1;
    }

    std::string source_path = argv[1];
    std::string dest_path = argv[2];

    off_t total_data = 0;
    off_t total_holes = 0;

    if (copy_file_with_holes(source_path, dest_path, total_data, total_holes)) {
        std::cout << "Successfully copied " << total_data + total_holes << " bytes (data: " 
                  << total_data << ", hole: " << total_holes << ").\n";
    } else {
        std::cerr << "File copy failed.\n";
        return 1;
    }

    return 0;
}

