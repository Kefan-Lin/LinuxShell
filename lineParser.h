#ifndef LINEPARSER_H
#define LINEPARSER_H
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Parser {
 public:
  virtual void parse(void) = 0;
};

class EnvParser : public Parser {
 private:
  std::string env;
  std::stringstream ss;
  std::vector<std::string> dirs;
  std::vector<std::string> fullPathName;

 public:
  EnvParser(std::string toParse) : env(toParse) { ss.str(toParse); }
  EnvParser(const EnvParser & rhs) :
      env(rhs.env),
      ss(rhs.ss.str()),
      dirs(rhs.dirs),
      fullPathName(rhs.fullPathName) {}
  EnvParser & operator=(const EnvParser & rhs) {
    if (this != &rhs) {
      env = rhs.env;
      ss = std::stringstream(rhs.ss.str());
      dirs = rhs.dirs;
      fullPathName = rhs.fullPathName;
    }
    return *this;
  }
  ~EnvParser() {
    ss.clear();
    std::vector<std::string>().swap(dirs);
    std::vector<std::string>().swap(fullPathName);
  }
  virtual void parse(void);

  void updateEnv(void);
  void makeFullPathName(bool isNeeded, std::string pathName);
  friend class MyShell;
  friend class ArgParser;
  friend class MyVar;
};

class ArgParser : public Parser {
 private:
  std::string command;
  std::stringstream ss;
  std::vector<std::string> args;

 public:
  ArgParser(std::string toParse) : command(toParse) {}
  ArgParser(ArgParser & rhs) : command(rhs.command), ss(rhs.ss.str()), args(rhs.args) {}
  ArgParser & operator=(const ArgParser & rhs) {
    if (this != &rhs) {
      command = rhs.command;
      ss = std::stringstream(rhs.ss.str());
      args = rhs.args;
    }
    return *this;
  }
  ~ArgParser() {
    ss.clear();
    std::vector<std::string>().swap(args);
  }
  virtual void parse(void);
  bool checkPathName(void);
  std::string extractKey(void);
  std::string extractValue(void);
  friend class MyShell;
  friend class EnvParser;
  friend class MyVar;
};

#endif
