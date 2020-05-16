#include "myVar.h"

#include <ctype.h>
#include <string.h>

#include <vector>

//Add a variable into varmap
void MyVar::addVar(std::string key, std::string value) {
  int c = 0;
  for (size_t i = 0; i < key.size(); i++) {
    c = key[i];
    if (!(isalnum(c) || c == '_')) {
      std::cerr
          << "Variable name must be a combination of letters, numbers and underscores!"
          << std::endl;
      return;
    }
  }
  std::map<std::string, std::string>::iterator it;
  it = varMap.find(key);
  //if the variable exists, update it
  if (it != varMap.end()) {
    varMap[key] = value;
    return;
  }
  //if the variable is new, insert it
  varMap.insert(std::pair<std::string, std::string>(key, value));
}

//Export a variable into environment
void MyVar::exportVar(std::string key) {
  std::map<std::string, std::string>::iterator it;
  it = varMap.find(key);
  if (it != varMap.end()) {
    std::string toExport = "";
    toExport += key;
    toExport += '=';
    toExport += it->second;
    if (setenv(key.c_str(), it->second.c_str(), 1) != 0) {
      std::cerr << "export " << key << " failed" << std::endl;
      return;
    }
  }
  else {
    std::cerr << "Variable " << key << " not found, can not export it!" << std::endl;
    return;
  }
}

//Reverse a variable's value
void MyVar::reverseVar(std::string key) {
  std::map<std::string, std::string>::iterator it;
  it = varMap.find(key);
  if (it != varMap.end()) {
    std::reverse(it->second.begin(), it->second.end());
  }
  else {
    std::cerr << "Variable " << key << " not found,can not reverse it!" << std::endl;
    return;
  }
}

//Replace all variables found in arguments
ArgParser MyVar::replaceAll(ArgParser argparser) {
  //for each argument, replace the variables inside of it
  for (size_t i = 0; i < argparser.args.size(); i++) {
    std::string afterReplace = replaceVar(argparser.args[i]);
    argparser.args[i] = afterReplace;
  }
  return argparser;
}

//Replace variables in a single argument
std::string MyVar::replaceVar(std::string arg) {
  size_t foundDollar = arg.find("$");
  if (foundDollar == std::string::npos) {
    return arg;
  }
  if (foundDollar == 0) {
    std::string str_return = "";
  }
  std::string str_return = arg.substr(0, foundDollar);
  std::vector<std::string> tokens = sliceByDollar(arg.substr(foundDollar));
  for (size_t i = 0; i < tokens.size(); i++) {
    bool foundVar = false;
    size_t j = 0;
    for (j = 0; j < tokens[i].size(); j++) {
      if (isalnum(tokens[i][j]) || tokens[i][j] == '_') {
        continue;
      }
      else {
        break;
      }
    }
    if (j == 0) {
      return arg;
    }
    //find the longest possible variable name in each token
    std::string validVarName = tokens[i].substr(0, j);
    std::map<std::string, std::string>::iterator it;
    it = varMap.find(validVarName);
    if (it != varMap.end()) {
      foundVar = true;
      std::string newToken = "";
      newToken += it->second;
      newToken += tokens[i].substr(j);
      tokens[i] = newToken;
    }
    if (foundVar == false) {
      std::cerr << "Variable " << validVarName << " not found" << std::endl;
      tokens[i] = tokens[i].substr(j);
    }
  }
  for (size_t i = 0; i < tokens.size(); i++) {
    str_return += tokens[i];
  }
  return str_return;
}

//Slice an argument by '$'
std::vector<std::string> MyVar::sliceByDollar(std::string arg) {
  std::vector<std::string> tokens;
  const char * delim = "$";
  char * str = const_cast<char *>(arg.c_str());
  char * token = strtok(str, delim);
  while (token != NULL) {
    tokens.push_back(std::string(token));
    token = strtok(NULL, delim);
  }
  return tokens;
}
