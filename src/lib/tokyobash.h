#ifndef TOKYOBASH_H
#define TOKYOBASH_H

typedef enum Themes {
  Tokyonight, Catppuccin,
  Kanagawa, Orange,
}Themes;

void replace_home(char *path, char *home, int Plen, int Hlen);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
bool git_is_accessible();
bool in_repo();
bool get_branch(char *branch_name);
int Committed();
int Fetched();
void get_status_of(int *staged, int *unstaged, int *untracked);

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50

#define bold "\\[\\e[1m\\]"
#define reset "\\[\\e[00m\\]"

#define red "\\[\\e[38;2;255;77;77m\\]"
#define light_red "\\[\\e[38;5;211m\\]"

#define pink "\\[\\e[38;5;217m\\]"
#define peach "\\[\\e[38;5;223m\\]"

#define dark_orange "\\[\\e[38;2;255;149;20m\\]"
#define orange "\\[\\e[38;5;214m\\]"

#define yellow "\\[\\e[38;2;255;255;0m\\]"
#define green "\\[\\e[38;2;0;255;0m\\]"
#define lime_green "\\[\\e[38;2;172;255;47m\\]"
#define desat_lime "\\[\\e[38;2;117;156;38m\\]"

#define cyan "\\[\\e[38;5;86m\\]"
#define blue "\\[\\e[38;5;4m\\]"
#define sky_blue "\\[\\e[38;5;117m\\]"
#define teal "\\[\\e[38;2;104;155;196m\\]"

#define orchid "\\[\\e[38;2;166;121;210m\\]"
#define light_purple "\\[\\e[38;5;182m\\]"
#define purple "\\[\\e[38;2;153;102;204m\\]"

#define khaki "\\[\\e[38;2;238;232;170m\\]"
#define white "\\[\\e[38;2;255;255;255m\\]"
#define beige "\\[\\e[38;2;239;239;200m\\]"

#endif
