#include "prompt.hpp"
#include "builtin/builtin.hpp"
#include "errors.hpp"
#include <filesystem>
#include <iostream>
#include <pwd.h>
#include <string>
#include <string_view>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace {
constexpr std::string_view reset = "\033[0m";
constexpr std::string_view red = "\033[31m";
constexpr std::string_view green = "\033[32m";
constexpr std::string_view yellow = "\033[33m";
constexpr std::string_view blue = "\033[34m";
} // namespace

void print_prompt(const fs::path &cwd) {
  char host[256];
  gethostname(host, sizeof(host));

  std::string hostname(host);
  hostname = hostname.substr(0, hostname.find('.'));

  passwd *user_info = getpwuid(geteuid());
  if (user_info == nullptr) {
    print_error({"prompt", "failed to get user info"});
    exit_shell(1);
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

  std::cout << blue << hostname << reset;
  std::cout << " in ";
  std::cout << yellow << dir << reset << '\n';

  if (geteuid() == 0) {
    std::cout << red << "❯" << reset << " " << reset;
  } else {
    std::cout << green << "❯" << reset << " " << reset;
  }
  std::cout << reset << std::flush;
}
