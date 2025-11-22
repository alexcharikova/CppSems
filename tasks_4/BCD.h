#ifndef BCD_H
#define BCD_H
#include <vector>
#include <cstdint>

class BCD {
  std::vector<uint8_t> _nums;
  size_t _prec;
  bool _is_neg;
  size_t IntDigits() const { return _nums.size() - _prec; }
  // Прибавляет к вектору ведущие нули в количестве size
  static void NormalizeVector(std::vector<uint8_t>& input, size_t size, const std::vector<uint8_t>& output);
  // Делает векторы одного размера, прибавляя незначащие нули при надобности
  static size_t EquateVectors(std::vector<uint8_t>& a, std::vector<uint8_t>& b);

public:
  BCD(int n, size_t prec);
  BCD(double n, size_t prec);
  BCD(std::vector<uint8_t>& n, size_t prec, bool negative);
  BCD() = default;

  BCD(BCD&& other) = default;
  BCD& operator=(BCD&& other) = default;
  BCD(const BCD& other) = default;
  BCD& operator=(const BCD& other) = default;

  void Print() const;

  BCD operator-() const;
  BCD operator+(const BCD& other) const;
  BCD operator-(const BCD& other) const;
  BCD operator*(const BCD& other) const;

  bool operator==(const BCD& other) const;
  bool operator<(const BCD& other) const;
  bool operator>(const BCD& other) const {
    return other < *this;
  }
  bool operator<=(const BCD& other) const {
    return !(other < *this);
  }
  bool operator>=(const BCD& other) const {
    return !(*this < other);
  }
  bool operator!=(const BCD& other) const {
    return !(*this == other);
  }

  int Precision() const { return _prec; }
  int Floor() const;
  int Ceil() const;
  static BCD Reciprocal(int64_t n, int precision);
  static BCD Exp(int precision);
};

#endif //BCD_H
