#include "../include/tokyobash.h"

int main(void) {

    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL) {
        perror("tokyobash error: failed to retrieve path");
        printf("\\u@\\h: \\w ");
        exit(-1);
    }

    char* pHome = getenv("HOME");
    if (pHome == NULL) {
        perror("tokyobash error: failed to retrieve $HOME");
        printf("\\u@\\h: \\w ");
        exit(-1);
    }

    ConfigSettings usrConfig;
    usrConfig.abv = true;
    usrConfig.time = true;
    usrConfig.fetch = false;
    usrConfig.debug = false;
    usrConfig.inARepo = false;
    usrConfig.statusbar = true;
    usrConfig.branchname = true;
    usrConfig.background = false;
    usrConfig.theme = Tokyonight;
    usrConfig.timeFormat[0] = 't';
    usrConfig.gitAccessible = false;
    usrConfig.fetchConfig.limit = 1;
    usrConfig.fetchConfig.modifier = Day;
    usrConfig.gitAccessible = isGitAccessible();
    strcpy(usrConfig.icon, "");

    parseConfig(&usrConfig, pHome);

    int pathLength = strlen(path);
    int homeLength = strlen(pHome);

    if (strstr(path, pHome) != NULL) {
        replaceHome(path, pathLength, homeLength);
        pathLength = (pathLength - homeLength) + 1;
    }

    if (path[0] == '~') {
        usrConfig.pathState = Home;
    } else {
        if (strstr(path, "/mnt") != NULL) {
            usrConfig.pathState = Mnt;
        } else {
            usrConfig.pathState = Root;
        }
    }

    if (usrConfig.abv && pathLength > ABV_PATH_LEN_T) {
        abrvPath(path, pathLength);
        pathLength = ABV_PATH_LEN_T;
    }

    if (pathLength > 1) {
        remCurntDir(path, pathLength);
    }

    if (usrConfig.gitAccessible) {
        usrConfig.inARepo = checkIfInRepo();
    }

    Colors colorDefs;
    defineColors(&usrConfig, &colorDefs);

    printUsrTime(&usrConfig, &colorDefs);

    if (!usrConfig.gitAccessible || !usrConfig.inARepo) {

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &colorDefs, path, pathLength);
        } else {
            printPathNoBg(&usrConfig, &colorDefs, path, pathLength);
        }

    } else {

        if (usrConfig.branchname) {
            printBranch(&usrConfig, &colorDefs);
        }

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &colorDefs, path, pathLength);
        } else {
            printPathNoBg(&usrConfig, &colorDefs, path, pathLength);
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

            if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 ||
                committed > 0) {
                printStatusBar(&colorDefs, untracked, unstaged, staged,
                               committed, fetched);
            }
        }
    }
    printf("  %s%s┗%s » %s", colorDefs.bold, colorDefs.usr_color,
           usrConfig.icon, colorDefs.reset);
    return 0;
}
