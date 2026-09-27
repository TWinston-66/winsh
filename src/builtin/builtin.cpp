#include "builtin.hpp"
#include "../errors.hpp"
#include "cd.hpp"
#include <cstdlib>
#include <print>
#include <string>
#include <vector>

BuiltInCommand getBuiltInCmd(const std::string &command) {
  if (command == "cd")
    return BuiltInCommand::CD;
  if (command == "pwd")
    return BuiltInCommand::PWD;
  if (command == "exit")
    return BuiltInCommand::EXIT;

  return BuiltInCommand::NONE;
}

void intSignal(int sig) {
  std::println("\n\nexiting...");
  exit(sig);
}

void runBuiltInCmd(BuiltInCommand cmd, const std::vector<std::string> &args) {
  switch (cmd) {
  case BuiltInCommand::EXIT:
    intSignal(0);
    return;
  case BuiltInCommand::CD:
    cd(args);
    return;
  case BuiltInCommand::PWD:
    printwd();
    return;
  case BuiltInCommand::NONE:
    printError({"builtin", "could not find builtin"});
  }
}
