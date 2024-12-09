#include <iostream>
#include <unordered_set>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Supported commands
  std::unordered_set<std::string> supported_commands;

  // Uncomment this block to pass the first stage
  std::cout << "$ ";
  
  std::string input;
  std::getline(std::cin, input);

  while(supported_commands.find(input) == supported_commands.end())
  {
    std::cout << input << ": command not found\n";
  }

  return 0;
}
