#include "builtin/builtin.hpp"
#include "errors.hpp"
#include "prompt.hpp"
#include <filesystem>
#include <iostream>
#include <print>
#include <pwd.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace fs = std::filesystem;

int main() {
  // ctrl+c needs to stop foreground child not whole shell
  // signal(SIGINT, intSignal);

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

    std::println();

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
          printError({args.at(0), "failed to replace child process\nDo you "
                                  "have the right command?"});
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
