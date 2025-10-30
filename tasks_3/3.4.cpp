#include <iostream>
#include <cstdint>
#include <string>

class IPAddress {
  int32_t val_;

  int32_t StringToInt(std::string&& str) {
    int32_t res = 0;
    for (char c : str) {
      res = res * 10 + (c - '0');
    }
    return res;
  }

public:
  IPAddress(std::string& ip) {
    size_t id_1 = ip.find('.');
    size_t id_2 = ip.find('.', id_1 + 1);
    size_t id_3 = ip.find('.', id_2 + 1);
    size_t id_4 = ip.find('.', id_3 + 1);

    int32_t p1 = StringToInt(ip.substr(0, id_1));
    int32_t p2 = StringToInt(ip.substr(id_1 + 1, id_2 - id_1 - 1));
    int32_t p3 = StringToInt(ip.substr(id_2 + 1, id_3 - id_2 - 1));
    int32_t p4 = StringToInt(ip.substr(id_3 + 1, id_4 - id_3 - 1));

    val_ = p4 + 256 * p3 + 256 * 256 * p2 + 256* 256 * 256 * p1;
  }

  IPAddress(int32_t val) {
    val_ = val;
  }

  int32_t P1() const { return (val_ >> 24) & 255; }
  int32_t P2() const { return (val_ >> 16) & 255; }
  int32_t P3() const { return (val_ >> 8) & 255; }
  int32_t P4() const { return val_ & 255; }
  int32_t GetValue() const { return val_; }

  void PrintBin() const {
    // Представление в двоичной системе
    for (int32_t i = 31; i >= 0; --i) {
      std::cout << (val_ >> i & 1);
      if (i % 8 == 0 and i > 0) {
        std::cout << '.';
      }
    }
    std::cout << '\n';
  }
};

std::ostream& operator<<(std::ostream& s, const IPAddress& ip) {
  s << ip.P1() << "." << ip.P2() << "." << ip.P3() << "." << ip.P4() << '\n';
  return s;
}

std::istream& operator>>(std::istream& s, IPAddress& ip) {
  std::string ip_str;
  s >> ip_str;
  ip = IPAddress(ip_str);
  return s;
}

void IpRange(const IPAddress& ip1, IPAddress& ip2) {
  int32_t max_val = ip1.GetValue();
  int32_t min_val = ip2.GetValue();
  if (max_val < min_val) {
    std::swap(max_val, min_val);
  }
  for (int32_t i = min_val; i <= max_val; ++i) {
    IPAddress ip(i);
    std::cout << ip;
  }
}

int main() {
  std::string ip1 = "192.168.1.13";
  std::string ip2 = "192.168.2.250";
  IPAddress ip_address(ip1);
  IPAddress ip_address2(ip2);
  std::cout << ip_address;
  ip_address.PrintBin();

  IpRange(ip_address2, ip_address);
}