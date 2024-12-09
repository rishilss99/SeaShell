#include <iostream>
#include <unordered_set>
#include <sstream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Supported commands
  std::unordered_set<std::string> supported_commands{"exit"};

  // Uncomment this block to pass the first stage
  std::string input;
  std::string command;

  while (true)
  {
    std::cout << "$ ";
    std::getline(std::cin, input);
    std::stringstream ss(input);
    ss >> command;
    if (supported_commands.find(command) == supported_commands.end())
    {
      std::cout << input << ": command not found\n";
    }
    else
    {
      break;
    }
  }

  return 0;
}
