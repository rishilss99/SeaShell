#include <iostream>
#include <unordered_map>
#include <sstream>
#include <functional>

void exitFn(std::stringstream &ss)
{
  int ret_val;
  ss >> ret_val;
  std::exit(ret_val);
}

void echoFn(std::stringstream &ss)
{
  std::string input_str;
  while (ss >> input_str)
  {
    std::cout << input_str << " ";
  }
  std::cout << "\n";
}

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Supported commands
  std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands{{"exit", exitFn}, {"echo", echoFn}};

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
      supported_commands[command](ss);
    }
  }

  return 0;
}
