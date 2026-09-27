#include "prompt.hpp"
#include "builtin/builtin.hpp"
#include "errors.hpp"
#include <filesystem>
#include <iostream>
#include <print>
#include <pwd.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

void print_prompt(const fs::path &cwd) {
  char host[256];
  gethostname(host, sizeof(host));

  std::string h(host);
  h = h.substr(0, h.find('.'));

  passwd *user_info = getpwuid(geteuid());
  if (user_info == nullptr) {
    printError({"prompt", "failed to get user info"});
    intSignal(1);
  }
  const std::string user = getpwuid(geteuid())->pw_name;

  const char *home_env = getenv("HOME");
  std::string home;

  if (home_env != nullptr) {
    home = home_env;
  }

  std::string dir;

  if (cwd == home) {
    dir = "~";
  } else if (cwd == "/") {
    dir = "/";
  } else {
    dir = cwd.filename().string();
  }

  std::string prompt_symbol;

  if (geteuid() == 0) {
    prompt_symbol = " # ";
  } else {
    prompt_symbol = " % ";
  }

  std::print("\n{}@{} {}{}", user, h, dir, prompt_symbol);
  std::cout << std::flush;
}
