#pragma once

#include <string>

struct Error {
  std::string command;
  std::string message;
};

void print_error(const Error &err);
