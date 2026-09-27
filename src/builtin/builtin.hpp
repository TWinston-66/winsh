#pragma once

#include <string>
#include <vector>

enum class BuiltInCommand {
  CD,
  PWD,
  EXIT,
  NONE,
};

BuiltInCommand getBuiltInCmd(const std::string &command);

void intSignal(int sig);

void runBuiltInCmd(BuiltInCommand cmd, const std::vector<std::string> &args);
