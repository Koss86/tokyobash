#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."
#define MAX_BRANCH_LEN 256

void replace_home(char *path, char *home, int Plen, int indx);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
int git_is_accessible();
int in_repo();
int get_branch(char *bName);

int main(int argc, char **argv) {

  char path[PATH_MAX];
  // Check to make sure we got the path
  if (!getcwd(path, sizeof(path))) {
    perror("Error: getcwd() failed to retrieve path\\n");
    exit(1);
  }

  char reset[] = "\\[\\e[00m\\]";
  char bold[] = "\\[\\e[1m\\]";
  char cyan[] = "\\[\\e[38;5;86m\\]";
  char dCyan[] = "\\[\\e[38;2;104;155;196m\\]";
  char blue[] = "\\[\\e[38;5;4m\\]";
  char lBlue[] = "\\[\\e[38;5;117m\\]";
  char red[] = "\\[\\e[38;2;255;77;77m\\]";
  char lRed[] = "\\[\\e[38;5;211m\\]";
  char peach[] = "\\[\\e[38;5;223m\\]";
  char purple[] = "\\[\\e[38;5;182m\\]";
  char pink[] = "\\[\\e[38;5;217m\\]";
  char orange[] = "\\[\\e[38;5;214m\\]";
  char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
  char lime[] = "\\[\\e[38;5;149m\\]";

  char *color_usr = &cyan[0];
  char *color_time = &lBlue[0];
  char *color_path = &blue[0];
  char *color_mnt = &peach[0];
  char *color_root = &lRed[0];

  if (argc > 1) {

    if (argv[1][0] >= 'a' && argv[1][0] <= 'z') {

      if (!strcmp(argv[1], "catppuccin")) {
        color_usr = &peach[0];
        color_time = &pink[0];
        color_path = &purple[0];
        color_mnt = &lBlue[0];
        color_root = &orange[0];

      } else if (!strcmp(argv[1], "kanagawa")) {
        color_usr = &red[0];
        color_time = &dCyan[0];
        color_path = &khaki[0];
        color_mnt = &lime[0];
        color_root = &purple[0];
      }

      if (argc > 2) {
        if (atoi(argv[2]) > 1) {
          printf("\\n");
        }
      }

    } else {

      if (atoi(argv[1]) > 1) {
        printf("\\n");
      }

      if (argc > 2) {

        if (!strcmp(argv[2], "catppuccin")) {
          color_usr = &peach[0];
          color_time = &pink[0];
          color_path = &purple[0];
          color_mnt = &lBlue[0];
          color_root = &orange[0];

        } else if (!strcmp(argv[2], "kanagawa")) {
          color_usr = &red[0];
          color_time = &dCyan[0];
          color_path = &khaki[0];
          color_mnt = &lime[0];
          color_root = &purple[0];
        }
      }
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
    if (strstr(path, pHome) != NULL) {
      replace_home(path, pHome, Plen, Hlen);
      Plen = (Plen - Hlen) + 1;
    }
  }

  if (Plen > ABV_PATH_LEN_T) {
    abrv_path(path, Plen);
    Plen = ABV_PATH_LEN_T;
  }

  char bName[MAX_BRANCH_LEN];
  // Check if git is available and current directory is a repo.
  // If yes, get current branch name for prompt.
  if (git_is_accessible() && in_repo()) {

    get_branch(&bName[0]);

    printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s  ", bold, color_usr, reset,
           color_time, color_path, bName, color_usr);

  } else {

    printf("%s%s\\u@\\h%s:%s [\\t] ", bold, color_usr, reset, color_time);
  }

  // If getenv() returned NULL, just print standard prompt
  if (!pHomeState) {
    rem_curDir(path, Plen);
    printf("%s%s%s\\W/\\n", color_path, path, bold);
  } else {
    if (path[0] == '~') {
      // Removing current directoy from path to change
      // text to bold before adding it back with \\W.
      rem_curDir(path, Plen);
      if (Plen > 1) {
        printf("%s%s%s\\W/\\n", color_path, path, bold);
      } else {
        printf("%s%s\\W/\\n", color_path, bold);
      }
    } else {

      bool inMnt;
      if (strstr(path, "/mnt") != NULL) {
        inMnt = true;
      } else {
        inMnt = false;
      }
      rem_curDir(path, Plen);

      if (inMnt) {
        printf("%s%s%s\\W/\\n", color_mnt, path, bold);
      } else if (Plen > 1) {
        printf("%s%s%s\\W/\\n", color_root, path, bold);
      } else {
        printf("%s%s\\W\\n", bold, color_root);
      }
    }
  }
  printf("  %s└:> %s", color_usr, reset);
  return 0;
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

int git_is_accessible() {
  FILE *file;
  if ((file = popen("git --version 2>/dev/null", "r")) == NULL) {
    return 0;
  }
  char buf[64];
  if (fgets(buf, sizeof(buf), file) == NULL) {
    pclose(file);
    return 0;
  }
  pclose(file);
  return (strstr(buf, "git version") != NULL);
}

int in_repo() {
  FILE *file;
  if ((file = popen("git rev-parse --is-inside-work-tree 2>/dev/null", "r")) ==
      NULL) {
    return 0;
  }
  char buf[16];
  if (fgets(buf, sizeof(buf), file) == NULL) {
    pclose(file);
    return 0;
  }
  pclose(file);
  return (strncmp(buf, "true", 4) == 0);
}

int get_branch(char *bName) {
  FILE *file;
  if ((file = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r")) ==
      NULL) {
    return 0;
  }
  if (fgets(bName, MAX_BRANCH_LEN, file) == NULL) {
    pclose(file);
    return 0;
  }
  int len = strlen(bName);
  if (len > 0 && bName[len - 1] == '\n') {
    bName[len - 1] = '\0';
  }
  pclose(file);
  return 1;
}
