#include "builtin/builtin.hpp"
#include "errors.hpp"
#include "prompt.hpp"
#include "timer.hpp"
#include <chrono>
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
using namespace std::chrono;

int main() {
  // ctrl+c needs to stop foreground child not whole shell

  std::string input;
  std::vector<std::string> args;

  fs::path current_dir;

  steady_clock::time_point start;
  steady_clock::time_point end;
  std::string duration;

  while (true) {

    // CWD
    current_dir = fs::current_path();

    // Print prompt
    duration = get_formatted_duration(start, end);
    print_prompt(current_dir, duration);

    // get input
    if (!std::getline(std::cin, input)) {
      exit_shell(0);
    }

    // parse into string vector
    std::stringstream input_stream(input);
    std::string word;
    while (input_stream >> word) {
      args.push_back(word);
    }

    // check for built in
    if (!args.empty()) {
      BuiltinCommand command = get_builtin_command(args.at(0));
      if (command != BuiltinCommand::None) {
        start = get_current_time();
        run_builtin_command(command, args);
      } else {
        // run external
        start = get_current_time();
        pid_t pid = fork();
        if (pid == -1) {
          print_error({args.at(0), "failed to fork process"});
        } else if (pid == 0) {
          std::vector<char *> argv;
          argv.reserve(args.size() + 1);

          for (std::string &arg : args) {
            argv.push_back(arg.data());
          }

          argv.push_back(nullptr);

          execvp(argv[0], argv.data());
          print_error({args.at(0), "failed to replace child process\nDo you "
                                   "have the right command?"});
          exit_shell(1);
        } else {
          int status;
          if (waitpid(pid, &status, 0) == -1) {
            print_error({"waitpid", "failed"});
          }
        }
      }
    }
    end = get_current_time();

    std::println();

    // clear vector + command string
    input = "";
    args.clear();
  }

  return 0;
}
