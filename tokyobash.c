#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tokyobash.h"

int main(int argc, char **argv) {

  char path[PATH_MAX];
  // Check to make sure we got the path
  if (!getcwd(path, sizeof(path))) {
    perror("Error: getcwd() failed to retrieve path\n");
    exit(1);
  }

  int leng = strlen(path);
  char *pHome = getenv("HOME");
  bool pHomeState = true;

  // Check that getenv() didn't return NULL
  if (!pHome) {
    pHomeState = false;
  } else {
    int Hleng = strlen(pHome);
    // If path contains $HOME replace it with '~'
    if (in_home(path, pHome, Hleng)) {
      replace_home(path, pHome, leng, Hleng);
      leng = (leng - Hleng) + 1;
    }
  }

  if (leng > ABV_PATH_LEN_T) {
    abrv_path(path, leng);
    leng = ABV_PATH_LEN_T;
  }

  // Add or remove color & format codes here
  // to use in the printfs below
  char bold[] = "\\[\\e[1m\\]";
  char reset[] = "\\[\\e[00m\\]";
  char blue[] = "\\[\\e[38;5;4m\\]";
  char lBlue[] = "\\[\\e[38;5;117m\\]";
  char cyan[] = "\\[\\e[38;5;86m\\]";
  char red[] = "\\[\\e[38;5;211m\\]";
  char yellow[] = "\\[\\e[38;5;222m\\]";

  // This checks if the term just opened,
  // W/o this, when the term first opens there
  // would be a blank line above the prompt.
  if (argc > 1) {
    if (atoi(argv[1]) > 1) {
      printf("\\n");
    }
  }

  // If getenv() returned NULL just print standard prompt
  if (!pHomeState) {
    rem_curDir(path, leng);
    printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, cyan, reset, blue, path,
           bold);
  } else {
    if (path[0] == '~') {
      // Removing current directoy from path to change
      // text to bold before adding it back with \\W.
      rem_curDir(path, leng);
      if (leng > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, cyan, reset, blue,
               path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s\\W/\\n", bold, cyan, reset, blue,
               bold);
      }
    } else {

      bool inMnt = in_mnt(path);
      rem_curDir(path, leng);

      if (inMnt) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, cyan, reset, blue,
               yellow, path, bold);
      } else if (leng > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, cyan, reset, blue,
               red, path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W\\n", bold, cyan, reset, blue,
               bold, red);
      }
    }
  }
  printf("  %s└:> %s%s", cyan, reset, lBlue);
  return 0;
}
