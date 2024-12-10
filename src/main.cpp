#include <iostream>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <string_view>
#include <vector>
#include <filesystem>

// Supported commands
std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands;

// Supported Paths
std::vector<std::string> supported_paths;

void exitFn(std::stringstream &ss)
{
  int ret_val;
  ss >> ret_val;
  std::exit(ret_val);
}

void echoFn(std::stringstream &ss)
{
  std::string input_str;
  ss.get();
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
    if(std::filesystem::exists(path))
    {
        std::cout << input_command << " is " << path << "\n";
        return;
    }
  }
  
  std::cout << input_command << ": not found\n";
}

void processPath()
{
  std::stringstream ss(std::getenv("PATH"));
  std::string path;
  while (getline(ss, path, ':'))
  {
    supported_paths.push_back(path);
  }
}

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
