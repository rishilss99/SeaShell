#include "builtin.h"
#include "exe.h"

extern std::vector<std::string> supported_paths;

extern std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands;


void exitFn(std::stringstream &ss)
{
  int ret_val;
  ss >> ret_val;
  std::exit(ret_val);
}

void echoFn(std::stringstream &ss)
{
  std::string input_str;
  getline(ss, input_str);
  std::cout << input_str << "\n";
}

void typeFn(std::stringstream &ss)
{
  std::string input_command;
  ss >> input_command;
  
  if (supported_commands.find(input_command) != supported_commands.end())
  {
    std::cout << input_command << " is a shell builtin\n";
    return;
  }
  
  for(auto path: supported_paths)
  {
    path += "/" + input_command;
    if (std::filesystem::exists(path))
    {
      std::cout << input_command << " is " << path << "\n";
      return;
    }
  }

  std::cout << input_command << ": not found\n";
}

bool checkIfSupportedCommand(std::string &command)
{
    return (supported_commands.find(command) != supported_commands.end());
}

void executeCommand(std::string &input)
{
    std::stringstream ss(input);
    std::string command;
    ss >> command;
    ss.get(); // Process the whitespace after the command
    supported_commands[command](ss);
}