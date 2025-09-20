#include "Complex.h"
#include <cmath>
#include <iomanip>

Complex::Complex(double real, double imag) : re(real), im(imag) {}

double Complex::real() const { return re; }
double Complex::imag() const { return im; }

Complex Complex::operator+(const Complex& other) const {
    return Complex(re + other.re, im + other.im);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(re - other.re, im - other.im);
}

Complex Complex::operator*(double scalar) const {
    return Complex(re * scalar, im * scalar);
}

Complex operator*(double scalar, const Complex& c) {
    return Complex(c.re * scalar, c.im * scalar);
}

double Complex::abs() const {
    // hypot более точен для больших/малых значений
    return std::hypot(re, im);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re;
    if (c.im >= 0) os << " + " << c.im << "i";
    else            os << " - " << std::abs(c.im) << "i";
    return os;
}
