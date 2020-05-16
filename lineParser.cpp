#include "lineParser.h"

#include <regex>

//Parse the environment variable into directories.
void EnvParser::parse(void) {
  std::string pathValue = env.substr(env.find("=") + 1);
  const char * delim = ":";
  char * str = const_cast<char *>(pathValue.c_str());
  char * token = strtok(str, delim);
  while (token != NULL) {
    dirs.push_back(std::string(token));
    token = strtok(NULL, delim);
  }
}

//Update ECE551PATH
void EnvParser::updateEnv(void) {
  std::string newEnvValue(getenv("ECE551PATH"));
  std::string newEnv("ECE551PATH=");
  newEnv += newEnvValue;
  this->env = newEnv;
  std::vector<std::string>().swap(dirs);
  parse();
}

//Make full path name according to the value of isNeeded.
//If isNeeded == true, we need path extension. e.g. ls
//If isNeeded == false, we don't need path extension. e.g. /bin/ls
void EnvParser::makeFullPathName(bool isNeeded, std::string pathName) {
  if (isNeeded == true) {
    std::string temp_name = "";
    for (size_t i = 0; i < dirs.size(); i++) {
      temp_name += dirs[i];
      temp_name += '/';
      temp_name += pathName;
      fullPathName.push_back(temp_name);
      temp_name = "";
    }
  }
  else {
    fullPathName.push_back(pathName);
  }
}

//Check if we need path extention.
//Return false if we don't need path extension. e.g. /bin/ls
//Return true if we need path extension. e.g. ls
bool ArgParser::checkPathName(void) {
  std::string pathName = args[0];
  std::regex pattern1("/");
  std::regex pattern2("./");
  std::regex pattern3("../");
  bool result1 = std::regex_search(pathName, pattern1);
  bool result2 = std::regex_search(pathName, pattern2);
  bool result3 = std::regex_search(pathName, pattern3);

  if (result1 || result2 || result3) {
    //do not need path extension
    return false;
  }
  else {
    //need path extension
    return true;
  }
}

//Parse the input command line into arguments.
void ArgParser::parse(void) {
  size_t i = 0;
  int quotation_count = 0;
  //skip whitespaces in the beginning
  while (command[i] == ' ') {
    i++;
  }
  while (command[i] != '\0') {
    //encounter a quotation mark
    if (command[i] == '"') {
      quotation_count++;
      while (command[++i] != '\0') {
        //escape
        if (command[i] == '\\') {
          if (command[++i] != '\0') {
            ss << command[i];
          }
        }
        //not escape
        else {
          //encounter a close quotation mark,
          //push the string in ss onto args
          if (command[i] == '"') {
            quotation_count++;
            args.push_back(ss.str());
            ss.clear();
            ss.str("");
            i++;
            //skip whitespaces
            while (command[i] == ' ') {
              i++;
            }
            break;
          }
          //still inside a pair of quotation marks
          //read current character
          else {
            ss << command[i];
          }
        }
      }
    }
    //not quotation mark
    else {
      while (command[i] != '\0') {
        //end of an argument,
        //push the string in ss onto args
        if (command[i] == ' ') {
          args.push_back(ss.str());
          ss.clear();
          ss.str("");
          //skip whitespaces in between
          while (command[i] == ' ') {
            i++;
          }
          break;
        }
        //escape
        if (command[i] == '\\') {
          if (command[++i] != '\0') {
            ss << command[i];
          }
        }
        //not escape
        else {
          ss << command[i];
        }
        i++;
      }
    }
  }
  //push the last argument onto args
  if (ss.str() != "") {
    args.push_back(ss.str());
  }
  ss.clear();
  ss.str("");
  //Make sure there are correct number and position of quotation marks.
  if (quotation_count % 2 != 0) {
    std::cerr << "Can't have unclosed quotation mark at the end of command line!"
              << std::endl;
    //if error, clean up args and return
    std::vector<std::string>().swap(args);
    return;
  }
}

//Extract variable value from the command line
//when the user wants to set a variable.
std::string ArgParser::extractValue(void) {
  size_t i = 0;
  int spaceCount = 0;
  while (command[i] == ' ') {
    i++;
  }
  while (i != command.size()) {
    if (spaceCount == 2) {
      break;
    }
    if (command[i] == ' ') {
      spaceCount++;
    }
    i++;
  }
  return command.substr(i);
}
//Extract variable name(key) from the command line
//when the user wants to set a variable.
std::string ArgParser::extractKey(void) {
  size_t i = 0;
  while (command[i] == ' ') {
    i++;
  }
  i = i + 4;  //skip "set" and a whitespace
  size_t start = i;
  size_t j = 0;
  while (i != command.size()) {
    if (command[i] == ' ') {
      break;
    }
    i++;
    j++;
  }
  size_t length = j;
  return command.substr(start, length);
}
