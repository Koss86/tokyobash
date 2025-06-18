#ifndef TOKYOBASH_H
#define TOKYOBASH_H

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

#define bold "\\[\\e[1m\\]"
#define reset "\\[\\e[00m\\]"
#define sky_blue "\\[\\e[38;5;117m\\]"
#define yellow "\\[\\e[38;2;255;255;0m\\]"
#define green "\\[\\e[38;2;0;255;0m\\]"

#define cyan "\\[\\e[38;5;86m\\]"
#define blue "\\[\\e[38;5;4m\\]"
#define peach "\\[\\e[38;5;223m\\]"
#define light_red "\\[\\e[38;5;211m\\]"

#define pink "\\[\\e[38;5;217m\\]"
#define purple "\\[\\e[38;5;182m\\]"
#define orange "\\[\\e[38;5;214m\\]"

#define red "\\[\\e[38;2;255;77;77m\\]"
#define teal "\\[\\e[38;2;104;155;196m\\]"
#define khaki "\\[\\e[38;2;238;232;170m\\]"
#define lime "\\[\\e[38;2;117;156;38m\\]"

#define dark_orange "\\[\\e[38;2;255;149;20m\\]"
#define beige "\\[\\e[38;2;239;239;200m\\]"
#define white "\\[\\e[38;2;255;255;255m\\]"



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
int Fetched();

#endif
