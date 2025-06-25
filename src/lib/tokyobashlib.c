#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/tokyobash.h"

void printpath(PathState pathstate, char *path, char *color_path, char *color_mnt, char *color_root, char *bold, int Plen) {

    switch (pathstate) {

        case noHome:
            printf("%s%s%s\\W/\\n", color_path, path, bold);
            break;

        case Home:
            if (Plen > 1) {
                printf("%s%s%s\\W/\\n", color_path, path, bold);
            } else {
                printf("%s%s\\W/\\n", color_path, bold);
            }
            break;

        case Mnt:
            printf("%s%s%s\\W/\\n", color_mnt, path, bold);
            break;

        case Root:
            if (Plen > 1) {
                printf("%s%s%s\\W/\\n", color_root, path, bold);
            } else {
                printf("%s%s\\W\\n", color_root, bold);
            }
            break;
    }
}

void parse_config(bool *debugsb, char *pHome, Themes *theme, bool *statusbar, bool *git) { 

    char path [PATH_MAX];
    char filepath[] = "/.config/tokyobash/config";
    int Hleng = strlen(pHome);
    int Fleng = 25;

    for (int i = 0; i < Hleng+Fleng; i++) {
        if (i < Hleng) {
            path[i] = pHome[i];
            continue;
        }
        path[i] = filepath[i-Hleng];
    }
    path[Hleng+Fleng] = '\0';

    FILE *config = fopen(path, "r");
    if (config == NULL) {
        perror("Tokyobash config not found");
        exit(-1);
    }

    char c;
    int inkey = 1;
    int invalue = 0;
    int indx = 0;
    char keybuf[64];
    char valbuf[64];

    while ((c = fgetc(config)) != EOF) {

        if (c == ' ' || c == '\'' || c == '"') continue;

        if ( c == '\n') {
            inkey = 1;
            invalue = 0;
            valbuf[indx] = '\0';
            indx = 0;
            if (valbuf[0] >= 'a' && valbuf[0] <= 'z') {
                if ((strncmp(keybuf, "theme", 5)) == 0) {

                    if ((strncmp(valbuf, "tokyonight", 10)) == 0) {
                        *theme = Tokyonight;
                    } else if ((strncmp(valbuf, "catppuccin", 10)) == 0) {
                        *theme = Catppuccin;
                    } else if ((strncmp(valbuf, "kanagawa", 8)) == 0) {
                        *theme = Kanagawa;
                    } else if ((strncmp(valbuf, "orange", 6)) == 0) {
                        *theme = Orange;
                    }
                } // add else if's here for future options with char vals.

            } else {

                if ((strncmp(keybuf, "statusbar", 9)) == 0) {

                    if (valbuf[0] == '0') {
                        *statusbar = false;
                    } else if (valbuf[0] == '1'){
                        *statusbar = true;
                    }
                } else if ((strncmp(keybuf, "git", 3)) == 0) {

                    if (valbuf[0] == '0') {
                        *git = false;
                    } else if ( valbuf[0] == '1') {
                        *git = true;
                    }
                } else if ((strncmp(keybuf, "debug", 5)) == 0) {

                    if (valbuf[0] == '0') {
                        *debugsb = false;
                    } else if (valbuf[0] == '1') {
                        *debugsb = true;
                    }
                } // add else if's here for future optoins with int vals.
            }
            continue;
        }
        if (c == '=') {
            invalue = 1;
            inkey = 0;
            keybuf[indx] = '\0';
            indx = 0;
            continue;
        }

        if (inkey) {
            keybuf[indx++] = c;
        }
        if (invalue) {
            valbuf[indx++] = c;
        }
    }
}
bool shouldFetch() {

    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    char cur_date[11];
    char cur_time[9];

    if (!strftime(cur_date, sizeof(cur_date), "%Y-%m-%d", time_struct))
    {
        return false;
    }
    if (!strftime(cur_time, sizeof(cur_time), "%X", time_struct))
    {
        return false;
    }

    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        return false;
    }

    char c;
    int indx = 0;
    int newline = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
    char fetch_date[11];
    char fetch_time[9];

    // If the return of 'stat .git/FETCH_HEAD' ever
    // changes, this will need to be refactored.
    while ((c = fgetc(fetch_status)) != EOF) {

        if (c == '\n') {
            newline++;
            continue;
        }

        if (newline == 6 && c == ' ') {
            space++;
            if (space == 1) {
                inDate = 1;
            } else if (space == 2) {
                inTime = 1;
                inDate = 0;
            }
            continue;
        }

        if (inDate) {
            fetch_date[indx++] = c;
            if (indx == 10) {
                fetch_date[indx] = '\0';
                indx = 0;
            }
        }

        if (inTime) {
            fetch_time[indx++] = c;
            if (indx == 8) {
                fetch_time[indx] = '\0';
                break;
            }
        }
    }

    pclose(fetch_status);

    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) return true; // Year
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) return true; // Month
    if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) return true; // Day
    if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) return true; // Hour

    if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) {            // Minute

        char fbuf[3];
        char cbuf[3];
        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[3+i];
            cbuf[i] = cur_time[3+i];
        }

        int i_fbuf = atoi(fbuf);
        int i_cbuf = atoi(cbuf);

        int diff = i_cbuf - i_fbuf;
        // Adjust to change time between fetches.
        if (diff > 44) {
            return true;
        }
    }
    return false;
}
// Checks when last time the repo was updated,
// if 45mins or longer, fetch is called. Then,
// whether fetch was called or not, it will return
// how many commits are ready to be pulled.
int Fetched() {

    if (shouldFetch()) {
        FILE *gitFetch = popen("git fetch 2>/dev/null", "r");
        pclose(gitFetch);
    }

    FILE *file = popen("git rev-list --count ..@{u} 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    return atoi(buf);
}
// Returns how many commits are ready to be pushed.
int Committed() {

    FILE *file;
    if ((file = popen("git rev-list --count @{u}.. 2>/dev/null", "r")) == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    return atoi(buf);
}
// Counts how many staged, unstaged, and untracked files in the repo
// on the current branch, then puts each value in the provided variables.
// !!Currently has a noticeable 1-2 second slow down in large repos like llvm.
void get_status_of(int *staged, int *unstaged, int *untracked) {
    FILE *file;
    if ((file = popen("git status --porcelain | grep -o '^..' 2>/dev/null", "r")) == NULL) {
        exit(-1);
    }
    char c;
    int state = 0;
    int st, unst, untr;
    st = unst = untr = 0;
    while ((c = fgetc(file)) != EOF) {

        if (c == '\n') {
            continue;
        }

        if (state == 0) {
            if (c == '?') {
                untr++;
            } else if (c == 'M' || c == 'A' || c == 'D' || c == 'R' || c == 'C') {
                st++;
            }
        } else {
            if (c == 'M' || c == 'D') {
                unst++;
            } //else if (c == 'U') to track files with merge conflicts.
        }

        if (state == 0) {
            state = 1;
        } else {
            state = 0;
        }
    }
    *staged = st;
    *unstaged = unst;
    *untracked = untr;
    pclose(file);
}
// If path contains $HOME, replace it with '~'.
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
// If path lenth is greater than 50, keep first 24 chars, add '...' then
// place last 23 chars after last '.' .
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

// Remove current directory from path. We add it back with \\W after
// changing text to bold. This way the path is normal, while current dir is highlighted.
void rem_curDir(char *path, int Plen) {

    for (int i = Plen - 1; i > -1; i--) {

        if (path[i] == '/') {
            path[i + 1] = '\0';
            break;
        }
    }
}

// If git is available, return true.
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

// If current directory is a repository, return true.
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

// Place branch name into provided buffer.
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
