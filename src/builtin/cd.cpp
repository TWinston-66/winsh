#include "cd.hpp"
#include "../errors.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

namespace fs = std::filesystem;

void printwd() {
  std::error_code ec;
  fs::path pwd = fs::current_path(ec);
  if (ec) {
    printError({"pwd", "could not get current directory: " + ec.message()});
    return;
  }
  std::cout << pwd.c_str() << '\n';
}

int cd(const std::vector<std::string> &args) {
  if (args.size() > 2) {
    printError({"cd", "too many arguments"});
    return 1;
  }

  const char *home = std::getenv("HOME");
  const char *old_pwd_env = std::getenv("OLDPWD");

  std::error_code ec;
  fs::path old_pwd = fs::current_path(ec);

  if (ec) {
    printError({"cd", "could not get current directory: " + ec.message()});
    return 1;
  }

  fs::path target;

  if (args.size() == 1) {
    if (!home) {
      printError({"cd", "HOME not set"});
      return 1;
    }

    target = home;
  } else if (args[1] == "-") {
    if (!old_pwd_env) {
      printError({"cd", "OLDPWD not set"});
      return 1;
    }

    target = old_pwd_env;
  } else if ((args[1] == "~" || args[1].starts_with("~/")) && home) {
    target = fs::path(home);
    if (args[1].size() > 2) {
      target /= args[1].substr(2);
    }
  } else {
    target = args[1];
  }

  fs::current_path(target, ec);

  if (ec) {
    printError({"cd", "could not change directory: " + ec.message()});
    return 1;
  }

  fs::path new_pwd = fs::current_path(ec);

  if (ec) {
    printError({"cd", "could not get new current directory: " + ec.message()});
    return 1;
  }

  if (setenv("OLDPWD", old_pwd.c_str(), 1) != 0) {
    printError({"cd", "could not set OLDPWD"});
    return 1;
  }

  if (setenv("PWD", new_pwd.c_str(), 1) != 0) {
    printError({"cd", "could not set PWD"});
    return 1;
  }

  if (args.size() == 2 && args[1] == "-") {
    std::cout << new_pwd.string() << '\n';
  }

  return 0;
}
