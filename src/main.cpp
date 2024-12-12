#include "comm.h"
#include "exe.h"
#include "builtin.h"

// Supported Paths
std::vector<std::string> supported_paths;

// Supported commands
std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands;

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  supported_commands["exit"] = exitFn;
  supported_commands["echo"] = echoFn;
  supported_commands["type"] = typeFn;

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
    else
    {
      std::cout << input << ": command not found\n";
    }
  }

  return 0;
}
