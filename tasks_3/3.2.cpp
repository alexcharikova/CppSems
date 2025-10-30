#include <iostream>
#include <chrono>
#include <string>
#include <ctime>
#include <list>
#include <source_location>
#include <format>


class Tracer {
  struct Message {
    std::string message;
    time_t timestamp;
    std::source_location src;

    void Print() {
      std::cout << "-------"
                << std::ctime(&timestamp)
                << message
                << std::endl;
    }
  };

  static inline std::list<Message> st_;
  std::source_location src_;

public:
  Tracer(std::source_location s = std::source_location::current()) {
    src_ = s;
    std::string str = std::format("Tracer started in: \nFunction: {} \nFile: {}\nLine: {}\nColumn: {}",
      src_.function_name(),
      src_.file_name(),
      src_.line(),
      src_.column());
    Log(str);
  }

  ~Tracer() {
    std::string str = std::format("Tracer ended in: \nFunction: {} \nFile: {}\nLine: {}\nColumn: {}",
      src_.function_name(),
      src_.file_name(),
      src_.line(),
      src_.column());
    Log(str);
  }

  static time_t GetTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return time;
  }

  static void Log(const std::string& message) {
    Message msg = {message, GetTime()};
    st_.push_back(msg);
    msg.Print();
  }

  static void CallStack() {
    std::cout << "\n------------ Stack trace ------------" << std::endl;
    for (auto iter = st_.begin(); iter != st_.end(); iter++) {
      iter->Print();
    }
    std::cout << std::endl;
  }
};

void foo() {
  Tracer tracer1;
  Tracer::Log("Message 3");
}

int main() {
  Tracer tracer;
  Tracer::Log("Message 1");
  Tracer::Log("Message 2");

  foo();

  Tracer::CallStack();
}