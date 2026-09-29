#pragma once

#include <string>
#include <vector>

enum class BuiltinCommand {
  Cd,
  Pwd,
  Exit,
  None,
};

BuiltinCommand get_builtin_command(const std::string &command);

void exit_shell(int status);

void run_builtin_command(BuiltinCommand command,
                         const std::vector<std::string> &args);
