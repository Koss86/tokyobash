#include "../../include/tokyobash.h"

bool isGitAccessible(void) {

    FILE* file;
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
bool checkIfInRepo(void) {

    FILE* file = popen("git rev-parse --is-inside-work-tree 2>/dev/null", "r");
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
void getBranch(char* branch_name) {

    FILE* file = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (file == NULL) {
        strcpy(branch_name, "Unknown");
        return;
    }

    if (fgets(branch_name, MAX_BRANCH_LEN, file) == NULL) {
        strcpy(branch_name, "Unknown");
        pclose(file);
        return;
    }

    int len = strlen(branch_name);
    if (len > 0 && branch_name[len - 1] == '\n') {
        branch_name[len - 1] = '\0';
    }
    pclose(file);
    return;
}
int Committed(void) {

    FILE* file;
    if ((file = popen("git rev-list --count @{u}.. 2>/dev/null", "r")) ==
        NULL) {
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
void getStatusOf(int* staged, int* unstaged, int* untracked) {
    FILE* file =
        popen("git status --porcelain | grep -o '^..' 2>/dev/null", "r");
    if (file == NULL) {
        return;
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
            } else if (c == 'M' || c == 'A' || c == 'D' || c == 'R' ||
                       c == 'C') {
                st++;
            }
            state = 1;

        } else {

            if (c == 'M' || c == 'D') {
                unst++;
            }
            state = 0;
        }
    }
    *staged = st;
    *unstaged = unst;
    *untracked = untr;
    pclose(file);
    return;
}
int Fetched(FetchOpts* fetchConfig) {

    if (shouldFetch(fetchConfig)) {
        FILE* gitFetch = popen("git fetch 2>/dev/null", "r");
        if (gitFetch == NULL) {
            return 0;
        }
        pclose(gitFetch);
    }

    FILE* file = popen("git rev-list --count ..@{u} 2>/dev/null", "r");
    if (file == NULL) {
        return 0;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return 0;
    }

    pclose(file);
    return atoi(buf);
}
