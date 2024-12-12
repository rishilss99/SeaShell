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
    std::vector<char*> args;
    args.push_back(const_cast<char*>(path.c_str()));
    for(auto& st: str_args)
    {
        args.push_back(const_cast<char*>(st.c_str()));
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

void executeExecutable(std::string &command)
{
    std::system(command.c_str());
}