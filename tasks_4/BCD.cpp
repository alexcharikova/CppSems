#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include "BCD.h"

BCD::BCD(int n, size_t prec) {
  _prec = prec;
  _is_neg = (n < 0);
  int num = std::abs(n);
  assert(num <= 1000000000 and "Слишком большое число");
  _nums = std::vector<uint8_t>();

  while (num > 0) {
    _nums.push_back((num % 10));
    num /= 10;
  }
  std::reverse(_nums.begin(), _nums.end());

  for (size_t i = 0; i < prec; ++i) {
    _nums.push_back(0);
  }
}

BCD::BCD(double n, size_t prec) {
  _is_neg = n < 0;
  _prec = prec;
  double num = std::abs(n);
  assert(num <= 1000000000 and "Слишком большое число");

  // Нормальный ли это способ получать целую часть числа?
  // Плюсы минусы подводные камни
  // Чем это принципиально отличается от floor
  // И почему floor возвращает double...

  int int_num = static_cast<int>(num);
  double fractional = num - int_num;
  _nums = std::vector<uint8_t>();

  while (int_num > 0) {
    _nums.push_back((int_num % 10));
    int_num /= 10;
  }
  std::reverse(_nums.begin(), _nums.end());
  for (size_t i = 0; i < prec; i++) {
    fractional *= 10;
    _nums.push_back(static_cast<int>(fractional) % 10);
    // Мне реально не нравиться эта строчка кода. Но лучше не сочинила.
  }
}

BCD::BCD(std::vector<uint8_t>& n, size_t prec, bool negative) {
  // Этот конструктор нужен для удобства арифметических операций
  _nums = n;
  _prec = prec;
  _is_neg = negative;
}


void BCD::Print() const {
  std::cout << "Binary coded decimal, precision = " << _prec << "  < ";
  size_t point_place = IntDigits();
  if (_is_neg) {
    std::cout << '-';
  }
  for (size_t i = 0; i < _nums.size(); i++) {
    std::cout << (int)_nums[i];
    if (i == point_place - 1) {
      std::cout <<'.';
    }
  }
  std::cout << " >\n";
}

int BCD::Floor() const {
  int res = 0;
  for (int i = 0; i < IntDigits() and i < _nums.size(); ++i) {
    res *= 10;
    res += _nums[i];
  }
  if (_is_neg) {
    res *= -1;
    if (_prec != 0) {
      res -= 1;
    }
  }
  return res;
}

int BCD::Ceil() const {
  int res = Floor();
  if (_prec == 0) {
    return res;
  }
  return res + 1;
}

BCD BCD::operator-() const {
  BCD res = *this;
  if (_nums.empty()) {
    return res;
  }
  res._is_neg = !res._is_neg;
  return res;
}

void BCD::NormalizeVector(std::vector<uint8_t>& output, size_t size, const std::vector<uint8_t>& input) {
  for (int i = 0; i < size; ++i) {
    output.push_back(0);
  }
  for (uint8_t d : input) {
    output.push_back(d);
  }
}

size_t BCD::EquateVectors(std::vector<uint8_t>& a, std::vector<uint8_t>& b) {
  size_t max_len = std::max(a.size(), b.size());
  while (a.size() < max_len) {
    a.push_back(0);
  }
  while (b.size() < max_len) {
    b.push_back(0);
  }
  return max_len;
}

BCD BCD::operator+(const BCD& other) const {
  if (_is_neg != other._is_neg) {
    if (_is_neg) {
      return other - (-(*this));
    }
    return *this - (-other);
  }

  size_t result_prec = std::min(_prec, other._prec) - 1;
  size_t a_int = IntDigits();
  size_t b_int = other.IntDigits();
  size_t max_int = std::max(a_int, b_int);
  std::vector<uint8_t> a_arr, b_arr;

  NormalizeVector(a_arr, max_int - a_int, _nums);
  NormalizeVector(b_arr, max_int - b_int, other._nums);
  size_t max_len = EquateVectors(a_arr, b_arr);
  std::vector<uint8_t> res_vec;
  int over = 0;

  for (size_t i = max_len; i > 0; --i) {
    int sum = a_arr[i - 1] + b_arr[i - 1] + over;
    res_vec.push_back(sum % 10);
    over = sum / 10;
  }

  if (over > 0) {
    res_vec.push_back(over);
    ++max_int;
  }

  std::reverse(res_vec.begin(), res_vec.end());
  size_t res_len = res_vec.size();

  for (size_t i = 0; i < res_len - max_int - result_prec; ++i) {
    res_vec.pop_back();
  }
  return BCD(res_vec, result_prec, _is_neg);
}

BCD BCD::operator-(const BCD& other) const {
  if (_is_neg != other._is_neg) {
    return *this + (-other);
  }

  if (_is_neg and other._is_neg) {
    return (-other) - (-(*this));
  }

  bool result_neg = false;
  BCD abs_this = *this;
  BCD abs_other = other;
  abs_other._is_neg = false;
  abs_this._is_neg = false;

  if (abs_this < abs_other) {
    result_neg = true;
    std::swap(abs_this, abs_other);
  }

  size_t max_prec = std::max(_prec, other._prec);
  size_t result_prec = std::min(_prec, other._prec) - 1;
  size_t a_int = abs_this.IntDigits();
  size_t b_int = abs_other.IntDigits();
  size_t max_int = std::max(a_int, b_int);
  std::vector<uint8_t> a_arr, b_arr;

  // Может для полной красоты стоило это в одну функцию записать, но пока так оставлю
  NormalizeVector(a_arr, max_int - a_int, _nums);
  NormalizeVector(b_arr, max_int - b_int, other._nums);
  size_t max_len = EquateVectors(a_arr, b_arr);

  std::vector<uint8_t> res_vec;
  int borrow = 0;

  for (size_t i = max_len; i > 0; --i) {
    int diff = a_arr[i - 1] - b_arr[i - 1] - borrow;
    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    res_vec.push_back(diff);
  }
  while (res_vec[res_vec.size() - 1] == 0) {
    res_vec.pop_back();
  }
  std::reverse(res_vec.begin(), res_vec.end());
  for (size_t i = 0; i < max_prec - result_prec; ++i) {
    res_vec.pop_back();
  }
  return BCD(res_vec, result_prec, result_neg);
}

BCD BCD::operator*(const BCD& other) const {
  std::vector<int> temp(_nums.size() + other._nums.size(), 0);

  for (size_t i = _nums.size(); i > 0; --i) {
    for (size_t j = other._nums.size(); j > 0; --j) {
      temp[i + j - 1] += _nums[i - 1] * other._nums[j - 1];
    }
  }

  for (size_t i = temp.size() - 1; i > 0; --i) {
    if (temp[i] >= 10) {
      temp[i - 1] += temp[i] / 10;
      temp[i] %= 10;
    }
  }

  std::vector<uint8_t> res_vec;
  for (size_t i = 0; i < temp.size(); ++i) {
    res_vec.push_back(temp[i]);
  }

  while (res_vec.size() > 1 and res_vec[0] == 0) {
    // Очень, очень криво, да. Но нужно удалять ведущие нули из начала.
    res_vec.erase(res_vec.begin());
  }

  size_t total_prec = _prec + other._prec;
  int max_ceil = std::max(std::abs(Ceil()), std::abs(other.Ceil()));
  size_t precision_loss = 1 + static_cast<size_t>(std::log10(max_ceil));
  size_t result_prec = total_prec - precision_loss;
  for (size_t i = 0; i < precision_loss; ++i) {
    res_vec.pop_back();
  }
  // Не очень я тут поняла, честно говоря, как считать новую точность, но если что поправить быстро
  return BCD(res_vec, result_prec, _is_neg != other._is_neg);
}

BCD BCD::Reciprocal(int64_t n, int precision) {
  assert(n != 0);
  if (n == 1) {
    return BCD(1, precision);
  }

  std::vector<uint8_t> result{0};
  int64_t remainder = 1;

  for (int i = 0; i < precision; i++) {
    remainder *= 10;
    int64_t digit = remainder / n;
    result.push_back(digit);
    remainder = remainder % n;
  }
  return BCD(result, precision, false);
}

bool BCD::operator==(const BCD& other) const {
  if (_is_neg != other._is_neg) {
    return false;
  }
  if (IntDigits() != other.IntDigits()) {
    return false;
  }
  size_t min_len = std::min(_nums.size(), other._nums.size());
  for (size_t i = 0; i < min_len; i++) {
    if (_nums[i] != other._nums[i]) {
      return false;
    }
  }
  return true;
}

bool BCD::operator<(const BCD& other) const {
  if (_is_neg != other._is_neg) {
    return _is_neg;
  }

  if (IntDigits() != other.IntDigits()) {
    return (IntDigits() < other.IntDigits()) != _is_neg;
  }

  size_t min_len = std::min(_nums.size(), other._nums.size());
  for (size_t i = 0; i < min_len; i++) {
    if (_nums[i] != other._nums[i]) {
      return (_nums[i] < other._nums[i]) != _is_neg;
    }
  }
  return false;
}

BCD BCD::Exp(int precision) {
  int n = 0;
  int64_t factorial = 1;
  int64_t needed = 3 * pow(10, precision);  // (n+1)! >= 3 * 10^precision
  BCD result(1, precision);

  while (factorial < needed) {
    ++n;
    factorial *= n;

    // Тут происходит костыль. Поскольку сложение убивает точность, приходится искусственно ее повышать.
    result._nums.push_back(0);
    ++result._prec;
    result = result + Reciprocal(factorial, precision + 1);
  }
  return result;
}
