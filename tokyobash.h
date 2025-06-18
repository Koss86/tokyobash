#ifndef TOKYOBASH_H
#define TOKYOBASH_H

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

typedef enum Themes {
  Tokyonight, Catppuccin,
  Kanagawa, Koss,
}Themes;

void replace_home(char *path, char *home, int Plen, int Hlen);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
bool git_is_accessible();
bool in_repo();
bool get_branch(char *branch_name);
int Untracked();
int Unstaged();
int Staged();
int Committed();
int Fetched(int seconds);

#endif
