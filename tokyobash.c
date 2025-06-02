#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50

bool in_home(char *path, char *home, int Hleng);
void replace_home(char *path, char *home, int leng, int indx);
void abrv_path(char *path, int leng);
void rem_curDir(char *path, int leng);
bool in_mnt(char *path);

int main(int argc, char *argv[]) {

  char path[PATH_MAX];
  // Check to make sure we got the path
  if (!getcwd(path, sizeof(path))) {
    perror("Error: getcwd() failed to retrieve path\n");
    exit(1);
  }
  char *pHome = getenv("HOME");
  int leng = strlen(path);
  bool pHomeState = true;

  // Check that getenv() didn't return NULL
  if (!pHome) {
    pHomeState = false;
  } else {
    int Hleng = strlen(pHome);
    // If path contains $HOME replace it with '~'
    if (in_home(path, pHome, Hleng)) {
      replace_home(path, pHome, leng, Hleng);
      leng = leng - Hleng;
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

    // Removing current directoy from path to change
    // text to bold before adding it back with \\W.

    rem_curDir(path, leng);
    printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, cyan, reset, blue, path,
           bold);
  } else {

    if (leng > 1) {

      if (path[0] == '~') {
        rem_curDir(path, leng);
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, cyan, reset, blue,
               path, bold);
      } else {

        bool inMnt = in_mnt(path);
        rem_curDir(path, leng);

        if (inMnt) {
          printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, cyan, reset,
                 blue, yellow, path, bold);
        } else {
          printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, cyan, reset,
                 blue, red, path, bold);
        }
      }
    } else {
      if (path[0] == '~') {

        printf("%s%s\\u@\\h%s:%s [\\t] %s\\W/\\n", bold, cyan, reset, blue,
               bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W\\n", bold, cyan, reset, blue,
               bold, red);
      }
    }
  }

  printf("  %s└:> %s%s", cyan, reset, lBlue);
  return 0;
}

bool in_home(char *path, char *home, int Hleng) {
  for (int i = 0; i < Hleng; i++) {
    if (path[i] != home[i]) {
      return false;
    }
  }
  return true;
}

void replace_home(char *path, char *home, int leng, int Hleng) {
  path[0] = '~';
  if (leng == Hleng) {
    path[1] = '\0';
  } else {
    int indx = Hleng;
    for (int i = 1; i < leng; i++) {
      path[i] = path[indx++];
    }
    path[indx] = '\0';
  }
}

void abrv_path(char *path, int leng) {
  int i;
  for (i = ABV_PATH_LEN1; i < ABV_PATH_LEN1 + 3; i++) {
    path[i] = '.';
  }
  int indx = leng - ABV_PATH_LEN2;
  for (i = ABV_PATH_LEN1 + 3; i < ABV_PATH_LEN_T; i++) {
    path[i] = path[indx++];
  }
  path[ABV_PATH_LEN_T] = '\0';
}

void rem_curDir(char *path, int leng) {
  for (int i = leng - 1; i > -1; i--) {
    if (path[i] == '/') {
      path[i + 1] = '\0';
      break;
    }
  }
}

bool in_mnt(char *path) {
  char mnt[] = "/mnt";
  for (int i = 0; i < 4; i++) {
    if (path[i] != mnt[i]) {
      return false;
    }
  }
  return true;
}
