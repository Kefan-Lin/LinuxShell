#ifndef MYVAR_H
#define MYVAR_H
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "lineParser.h"

class MyVar {
  std::map<std::string, std::string> varMap;

 public:
  MyVar() {}
  ~MyVar() {}
  void addVar(std::string key, std::string value);
  void exportVar(std::string key);
  void reverseVar(std::string key);
  ArgParser replaceAll(ArgParser argparser);
  std::string replaceVar(std::string arg);
  std::vector<std::string> sliceByDollar(std::string arg);
  friend class MyShell;
  friend class ArgParser;
  friend class EnvParser;
};

#endif
