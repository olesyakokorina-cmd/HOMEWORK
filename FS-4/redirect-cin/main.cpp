#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

void initialize(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        std::exit(1);  // Ошибка: не указан файл
    }

    const char* filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        std::perror("Error opening file");
        std::exit(2);  // Ошибка: не удалось открыть файл
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
        std::perror("Error redirecting stdin");
        std::exit(3);  // Ошибка: не удалось перенаправить ввод
    }

    close(fd); 
}

int main(int argc, char** argv)
{
    initialize(argc, argv);

    // read the string
    std::string input;
    std::cin >> input;

    // logic to get the reversed string
    std::string reversed(input.rbegin(), input.rend());

    // print to the console
    std::cout << reversed << std::endl;

    return 0;
}
