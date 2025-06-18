#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>

#include "tokyobash.h"

bool shouldFetch() {

    /*
    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        printf("Error with FETCH_HEAD");
        return -1;
    }
    char fbuf[512];
    if (fgets(fbuf, sizeof(fbuf), fetch_status) == NULL) {
        pclose(fetch_status);
        printf("Error with fgets");
        return -1;
    }

    char c;
    int indx = 0;
    int nl = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
    char date[11];
    char time[9];

    for (int i = 0; i < 512; i++) {

        c = fbuf[i];

        if (c == EOF) break;

        if (c == '\n') {
            nl++;
            continue;
        }

        if (nl == 6 && c == ' ') {
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
            date[indx++] = c;
            if (indx == 10) {
                date[indx] = '\0';
                indx = 0;
            }
        }

        if (inTime) {
            time[indx++] = c;
            if (indx == 8) {
                time[indx] = '\0';
                break;
            }
        }
    }
    */
    return false;
}

int Fetched(int seconds) {

    if (shouldFetch()) {
        popen("git fetch 2>/dev/null", "r");
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

    pclose(file);
    int unstaged = atoi(buf);
    return unstaged;
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
