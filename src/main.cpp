#include "comm.h"
#include "exe.h"
#include "builtin.h"

// Supported Paths
std::vector<std::string> supported_paths;

// Supported commands
std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands;

// Current path
std::filesystem::path curr_path = std::filesystem::current_path();

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  supported_commands["exit"] = exitFn;
  supported_commands["echo"] = echoFn;
  supported_commands["type"] = typeFn;
  supported_commands["pwd"] = pwdFn;
  supported_commands["cd"] = cdFn;

  processPath();

  // Uncomment this block to pass the first stage
  std::string input;
  std::string command;

  while (true)
  {
    std::cout << "$ ";
    std::getline(std::cin, input);
    std::stringstream ss(input);
    ss >> command;
    if (checkIfSupportedCommand(command))
    {
      executeCommand(input);
    }
    else if (checkIfPathExecutable(command))
    {
      executeExecutable(input);
    }
    else if (checkIfQuotedExecutable(input))
    {
      executeQuotedExecutable(input);
    }
    else
    {
      std::cout << input << ": command not found\n";
    }
  }

  return 0;
}
