#include <iostream>
#include <cmath>

class Rational {
  int gcd(int a, int b) const {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
      int c = b;
      b = a % b;
      a = c;
    }
    return a;
  }

  void reduce() {
    int a = gcd(numerator, denominator);
    numerator /= a;
    denominator /= a;
  }

 public:
  int numerator;
  int denominator;

  Rational(int n, int d = 1) {
    numerator = n;
    denominator = d;
  }

  operator double() const {
    return static_cast<double>(numerator) / denominator;
  }

  Rational& operator+=(const Rational& other) {
    numerator = numerator * other.denominator + other.numerator * denominator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }

  Rational& operator-=(const Rational& other) {
    numerator = numerator * other.denominator - other.numerator * denominator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }

  Rational& operator*=(const Rational& other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    reduce();
    return *this;
  }

  Rational& operator/=(const Rational& other) {
    numerator *= other.denominator;
    denominator *= other.numerator;
    reduce();
    return *this;
  }

  Rational& operator++() {
    numerator += denominator;
    reduce();
    return *this;
  }

  Rational& operator--() {
    numerator -= denominator;
    reduce();
    return *this;
  }

  Rational operator++(int) {
    Rational tmp = *this;
    numerator += denominator;
    reduce();
    return tmp;
  }

  Rational operator--(int) {
    Rational tmp = *this;
    numerator -= denominator;
    reduce();
    return tmp;
  }

  Rational operator+(const Rational& other) const {
    Rational result = Rational(numerator, denominator);
    result += other;
    return result;
  }

  Rational operator-(const Rational& other) const {
    Rational result = Rational(numerator, denominator);
    result -= other;
    return result;
  }

  Rational operator*(const Rational& other) const {
    Rational result = Rational(numerator, denominator);
    result *= other;
    return result;
  }

  Rational operator/(const Rational& other) const {
    Rational result = Rational(numerator, denominator);
    result /= other;
    return result;
  }

  bool operator==(const Rational& other) const {
    return numerator == other.numerator and denominator == other.denominator;
  }

  bool operator!=(const Rational& other) const {
    return numerator != other.numerator or denominator != other.denominator;
  }

  bool operator<(const Rational& other) const {
    return numerator * other.denominator < other.numerator * denominator;
  }

  bool operator>(const Rational& other) const {
    return numerator * other.denominator > other.numerator * denominator;
  }

  bool operator<=(const Rational& other) const {
    return numerator * other.denominator <= other.numerator * denominator;
  }

  bool operator>=(const Rational& other) const {
    return numerator * other.denominator >= other.numerator * denominator;
  }
};

std::ostream& operator<<(std::ostream& s, const Rational& num) {
  s << num.numerator << " / " << num.denominator;
  return s;
}

std::istream& operator>>(std::istream& s, Rational& num) {
  s >> num.numerator >> num.denominator;
  return s;
}

int main() {
  Rational r(1, 2);
  std::cout << r << std::endl;
  std::cin >> r;
  std::cout << r << std::endl;
  std::cout << ++r << std::endl;
  std::cout << r++ << std::endl;
  std::cout << r << std::endl;
  std::cout << r-- << std::endl;
  std::cout << --r << std::endl;

  Rational r2(3, 4);
  std::cout << r2 + r << std::endl;
  std::cout << r - r2 << std::endl;
  std::cout << r * r2 << std::endl;
  std::cout << r / r2 << std::endl;

  r += r2;
  std::cout << r << std::endl;
  r -= r2;
  std::cout << r << std::endl;
  r *= r2;
  std::cout << r << std::endl;
  r /= r2;
  std::cout << r << std::endl;
}