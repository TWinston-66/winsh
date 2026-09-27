#include "builtin/builtin.hpp"
#include "errors.hpp"
#include <filesystem>
#include <iostream>
#include <print>
#include <pwd.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

void print_prompt(const fs::path &cwd) {
  char host[256];
  gethostname(host, sizeof host);
  std::string h(host);
  h = h.substr(0, h.find('.'));
  const std::string user = getpwuid(geteuid())->pw_name;

  std::string home = getenv("HOME") ? getenv("HOME") : "";
  std::string dir = cwd == home  ? "~"
                    : cwd == "/" ? "/"
                                 : cwd.filename().string();

  std::print("{}@{} {}{}", user, h, dir, geteuid() == 0 ? " # " : " % ");
  std::fflush(stdout);
}

int main() {

  signal(SIGINT, intSignal);

  std::string input;
  std::vector<std::string> args;

  fs::path current_dir;
  while (true) {

    // CWD
    current_dir = fs::current_path();

    // Print prompt
    print_prompt(current_dir);

    // get input
    if (!std::getline(std::cin, input)) {
      intSignal(0);
    }

    // parse into string vector
    std::stringstream input_stream(input);
    std::string word;
    while (input_stream >> word) {
      args.push_back(word);
    }

    // check for built in
    if (!args.empty()) {
      BuiltInCommand cmd = getBuiltInCmd(args.at(0));
      if (cmd != BuiltInCommand::NONE) {
        runBuiltInCmd(cmd, args);
      } else {
        // run external
        pid_t pid = fork();
        if (pid == -1) {
          printError({args.at(0), "failed to fork process"});
        } else if (pid == 0) {
          std::vector<char *> argv;
          argv.reserve(args.size() + 1);

          for (std::string &arg : args) {
            argv.push_back(arg.data());
          }

          argv.push_back(nullptr);

          execvp(argv[0], argv.data());
          printError({args.at(0), "failed to replace process"});
          intSignal(1);
        } else {
          int status;
          if (waitpid(pid, &status, 0) == -1) {
            printError({"waitpid", "failed"});
          }
        }
      }
    }

    // clear vector + command string
    input = "";
    args.clear();
  }

  return 0;
}
