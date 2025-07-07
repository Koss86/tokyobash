#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tokyobash.h"

void parse_config(ConfigSettings *usrConfig, char *pHome, int Hleng) { 

    char path [PATH_MAX];
    char *pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[PATH_MAX] = "/tokyobash/config";

    int Flen = 17;
    if (pConfig == NULL || pConfig[0] == '\0') {
        Flen = 25;
        strncpy(filepath, "/.config/tokyobash/config", Flen);

        for (int i = 0; i < Hleng+Flen; i++) {
            if (i < Hleng) {
                path[i] = pHome[i];
                continue;
            }
            path[i] = filepath[i-Hleng];
        }
        path[Hleng+Flen] = '\0';

    } else {

        int Cleng = strlen(pConfig);
        // This check may not be needed but it's here just in case.
        if (pConfig[Cleng-1] == '/') {
            pConfig[Cleng-1] = '\0';
            Cleng--;
        }

        for (int i = 0; i < Cleng+Flen; i++) {
            if (i < Cleng) {
                path[i] = pConfig[i];
                continue;
            }
            path[i] = filepath[i-Cleng];
        }
        path[Cleng+Flen] = '\0';
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    char c;
    int inkey = 1;
    int invalue = 0;
    int incomment = 0;
    int indx = 0;
    char keybuf[64];
    char valbuf[64];
    char fetchbuf[3];
    Fetchtimer fetchTimeState;

    while ((c = fgetc(file)) != EOF) {

        if (c == ' ' || c == '\'' || c == '"') continue;
        if ( c == '#') { 
            incomment = 1;
            continue;
        }

        if ( c == '\n') {
            if (incomment) {
                incomment = 0;
                continue;
            }
            inkey = 1;
            invalue = 0;
            valbuf[indx] = '\0';
            indx = 0;
            if (valbuf[0] >= 'a' && valbuf[0] <= 'z') {

                if ((strncmp(keybuf, "theme", 5)) == 0) {

                    if ((strncmp(valbuf, "tokyonight", 10)) == 0) {
                        usrConfig->theme = Tokyonight;

                    } else if ((strncmp(valbuf, "catppuccin", 10)) == 0) {
                        usrConfig->theme = Catppuccin;

                    } else if ((strncmp(valbuf, "kanagawa", 8)) == 0) {
                        usrConfig->theme = Kanagawa;

                    } else if ((strncmp(valbuf, "orange", 6)) == 0) {
                        usrConfig->theme = Orange;
                    }
                } // add else if's here for future options with char vals.

            } else {

                if ((strncmp(keybuf, "statusbar", 9)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->statusbar = false;
                    } else if (valbuf[0] == '1'){
                        usrConfig->statusbar = true;
                    }

                } else if ((strncmp(keybuf, "git", 3)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->git = false;
                    } else if ( valbuf[0] == '1') {
                        usrConfig->git = true;
                    }

                } else if ((strncmp(keybuf, "branchname", 10)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->branchname = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->branchname = true;
                    }

                } else if ((strncmp(keybuf, "fetchtimer", 10)) == 0) {

                    if (valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'm' || valbuf[2] == 'm') {

                        usrConfig->fetchSettings.state = Minute;

                        if (valbuf[2] == 'm') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                            if (usrConfig->fetchSettings.amount > 60) {
                                usrConfig->fetchSettings.amount = 60;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                        }

                    } else if (valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'h' || valbuf[2] == 'h') {

                        usrConfig->fetchSettings.state = Hour;

                        if (valbuf[2] == 'h') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                            if (usrConfig->fetchSettings.amount > 24) {
                                usrConfig->fetchSettings.amount = 24;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                        }

                    } else if (valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'd' || valbuf[2] == 'd') {

                        usrConfig->fetchSettings.state = Day;

                        if (valbuf[2] == 'd') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                            if (usrConfig->fetchSettings.amount > 31) {
                                usrConfig->fetchSettings.amount = 31;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchSettings.amount = atoi(fetchbuf);
                        }

                    } else {
                        // Justin Case
                        usrConfig->fetchSettings.state = Hour;
                        usrConfig->fetchSettings.amount = 1;
                    }

                } else if ((strncmp(keybuf, "time", 4)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->time = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->time = true;
                    }

                } // add else if's here for future optoins with int vals.

                else if ((strncmp(keybuf, "debug", 5)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->debug = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->debug = true;
                    }
                }
            }
            continue;
        }

        if (incomment) { continue; }

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
bool shouldFetch(FetchOpts *fetchSettings) {

    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    char cur_date[11];
    char cur_time[9];

    if (!strftime(cur_date, sizeof(cur_date), "%Y-%m-%d", time_struct)) {
        return false;
    }

    if (!strftime(cur_time, sizeof(cur_time), "%X", time_struct)) {
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

        // Date and time we want are on the 7th line,
        // so after 6 \n we are on the line we want to be.
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

    char fbuf[3];
    char cbuf[3];
    int i_fbuf;
    int i_cbuf;
    int dayDif;
    int hrDif;
    int minDif;

    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) return true; // Year
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) return true; // Month

    if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) {  // Day

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_date[8+i];
            cbuf[i] = cur_date[8+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        dayDif = i_cbuf - i_fbuf;

        if (fetchSettings->state == Day) {

            if (dayDif >= fetchSettings->amount) {
                // If dif is same as what user set, we need
                // to check for false positives.
                if (dayDif == fetchSettings->amount) {

                    for (int i = 0; i < 3; i++) {
                        if (i == 2) {
                            fbuf[i] = '\0';
                            cbuf[i] = '\0';
                            break;
                        }
                        fbuf[i] = fetch_time[0+i];
                        cbuf[i] = cur_time[0+i];
                    }
                    i_fbuf = atoi(fbuf);
                    i_cbuf = atoi(cbuf);

                    hrDif = (24 - i_fbuf) + i_cbuf;

                    if (hrDif >= 24) {

                        for (int i = 0; i < 3; i++) {
                            if (i == 2) {
                                fbuf[i] = '\0';
                                cbuf[i] = '\0';
                                break;
                            }
                            fbuf[i] = fetch_time[3+i];
                            cbuf[i] = cur_time[3+i];
                        }
                        i_fbuf = atoi(fbuf);
                        i_cbuf = atoi(cbuf); 

                        minDif = (60 - i_fbuf) + i_cbuf;

                        if (minDif >= 60) {
                            return true;
                        } else { return false; }

                    } else { return false; }

                } else { return true; }

            } else { return false; }

        } else if (dayDif > 1) { return true; }
    }

    if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) { // Hour

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[0+i];
            cbuf[i] = cur_time[0+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        hrDif = (24 - i_fbuf) + i_cbuf;

        if (fetchSettings->state == Hour) {

            if (hrDif >= fetchSettings->amount) {

                if (hrDif == fetchSettings->amount) {
                    for (int i = 0; i < 3; i++) {
                        if (i == 2) {
                            fbuf[i] = '\0';
                            cbuf[i] = '\0';
                            break;
                        }
                        fbuf[i] = fetch_time[3+i];
                        cbuf[i] = cur_time[3+i];
                    }
                    i_fbuf = atoi(fbuf);
                    i_cbuf = atoi(cbuf); 

                    minDif = (60 - i_fbuf) + i_cbuf;
                    if (minDif >= 60) {
                        return true;
                    } else { return false; }

                } else { return true; }

            } else { return false; }

        } else if (hrDif > 1) { return true; }
    }

    if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) {  // Minute

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[3+i];
            cbuf[i] = cur_time[3+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        if (((60 - i_fbuf) + i_cbuf) >= fetchSettings->amount) {
            return true;
        }
    }

    return false;
}
// Checks when last time the repo was updated,
// calls fetch if longer than user specified. Then,
// returns number of commits if any from remote.
int Fetched(FetchOpts *fetchSettings) {

    if (shouldFetch(fetchSettings)) {
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
void get_branch(char *branch_name) {

    FILE *file = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (file == NULL) {
        exit(-1);
    }

    if (fgets(branch_name, MAX_BRANCH_LEN, file) == NULL) {
        pclose(file);
        exit(-1);
    }

    int len = strlen(branch_name);
    if (len > 0 && branch_name[len - 1] == '\n') {

        branch_name[len - 1] = '\0';
    }
    pclose(file);
}
