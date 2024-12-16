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
        std::string executable = input.substr(1, itr - 1);
        for (auto path : supported_paths)
        {
          path += "/" + curr + executable + curr;
          if (std::filesystem::exists(std::filesystem::absolute(path)))
            ;
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void executeExecutable(std::string &command)
{
  std::system(command.c_str());
}

static std::string parseString(std::string &str)
{
  std::string temp_str = "";
  auto findComplement = [&str](int &itr, bool &found_complement, char curr)
  {
    while (itr < str.size())
    {
      if (str[itr] == curr)
      {
        found_complement = true;
        break;
      }
      itr++;
    }
  };
  int idx = 0;
  char curr = str[idx];
  int itr = idx + 1;
  bool found_complement = false; // Check if complement found, if not parse as normal character
  findComplement(itr, found_complement, curr);
  temp_str.push_back(curr);
  if (found_complement)
  {
    if (curr == '\'') // Handles backslash as literal
    {
      for (int i = idx + 1; i < itr; i++)
      {
        temp_str.push_back(str[i]);
      }
    }
    else if (curr == '\"') // Holds special meaning for backslash if followed by \,$,",newline
    {
      std::unordered_set<char> special_chars = {'\"', '\\', '$'};
      if (str[itr - 1] == '\\')
      {
        int itr2 = itr + 1;
        bool found_complement2 = false;
        findComplement(itr2, found_complement2, curr);
        if (found_complement2)
        {
          itr = itr2;
        }
      }
      for (int i = idx + 1; i < itr; i++)
      {
        if (str[i] == '\\' && i + 1 < itr && special_chars.find(str[i + 1]) != special_chars.end())
        {
          temp_str.push_back(str[i + 1]);
          i++;
          continue;
        }
        temp_str.push_back(str[i]);
      }
    }
  }
  temp_str.push_back(curr);
  return temp_str;
}

void executeQuotedExecutable(std::string &input)
{
  std::string executable = parseString(input);
  std::stringstream ss(input);
  std::string args;
  char quote = ss.get();
  getline(ss, args, quote);
  ss.get();
  getline(ss, args);
  std::string path = executable + " " + args;
  std::system(path.c_str());
  return;
}