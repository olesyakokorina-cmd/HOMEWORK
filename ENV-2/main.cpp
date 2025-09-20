#include <iostream>
#include <vector>
#include "Complex.h"
#include "sort_complex.h"

int main() {
    std::vector<Complex> arr = {
        Complex(3, 4),    // abs = 5
        Complex(1, -1),   // abs = sqrt(2)
        Complex(0, 0),    // abs = 0
        Complex(-2, 2),   // abs = sqrt(8)
        Complex(5, 12),   // abs = 13
        Complex(2, 0)     // abs = 2
    };

    std::cout << "Before sorting:\n";
    for (const auto& c : arr) {
        std::cout << c << "  | abs = " << c.abs() << '\n';
    }

    sortByMagnitude(arr);

    std::cout << "\nAfter sorting by magnitude (ascending):\n";
    for (const auto& c : arr) {
        std::cout << c << "  | abs = " << c.abs() << '\n';
    }

    Complex a(1, 2), b(3, -1);
    std::cout << "\nDemo of operations:\n";
    std::cout << a << " + " << b << " = " << (a + b) << '\n';
    std::cout << a << " - " << b << " = " << (a - b) << '\n';
    std::cout << a << " * 2.5 = " << (a * 2.5) << '\n';
    std::cout << "abs(" << b << ") = " << b.abs() << '\n';

    return 0;
}
