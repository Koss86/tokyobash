#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 25
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

bool in_home(char *path, char *home, int Hlen);
void replace_home(char *path, char *home, int Plen, int indx);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
bool in_mnt(char *path);

int main(int argc, char **argv) {

  char path[PATH_MAX];
  // Check to make sure we got the path
  if (!getcwd(path, sizeof(path))) {
    perror("Error: getcwd() failed to retrieve path\\n");
    exit(1);
  }

  char reset[] = "\\[\\033[00m\\]";
  char bold[] = "\\[\\033[1m\\]";
  char cyan[] = "\\[\\033[38;5;86m\\]";
  char blue[] = "\\[\\033[38;5;4m\\]";
  char lBlue[] = "\\[\\033[38;5;117m\\]";
  char red[] = "\\[\\033[38;5;211m\\]";
  char peach[] = "\\[\\033[38;5;223m\\]";
  char purple[] = "\\[\\033[38;5;182m\\]";
  char pink[] = "\\[\\033[38;5;217m\\]";
  char orange[] = "\\[\\033[38;5;214m\\]";
  char lGreen[] = "\\[\\033[38;5;149m\\]";

  char *color_1; // User/Host
  char *color_2; // Time
  char *color_3; // Path
  char *color_4; // Text
  char *color_5; // /mnt Path
  char *color_6; // /root Path

  if (argc > 1) {
    if (atoi(argv[1]) > 1) {
      printf("\\n");
    }
    if ((strcmp(argv[2], "catppuccin")) == 0) {
      color_1 = &peach[0];
      color_2 = &purple[0];
      color_3 = &purple[0];
      color_4 = &pink[0];
      color_5 = &lBlue[0];
      color_6 = &orange[0];
    } else if ((strcmp(argv[2], "tokyonight")) == 0) {
      color_1 = &cyan[0];
      color_2 = &lBlue[0];
      color_3 = &blue[0];
      color_4 = &lBlue[0];
      color_5 = &peach[0];
      color_6 = &red[0];
    }
  }

  int Plen = strlen(path);
  char *pHome = getenv("HOME");
  bool pHomeState = true;

  // Check that getenv() didn't return NULL
  if (!pHome) {
    pHomeState = false;
  } else {
    int Hlen = strlen(pHome);
    // If path contains $HOME replace it with '~'
    if (in_home(path, pHome, Hlen)) {
      replace_home(path, pHome, Plen, Hlen);
      Plen = (Plen - Hlen) + 1;
    }
  }

  if (Plen > ABV_PATH_LEN_T) {
    abrv_path(path, Plen);
    Plen = ABV_PATH_LEN_T;
  }

  // If getenv() returned NULL, just print standard prompt
  if (!pHomeState) {
    rem_curDir(path, Plen);
    printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
           color_2, color_3, path, bold);
  } else {
    if (path[0] == '~') {
      // Removing current directoy from path to change
      // text to bold before adding it back with \\W.
      rem_curDir(path, Plen);
      if (Plen > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
               color_2, color_3, path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W/\\n", bold, color_1, reset,
               color_2, color_3, bold);
      }
    } else {

      bool inMnt = in_mnt(path);
      rem_curDir(path, Plen);

      if (inMnt) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
               color_2, color_5, path, bold);
      } else if (Plen > 1) {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s\\W/\\n", bold, color_1, reset,
               color_2, color_6, path, bold);
      } else {
        printf("%s%s\\u@\\h%s:%s [\\t] %s%s\\W\\n", bold, color_1, reset,
               color_2, bold, color_6);
      }
    }
  }
  printf("  %s└:> %s%s", color_1, reset, color_4);
  return 0;
}

bool in_home(char *path, char *home, int Hlen) {
  for (int i = 0; i < Hlen; i++) {
    if (path[i] != home[i]) {
      return false;
    }
  }
  return true;
}

void replace_home(char *path, char *home, int Plen, int Hlen) {
  path[0] = '~';
  if (Plen == Hlen) {
    path[1] = '\0';
  } else {
    int indx = Hlen;
    for (int i = 1; i < Plen; i++) {
      path[i] = path[indx++];
    }
    path[indx] = '\0';
  }
}

void abrv_path(char *path, int Plen) {
  int i;
  for (i = ABV_PATH_LEN1; i < ABV_PATH_LEN1 + 3; i++) {
    path[i] = '.';
  }
  int indx = Plen - ABV_PATH_LEN2;
  for (i = ABV_PATH_LEN1 + 3; i < ABV_PATH_LEN_T; i++) {
    path[i] = path[indx++];
  }
  path[ABV_PATH_LEN_T] = '\0';
}

void rem_curDir(char *path, int Plen) {
  for (int i = Plen - 1; i > -1; i--) {
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
