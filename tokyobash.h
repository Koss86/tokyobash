#ifndef TOKYOBASH_H
#define TOKYOBASH_H

#define Tokyonight "tokyonight"
#define Catppuccin "catppuccin"

#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

bool in_home(char *path, char *home, int Hleng);
void replace_home(char *path, char *home, int leng, int indx);
void abrv_path(char *path, int leng);
void rem_curDir(char *path, int leng);
bool in_mnt(char *path);

#endif
