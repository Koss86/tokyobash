#include <stdbool.h>

#include "tokyobash.h"

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
