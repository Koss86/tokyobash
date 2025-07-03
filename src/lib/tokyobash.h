#ifndef TOKYOBASH_H
#define TOKYOBASH_H

#include <stdbool.h>

typedef enum PathState {
  Home, Mnt, Root,
} PathState;

typedef enum Themes {
  Tokyonight, Catppuccin,
  Kanagawa, Orange,
}Themes;

typedef enum Fetchtimer {
    Minute, Hour, Day,
} Fetchtimer;

typedef struct FetchOpts {
    Fetchtimer state;
    int amount;
} FetchOpts;

typedef struct ConfigSettings {
    Themes theme;
    bool git;
    bool statusbar;
    bool branchname;
    FetchOpts fetchSettings;
    bool debug;
} ConfigSettings;

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50

void parse_config(ConfigSettings *usrConfig, char *pHome, int Hlen);
void replace_home(char *path, char *home, int Plen, int Hlen);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);

bool git_is_accessible();
bool in_repo();
void get_branch(char *branch_name);
int Committed();
int Fetched(FetchOpts *fetchSettings);
void get_status_of(int *staged, int *unstaged, int *untracked);

#endif
