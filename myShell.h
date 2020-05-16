#ifndef MYSHELL_H
#define MYSHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "lineParser.h"
#include "myVar.h"
class MyShell {
  std::string currDir;

 public:
  MyVar myvar;
  MyShell() {}
  void printPrompt(void);
  void start(void);
  void myExecute(EnvParser envparser, ArgParser argparser);
  EnvParser initEnv(void);
  void updateCurrDir(void);
  friend class ArgParser;
  friend class EnvParser;
  friend class myVar;
};

#endif
