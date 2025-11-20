#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include <string>

class Operation {
public:
  virtual void operator()(std::stack<double>& stack) const = 0;
};

class UnaryOperation : public Operation {
  double (*func)(double);

 public:
  UnaryOperation(double (*f)(double)) {
    func = f;
  }

  void operator()(std::stack<double>& stack) const override {
    assert(!stack.empty() and "Недостаточно операндов (требуется 1)");
    double a = stack.top();
    stack.pop();
    stack.push(func(a));
  }
};

class BinaryOperation : public Operation {
  double (*func)(double, double);

 public:
  BinaryOperation(double (*f)(double, double)) {
    func = f;
  }

  void operator()(std::stack<double>& stack) const override {
    assert(stack.size() >= 2 and "Недостаточно операндов (требуется 2)");
    double a = stack.top();
    stack.pop();
    double b = stack.top();
    stack.pop();
    stack.push(func(a, b));
  }
};

class TernaryOperation : public Operation {
  double (*func)(double, double, double);

 public:
  TernaryOperation(double (*f)(double, double, double)) {
    func = f;
  }

  void operator()(std::stack<double>& stack) const override {
    assert(stack.size() >= 3 and "Недостаточно операндов (требуется 3)");
    double a = stack.top();
    stack.pop();
    double b = stack.top();
    stack.pop();
    double c = stack.top();
    stack.pop();
    stack.push(func(a, b, c));
  }
};

class Calculator {
  std::stack<double> stack;
  std::map<std::string, Operation*> operations;

  void BuildCalculator() {
    operations["+"] = new BinaryOperation([](double a, double b) { return a + b; });
    operations["-"] = new BinaryOperation([](double a, double b) { return a - b; });
    operations["*"] = new BinaryOperation([](double a, double b) { return a * b; });
    operations["/"] = new BinaryOperation([](double a, double b) {
      assert(b != 0 and "Division by zero!");
      return a / b;
    });
    operations["sin"] = new UnaryOperation([](double a) { return std::sin(a); });
    operations["cos"] = new UnaryOperation([](double a) { return std::cos(a); });
    operations["tg"] = new UnaryOperation([](double a) { return std::tan(a); });
    operations["ctg"] = new UnaryOperation([](double a) { return 1 / std::tan(a); });
    operations["exp"] = new UnaryOperation([](double a) { return std::exp(a); });
    operations["log"] = new UnaryOperation([](double a) { return std::log(a); });
    operations["sqrt"] = new UnaryOperation([](double a) { return std::sqrt(a); });

    operations["pow"] = new BinaryOperation([](double a, double b) { return std::pow(a, b); });
    operations["atan2"] = new BinaryOperation([](double a, double b) { return std::atan2(a, b); });

    operations["median"] = new TernaryOperation([](double a, double b, double c) {
      return (a + b + c) / 3;
    });
  }

  bool IsNum(const std::string& value) {
    try {
      std::stod(value);
      return true;
    } catch (...) {
      return false;
    }
  }

 public:
  Calculator() {
    BuildCalculator();
  }

  void Execute(const std::string& command) {
    assert(operations.contains(command) and "No such operation!");
    (*operations[command])(stack);
  }

  void AddValue(double value) {
    stack.push(value);
  }

  void DoSmth(const std::string& commands) {
    if (IsNum(commands)) {
      AddValue(std::stod(commands));
    } else {
      Execute(commands);
    }
  }

  void StopCalculation() {
    assert(stack.empty() and "Stack is not empty!");
  }

  void PrintResult() {
    if (stack.empty()) {
      return;
    }
    std::cout << stack.top() << std::endl;
  }

  ~Calculator() {
    assert(stack.size() == 1 and "Stack is not empty!");

    for (std::pair<std::string, Operation*> op : operations) {
      delete op.second;
    }
  }
};

int main() {
  Calculator c;
  std::string command;
  while (std::cin >> command) {
    if (command == "---") {
      break;
    }
    c.DoSmth(command);
    c.PrintResult();
  }
}