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

void replace_home(char *path, char *home, int Plen, int Hlen);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
bool git_is_accessible();
bool in_repo();
bool get_branch(char *branch_name);

int main(int argc, char **argv) {

  char path[PATH_MAX];
  // Check to make sure we got the path
  if (getcwd(path, sizeof(path)) == NULL) {
    perror("Error: getcwd() failed to retrieve path\\n");
    exit(1);
  }

  char reset[] = "\\[\\e[00m\\]";
  char bold[] = "\\[\\e[1m\\]";

  char cyan[] = "\\[\\e[38;5;86m\\]";
  char sky_blue[] = "\\[\\e[38;5;117m\\]";
  char blue[] = "\\[\\e[38;5;4m\\]";
  char peach[] = "\\[\\e[38;5;223m\\]";
  char light_red[] = "\\[\\e[38;5;211m\\]";

  char pink[] = "\\[\\e[38;5;217m\\]";
  char purple[] = "\\[\\e[38;5;182m\\]";
  char orange[] = "\\[\\e[38;5;214m\\]";

  char red[] = "\\[\\e[38;2;255;77;77m\\]";
  char teal[] = "\\[\\e[38;2;140;185;187m\\]";
  char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
  char lime[] = "\\[\\e[38;2;117;156;38m\\]";

  char dark_orange[] = "\\[\\e[38;2;255;149;20m\\]";
  char beige[] = "\\[\\e[38;2;239;239;200m\\]";
  char white[] = "\\[\\e[38;2;255;255;255m\\]";

  // Assign tokyonight colors as default.
  char *color_usr = &cyan[0];
  char *color_time = &sky_blue[0];
  char *color_path = &blue[0];
  char *color_mnt = &orange[0];
  char *color_root = &light_red[0];

  if (argc > 1) {

    if (argv[1][0] >= 'a' && argv[1][0] <= 'z') {

      if (strcmp(argv[1], "catppuccin") == 0) {
        color_usr = &peach[0];
        color_time = &pink[0];
        color_path = &purple[0];
        color_mnt = &blue[0];
        color_root = &orange[0];

      } else if (strcmp(argv[1], "kanagawa") == 0) {
        color_usr = &red[0];
        color_time = &teal[0];
        color_path = &khaki[0];
        color_mnt = &lime[0];
        color_root = &purple[0];

      } else if (strcmp(argv[1], "koss") == 0) {
        color_usr = &dark_orange[0];
        color_time = &beige[0];
        color_path = &white[0];
      }

      if (argc > 2 && (atoi(argv[2]) > 1)) {
        printf("\\n");
      }

    } else {

      if (atoi(argv[1]) > 1) {
        printf("\\n");
      }

      if (argc > 2) {

        if (strcmp(argv[2], "catppuccin") == 0) {
          color_usr = &peach[0];
          color_time = &pink[0];
          color_path = &purple[0];
          color_mnt = &blue[0];
          color_root = &orange[0];

        } else if (strcmp(argv[2], "kanagawa") == 0) {
          color_usr = &red[0];
          color_time = &teal[0];
          color_path = &khaki[0];
          color_mnt = &lime[0];
          color_root = &purple[0];

        } else if (strcmp(argv[2], "koss") == 0) {
          color_usr = &dark_orange[0];
          color_time = &beige[0];
          color_path = &white[0];
        }
      }
    }
  }

  int Plen = strlen(path);
  char *pHome = getenv("HOME");
  bool pHomeState = true;

  // Check that getenv() didn't return NULL
  if (pHome == NULL) {

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

  // Check if git is available and current directory is a repo.
  // If yes, get current branch name for prompt.
  if (git_is_accessible() && in_repo()) {

    char branch_name[MAX_BRANCH_LEN];
    get_branch(&branch_name[0]);

    printf("%s%s\\u@\\h%s:%s [\\t] %s%s%s  ", bold, color_usr, reset,
           color_time, color_path, branch_name, color_usr);

  } else {

    printf("%s%s\\u@\\h%s:%s [\\t] ", bold, color_usr, reset, color_time);
  }

  // If getenv() returned NULL, just print standard prompt
  if (pHomeState == false) {

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
        printf("%s%s\\W\\n", color_root, bold);
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

bool git_is_accessible() {
  FILE *file;
  if ((file = popen("git --version 2>/dev/null", "r")) == NULL) {
    return false;
  }
  char buf[64];
  if (fgets(buf, sizeof(buf), file) == NULL) {
    pclose(file);
    return false;
  }
  pclose(file);
  return (strstr(buf, "git version") != NULL);
}

bool in_repo() {
  FILE *file = popen("git rev-parse --is-inside-work-tree 2>/dev/null", "r");
  if (file == NULL) {
    return false;
  }
  char buf[16];
  if (fgets(buf, sizeof(buf), file) == NULL) {
    pclose(file);
    return false;
  }
  pclose(file);
  return (strncmp(buf, "true", 4) == 0);
}

bool get_branch(char *branch_name) {
  FILE *file = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
  if (file == NULL) {
    return false;
  }
  if (fgets(branch_name, MAX_BRANCH_LEN, file) == NULL) {
    pclose(file);
    return false;
  }
  int len = strlen(branch_name);
  if (len > 0 && branch_name[len - 1] == '\n') {
    branch_name[len - 1] = '\0';
  }
  pclose(file);
  return true;
}
