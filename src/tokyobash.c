#include "../include/tokyobash.h"

int main(void) {

    char path[PATH_MAX];
    getcwd(path, sizeof(path));

    if (getcwd(path, sizeof(path)) == NULL) {
        perror("tokyobash error: failed to retrieve path");
        printf("\\u@\\h: ");
        exit(-1);
    }

    char* pHome = getenv("HOME");
    if (pHome == NULL) {
        perror("tokyobash error: failed to retrieve $HOME");
        printf("\\u@\\h: ");
        exit(-1);
    }

    int pathLength = strlen(path);
    int homeLength = strlen(pHome);
    PathState pathState;

    if (strstr(path, pHome) != NULL) {
        replaceHome(path, pathLength, homeLength);
        pathLength = (pathLength - homeLength) + 1;
    }

    if (path[0] == '~') {
        pathState = Home;
    } else {
        if (strstr(path, "/mnt") != NULL) {
            pathState = Mnt;
        } else {
            pathState = Root;
        }
    }

    if (pathLength > ABV_PATH_LEN_T) {
        abrvPath(path, pathLength);
        pathLength = ABV_PATH_LEN_T;
    }

    ConfigSettings usrConfig;
    usrConfig.git = true;
    usrConfig.time = true;
    usrConfig.fetch = false;
    usrConfig.debug = false;
    usrConfig.inARepo = false;
    usrConfig.statusbar = false;
    usrConfig.branchname = true;
    usrConfig.background = false;
    usrConfig.theme = Tokyonight;
    usrConfig.fetchConfig.limit = 1;
    usrConfig.fetchConfig.modifier = Day;
    usrConfig.gitAccessible = false;
    usrConfig.gitAccessible = isGitAccessible();

    parseConfig(&usrConfig, pHome, homeLength);

    if (usrConfig.git && usrConfig.gitAccessible) {
        usrConfig.inARepo = checkIfInRepo();
    }

    Colors colorDefs;
    ColorPointers colors;
    colorDefinitions(&colorDefs);
    assignPointers(&colors, &colorDefs, &usrConfig);

    printUsrTime(&usrConfig, &colors);

    remCurntDir(path, pathLength);

    if ((!usrConfig.git || !usrConfig.gitAccessible) || !usrConfig.inARepo) {

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &colors, pathState, path, pathLength);
        } else {
            printPathNoBg(&colors, pathState, path, pathLength);
        }

    } else {

        if (usrConfig.branchname) {
            printBranch(&usrConfig, &colors);
        }

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &colors, pathState, path, pathLength);
        } else {
            printPathNoBg(&colors, pathState, path, pathLength);
        }

        if (usrConfig.statusbar || usrConfig.debug) {

            int untracked, unstaged, staged, committed, fetched;
            untracked = unstaged = staged = committed = fetched = 0;

            getStatusOf(&staged, &unstaged, &untracked);
            committed = Committed();

            if (usrConfig.fetch) {
                fetched = Fetched(&usrConfig.fetchConfig);
            }

            if (usrConfig.debug) {
                untracked = 2;
                unstaged = 3;
                staged = 3;
                committed = 3;
                fetched = 2;
            }

            if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 || committed > 0) {
                printStatusBar(&colors, untracked, unstaged, staged, committed, fetched);
            }
        }
    }
    printf("  %s%s┗>$ %s", colors.bold, colors.usr_color, colors.reset);
    return 0;
}
