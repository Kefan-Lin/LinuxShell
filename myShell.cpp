#include "myShell.h"

#include "myVar.h"
//Print current directory and the prompt
void MyShell::printPrompt() {
  std::cout << "ffosh:" << currDir << " $ ";
}
//Start the shell
void MyShell::start() {
  //Initialize ECE551PATH
  EnvParser envparser = initEnv();
  while (true) {
    //Update ECE551PATH
    envparser.updateEnv();
    //Update current directory
    updateCurrDir();
    //Print the prompt
    printPrompt();
    //Encounter EOF, exit the shell
    if (std::cin.eof()) {
      return;
    }
    //Get a line from stdin,and use it to initialize an ArgParser
    std::string inputLine;
    std::getline(std::cin, inputLine);
    ArgParser argparser(inputLine);
    argparser.parse();
    if (argparser.args.size() == 0) {
      continue;
    }
    //Replace all variables in arguments
    argparser = myvar.replaceAll(argparser);
    if (argparser.args.size() == 0) {
      continue;
    }
    //If user type "exit", exit the shell
    if (inputLine.compare("exit") == 0) {
      return;
    }
    //If user just type Enter, start another line, waiting for input
    else if (inputLine.compare("") == 0) {
      continue;
    }
    //set a variable
    else if (argparser.args[0] == "set") {
      if (argparser.args.size() >= 3) {
        std::string key = argparser.extractKey();
        std::string value = argparser.extractValue();
        myvar.addVar(key, value);
        continue;
      }
      else {
        std::cerr << "Input format is wrong.\nUsage: set var value" << std::endl;
        continue;
      }
    }
    //export a variable
    else if (argparser.args[0] == "export") {
      if (argparser.args.size() == 2) {
        myvar.exportVar(argparser.args[1]);
        continue;
      }
      else {
        std::cerr << "Input format is wrong.\nUsage: export var" << std::endl;
        continue;
      }
    }
    //rev a variable
    else if (argparser.args[0] == "rev") {
      if (argparser.args.size() == 2) {
        myvar.reverseVar(argparser.args[1]);
        continue;
      }
      else {
        std::cerr << "Input format is wrong.\nUsage: rev var" << std::endl;
        continue;
      }
    }
    //Build-in cd command
    else if (argparser.args[0] == "cd") {
      if (argparser.args.size() == 1) {
        int cd_success = chdir(getenv("HOME"));
        if (cd_success == 0) {
          updateCurrDir();
          continue;
        }
        else {
          std::cerr << "The HOME variable is not valid." << std::endl;
          continue;
        }
      }
      else if (argparser.args.size() == 2) {
        int cd_success = chdir(argparser.args[1].c_str());
        if (cd_success == 0) {
          updateCurrDir();
          continue;
        }
        else {
          std::cerr << "No such directory" << std::endl;
          continue;
        }
      }
      else {
        std::cerr << "Input format is wrong.\nUsage: cd dir" << std::endl;
        continue;
      }
    }
    //Execute command
    else {
      myExecute(envparser, argparser);
    }
  }
}

//Execute command
void MyShell::myExecute(EnvParser envparser, ArgParser argparser) {
  pid_t pid = fork();
  //fork failure handle
  if (pid < 0) {
    std::cerr << "Fork failed!" << std::endl;
    return;
  }
  //child process
  else if (pid == 0) {
    argparser = myvar.replaceAll(argparser);
    //check if we need path extension
    bool isNeeded = argparser.checkPathName();
    //make full path name
    envparser.makeFullPathName(isNeeded, argparser.args[0]);
    char * input_argv[1024] = {NULL};
    if (argparser.args.size() > 1023) {
      std::cerr << "Too many arguments!\nMax argument number:1023" << std::endl;
      exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < argparser.args.size(); i++) {
      input_argv[i] = const_cast<char *>(argparser.args[i].c_str());
    }
    for (size_t i = 0; i < envparser.fullPathName.size(); i++) {
      execve(const_cast<char *>(envparser.fullPathName[i].c_str()), input_argv, environ);
    }
    std::cout << "Command " << argparser.args[0] << " not found" << std::endl;
    exit(EXIT_FAILURE);
  }

  //parent process
  else {
    int wstatus, w;
    do {
      w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
      if (w == -1) {
        std::cerr << "waitpid" << std::endl;
        return;
      }

      if (WIFEXITED(wstatus)) {
        if (WEXITSTATUS(wstatus) == 0) {
          std::cout << "Program was successful" << std::endl;
        }
        else {
          std::cout << "Program failed with code " << WEXITSTATUS(wstatus) << std::endl;
        }
      }
      else if (WIFSIGNALED(wstatus)) {
        std::cout << "Terminated by signal " << WTERMSIG(wstatus) << std::endl;
      }
    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
  }
}

//Initialize ECE551PATH
EnvParser MyShell::initEnv(void) {
  std::string pathValue(getenv("PATH"));
  myvar.addVar("ECE551PATH", pathValue);
  std::string newPath("ECE551PATH=");
  newPath += pathValue;
  EnvParser envparser(newPath);
  envparser.parse();
  myvar.exportVar("ECE551PATH");
  return envparser;
}

//Update current directory
void MyShell::updateCurrDir(void) {
  char * cwd = getcwd(NULL, 0);
  if (cwd != NULL) {
    std::string str_cwd(cwd);
    currDir = "";
    currDir += str_cwd;
    free(cwd);
    return;
  }
  else {
    std::cerr << "Can not get current directory!" << std::endl;
    return;
  }
}
