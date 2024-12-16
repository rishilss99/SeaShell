#include "exe.h"

extern std::vector<std::string> supported_paths;

void processPath()
{
  std::stringstream ss(std::getenv("PATH"));
  std::string path;
  while (getline(ss, path, ':'))
  {
    supported_paths.push_back(path);
  }
}

void forkAndExec(std::string &path, std::stringstream &ss)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    std::string str;
    std::vector<std::string> str_args;
    while (ss >> str)
    {
      str_args.push_back(std::move(str));
    }
    std::vector<char *> args;
    args.push_back(const_cast<char *>(path.c_str()));
    for (auto &st : str_args)
    {
      args.push_back(const_cast<char *>(st.c_str()));
    }
    args.push_back(nullptr);
    execv(path.c_str(), args.data());
  }
  else
  {
    wait(nullptr);
  }
  return;
}

bool checkIfPathExecutable(std::string &executable)
{
  for (auto path : supported_paths)
  {
    path += "/" + executable;
    if (std::filesystem::exists(path))
    {
      return true;
    }
  }
  return false;
}

bool checkIfQuotedExecutable(std::string &input)
{
  if (input.length() > 1 && input.front() == '\'' || input.front() == '\"')
  {
    char curr = input.front();
    for (int itr = 1; itr < input.length(); itr++)
    {
      if (input[itr] == curr)
      {
        return true;
      }
    }
  }
  return false;
}

void executeExecutable(std::string &command)
{
  std::system(command.c_str());
}

void executeQuotedExecutable(std::string &input)
{
  // std::stringstream ss(input);
  // std::string command;
  // getline(ss, command, '\'');
  // getline(ss, command, '\'');
  // std::string rest;
  // ss >> rest;
  // std::string final_command = command + " " + rest;
  // std::system(final_command.c_str());
  // File name with double quotes
  pid_t pid = fork();
  char *filename = "\'exe  with  space\'";

  // Arguments for the executable
  char *args[] = {filename, "/tmp/bar/f1", NULL};
  if (pid == 0)
  {
    execv("filename", args);
  }
  else
  {
    wait(nullptr);
  }
  return;
}