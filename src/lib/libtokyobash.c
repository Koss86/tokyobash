#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tokyobash.h"

int Untracked() {

    FILE *file = popen("git ls-files --others --exclude-standard | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    int untracked = atoi(buf);
    return untracked;
}

bool shouldFetch() {

    char cur_date[11];
    char cur_time[9];
    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    if (!strftime(cur_date, sizeof(cur_date), "%Y-%m-%d", time_struct))
    {
        return false;
    }

    if (!strftime(cur_time, sizeof(cur_time), "%X", time_struct))
    {
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

    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        return false;
    }

    // If the return of 'stat .git/FETCH_HEAD' ever
    // changes, this will need to be reworked.
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
    //printf("%s %s\n", fetch_time, cur_time);
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
        if (diff > 44) {
            pclose(fetch_status);
            return true;
        }
    }
    pclose(fetch_status);
    return false;
}

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
    int fetched = atoi(buf);
    return fetched;
}

int Unstaged() {

    FILE *file = popen("git diff --name-only | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    int unstaged = atoi(buf);
    pclose(file);
    return unstaged;
}

int Staged() {

    FILE *file = popen("git diff --cached --name-only | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    int staged = atoi(buf);
    return staged;
}

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
    int committed = atoi(buf);
    return committed;
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
