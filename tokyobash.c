#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THEME_1 "catppuchinn"
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

bool in_home(char *path, char *home, int Hleng);
void replace_home(char *path, char *home, int leng, int indx);
void abrv_path(char *path, int leng);
void rem_curDir(char *path, int leng);
bool in_mnt(char *path);

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
  char color_1[50];
  char color_2[50];
  char color_3[50];
  char color_4[50];
  char color_5[50];

  if (argc > 1) {
    if (atoi(argv[1]) > 1) {
      printf("\\n");
    }
    if ((strcmp(argv[2], "catppuccin")) == 0) {
      strncpy(color_1, "\\[\\e[38;5;208m\\]", 50);
      strncpy(color_2, "\\[\\e[38;5;90m\\]", 50);
      strncpy(color_3, "\\[\\e[38;5;117m\\]", 50);
      strncpy(color_4, "\\[\\e[38;5;222m\\]", 50);
      strncpy(color_5, "\\[\\e[38;5;211m\\]", 50);
    } else {
      strncpy(color_1, "\\[\\e[38;5;86m\\]", 50);
      strncpy(color_2, "\\[\\e[38;5;4m\\]", 50);
      strncpy(color_3, "\\[\\e[38;5;117m\\]", 50);
      strncpy(color_4, "\\[\\e[38;5;222m\\]", 50);
      strncpy(color_5, "\\[\\e[38;5;211m\\]", 50);
    }
  }

  // If getenv() returned NULL just print standard prompt
  if (!pHomeState) {
    rem_curDir(path, leng);
    printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, color_1, reset, color_2,
           path, bold);
  } else {
    if (path[0] == '~') {
      // Removing current directoy from path to change
      // text to bold before adding it back with \\W.
      rem_curDir(path, leng);
      if (leng > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, color_1, reset,
               color_2, path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s\\W/\\n", bold, color_1, reset,
               color_2, bold);
      }
    } else {

      bool inMnt = in_mnt(path);
      rem_curDir(path, leng);

      if (inMnt) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
               color_2, color_4, path, bold);
      } else if (leng > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
               color_2, color_5, path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W\\n", bold, color_1, reset,
               color_2, bold, color_5);
      }
    }
  }
  printf("  %s└:> %s%s", color_1, reset, color_3);
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
