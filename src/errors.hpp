#pragma once

#include <string>

struct Error {
  std::string command;
  std::string message;
};

void printError(const Error &err);
