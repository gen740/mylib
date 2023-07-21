#include <coroutine/generator.hpp>
#include <iostream>
#include <ranges>

Mylib::Coroutine::Generator<int> foo() {
  for (int i = 0; i < 1; i++) {
    co_yield i;
  }
}

int main() {
  for (auto&& i : foo()) {
    std::cout << i << std::endl;
  }
  return 0;
}
