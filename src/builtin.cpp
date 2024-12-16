#include "builtin.h"
#include "exe.h"

extern std::vector<std::string> supported_paths;

extern std::unordered_map<std::string, std::function<void(std::stringstream &)>> supported_commands;

extern std::filesystem::path curr_path;

void exitFn(std::stringstream &ss)
{
  if (ss.eof())
  {
    return;
  }
  int ret_val;
  ss >> ret_val;
  std::exit(ret_val);
}

static std::vector<std::string> parseString(std::string &str)
{
  std::vector<std::string> output_str;
  std::string temp_str = "";
  auto pushToVec = [&output_str, &temp_str](){
    if(temp_str.length())
    {
      output_str.push_back(temp_str);
      temp_str.clear();
    }
  };
  int idx = 0;
  while(idx <= str.length())
  {
    char curr = (idx == str.size()) ? ' ': str[idx]; // If at the end, treat it as whitespace to save
    if(curr == '\\') // Non-quoted slash
    {
      if(idx + 1 < str.size() && str[idx+1] == ' ')
      {
        temp_str.push_back(' ');
        idx+=2;
        continue;
      }
    }
    else if(curr == ' ')
    {
      pushToVec();
      idx++;
      continue;
    }
    else if(curr == '\'' || curr == '\"')
    {
      int itr = idx + 1;
      bool found_complement = false; // Check if complement found, if not parse as normal character
      while(itr < str.size())
      {
        if(str[itr] == curr)
        {
          found_complement = true;
          break;
        }
        itr++;
      }
      if(found_complement)
      {
        for(int i = idx + 1; i < itr; i++)
        {
          temp_str.push_back(str[i]);
        }
        pushToVec();
        idx = itr+1;  // If complement found, place idx next to the end of the iterator
        continue;
      }
    }
    temp_str.push_back(curr);
    idx++;
  }
  return output_str;
}

void echoFn(std::stringstream &ss)
{
  if (ss.eof())
  {
    return;
  }
  std::string input_str;
  getline(ss, input_str);
  auto string_vec = parseString(input_str);
  for (int i = 0; i < string_vec.size(); i++)
  {
    if (i == 0)
    {
      std::cout << string_vec[i];
    }
    else
    {
      std::cout << " " << string_vec[i];
    }
  }
  std::cout << "\n";
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

  for (auto path : supported_paths)
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

void pwdFn(std::stringstream &ss)
{
  std::cout << curr_path.string() << "\n";
}

void cdFn(std::stringstream &ss)
{

  std::string file_path; // Can be absolute or relative
  std::string start_path;
  std::string iter_path;
  getline(ss, start_path, '/');
  if (start_path.empty())
  {
    iter_path = curr_path.root_path().string();
  }
  else if (start_path == ".")
  {
    iter_path = curr_path.string();
  }
  else if (start_path == "~")
  {
    iter_path = std::getenv("HOME");
  }
  else if (start_path == "..")
  {
    iter_path = curr_path.parent_path().string();
  }

  std::stringstream iter_stream(iter_path);
  std::vector<std::string> traversal_stack;
  std::string iter_file;
  while (getline(iter_stream, iter_file, '/'))
  {
    if (iter_file.empty())
      continue;
    traversal_stack.push_back(iter_file);
  }

  while (getline(ss, iter_file, '/'))
  {
    if (iter_file.empty())
      continue;
    if (iter_file == ".")
      continue;
    if (iter_file == "..")
    {
      if (!traversal_stack.empty())
      {
        traversal_stack.pop_back();
      }
    }
    else
    {
      traversal_stack.push_back(iter_file);
    }
  }

  std::string final_path = (traversal_stack.empty()) ? "/" : "";
  for (auto &path : traversal_stack)
  {
    final_path += "/" + path;
  }

  if (!std::filesystem::exists(final_path))
  {
    ss.clear();
    ss.seekg(3);
    ss >> file_path;
    std::cout << "cd: " << file_path << ": No such file or directory\n";
  }
  else
  {
    std::filesystem::current_path(final_path);
    curr_path = std::filesystem::current_path();
  }
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