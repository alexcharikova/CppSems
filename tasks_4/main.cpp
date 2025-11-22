#include "tasks_4/BCD.h"

int main() {
  BCD a(3.14, 4);
  BCD b(12, 2);

  a.Print();
  b.Print();

  BCD c = b + a;
  c.Print();
  c = b - a;
  c.Print();
  c = b * a;
  c.Print();

  c = BCD::Reciprocal(2, 3);
  c.Print();

  c = BCD::Exp(10);
  c.Print();
}