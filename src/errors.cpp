#include "errors.hpp"
#include <print>

void printError(const Error &err) {
  std::println("error in {}", err.command);
  std::println("{}", err.message);
}
