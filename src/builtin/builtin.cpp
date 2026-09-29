#include "builtin.hpp"
#include "../errors.hpp"
#include "cd.hpp"
#include <cstdlib>
#include <string>
#include <vector>

BuiltinCommand get_builtin_command(const std::string &command) {
  if (command == "cd")
    return BuiltinCommand::Cd;
  if (command == "pwd")
    return BuiltinCommand::Pwd;
  if (command == "exit")
    return BuiltinCommand::Exit;

  return BuiltinCommand::None;
}

void exit_shell(int status) {
  // std::println("\n\nexiting...");
  exit(status);
}

void run_builtin_command(BuiltinCommand command,
                         const std::vector<std::string> &args) {
  switch (command) {
  case BuiltinCommand::Exit:
    exit_shell(0);
    return;
  case BuiltinCommand::Cd:
    cd(args);
    return;
  case BuiltinCommand::Pwd:
    print_pwd();
    return;
  case BuiltinCommand::None:
    print_error({"builtin", "could not find builtin"});
  }
}
