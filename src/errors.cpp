#include "errors.hpp"
#include <print>

void print_error(const Error &err) {
  std::println("error in {}", err.command);
  std::println("{}", err.message);
}
