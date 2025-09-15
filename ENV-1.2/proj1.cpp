#include <iostream>
#include <cstdlib>      
#include <stdexcept>   

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " a b c\n";
        return 1;
    }

    try {
        double a = std::stod(argv[1]);
        double b = std::stod(argv[2]);
        double c = std::stod(argv[3]);

        double min = a, max = a;

        if (b < min) min = b;
        if (c < min) min = c;

        if (b > max) max = b;
        if (c > max) max = c;

        std::cout << "min = " << min << ", max = " << max << std::endl;
    }
    catch (const std::invalid_argument&) {
        std::cerr << "Error: One of the inputs is not a valid number.\n";
        return 1;
    }
    catch (const std::out_of_range&) {
        std::cerr << "Error: One of the numbers is out of range.\n";
        return 1;
    }

    return 0;
}
