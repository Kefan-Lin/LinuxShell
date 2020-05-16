#include <cstdlib>
#include <string>

#include "myShell.h"
extern char ** environ;
int main(void) {
  MyShell ffosh;
  ffosh.start();
  return EXIT_SUCCESS;
}
