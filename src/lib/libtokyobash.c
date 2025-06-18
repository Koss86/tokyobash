#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tokyobash.h"

bool shouldFetch() {

    char cdate[11];
    char ctime[9];
    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    if (!strftime(cdate, sizeof(cdate), "%Y-%m-%d", time_struct))
    {
        return false;
    }
    if (!strftime(ctime, sizeof(ctime), "%X", time_struct))
    {
        return false;
    }

    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        return false;
    }

    char c;
    int indx = 0;
    int nl = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
    char fdate[11];
    char ftime[9];

    // If the return of 'stat .git/FETCH_HEAD' ever
    // changes, this will need to be reworked.
    while ((c = fgetc(fetch_status)) != EOF) {

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
            fdate[indx++] = c;
            if (indx == 10) {
                fdate[indx] = '\0';
                indx = 0;
            }
        }
        if (inTime) {
            ftime[indx++] = c;
            if (indx == 8) {
                ftime[indx] = '\0';
                break;
            }
        }
    }
    //printf("%s %s\n", ftime, ctime);

    if (fdate[2] != cdate[2] || fdate[3] != cdate[3]) return true; // Year
    if (fdate[5] != cdate[5] || fdate[6] != cdate[6]) return true; // Month
    if (fdate[8] != cdate[8] || fdate[9] != cdate[9]) return true; // Day
    if (ftime[0] != ctime[0] || ftime[1] != ctime[1]) return true; // Hour
    if (ftime[3] != ctime[3] || ftime[4] != ctime[4]) {            // Minute

        char fbuf[3];
        char cbuf[3];

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = ftime[3+i];
            cbuf[i] = ctime[3+i];
        }

        int i_fbuf = atoi(fbuf);
        int i_cbuf = atoi(cbuf);

        int diff = i_cbuf - i_fbuf;
        if (diff > 29) {
            return true;
        }
    }
    //printf("\\n%s %s\\n%s %s\\n", fdate, ftime, cdate, ctime);
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
