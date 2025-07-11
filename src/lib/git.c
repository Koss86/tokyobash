#include "../../include/tokyobash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// If git is available, return true.
bool git_is_accessible(void) {

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
bool in_repo(void) {

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
// Returns how many commits are ready to be pushed.
int Committed(void) {

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
            } // else if (c == 'U') to track files with merge conflicts.
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
