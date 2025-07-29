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
    usrConfig.debug = false;
    usrConfig.inARepo = false;
    usrConfig.statusbar = false;
    usrConfig.branchname = true;
    usrConfig.background = false;
    usrConfig.theme = Tokyonight;
    usrConfig.fetchConfig.limit = 1;
    usrConfig.fetchConfig.modifier = Day;
    usrConfig.gitAccessible = isGitAccessible();

    parseConfig(&usrConfig, pHome, homeLength);

    if (usrConfig.git && usrConfig.gitAccessible) {
        usrConfig.inARepo = checkIfInRepo();
    }

    Colors colors;
    ColorPointers pointers;
    populateColors(&colors);
    assignPointers(&pointers, &colors, &usrConfig);

    if (usrConfig.background) {
        printf("%s%s%s\\u@\\h%s%s", pointers.usr_color, pointers.txt_fg_color, pointers.usr_bg,
               pointers.usr_color, pointers.reset);

    } else {
        printf("%s%s\\u@\\h%s: ", pointers.bold, pointers.usr_color, pointers.reset);
    }

    if (usrConfig.time) {
        if (usrConfig.background) {
            printf("%s%s%s%s\\t%s", pointers.usr_color, pointers.time_bg, pointers.txt_fg_color,
                   pointers.time_bg, pointers.reset);
        } else {
            printf("%s[\\t] ", pointers.time_color);
        }
    } else {
        if (usrConfig.background) {
            // printf("%s%s", );
        } else {
        }
    }

    remCurntDir(path, pathLength);

    if ((!usrConfig.git || !usrConfig.gitAccessible) || !usrConfig.inARepo) {

        // Skip branch name and status bar and just print the path.
        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &pointers, pathState, path, pathLength);
        } else {
            printPathNoBg(&pointers, pathState, path, pathLength);
        }

    } else {

        if (usrConfig.branchname) {

            char branch_name[MAX_BRANCH_LEN];
            getBranch(&branch_name[0]);

            if (usrConfig.background) {
                if (usrConfig.time) {
                    printf("%s%s%s%s%s %s", pointers.branch_bg, pointers.time_color,
                           pointers.txt_fg_color, pointers.branch_bg, branch_name, pointers.reset);
                } else {
                    printf("%s%s%s%s%s %s", pointers.branch_bg, pointers.usr_color,
                           pointers.txt_fg_color, pointers.branch_bg, branch_name, pointers.reset);
                }
            } else {
                printf("%s%s%s ", pointers.path_color, branch_name, pointers.usr_color);
            }
        }

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, &pointers, pathState, path, pathLength);

        } else {
            printPathNoBg(&pointers, pathState, path, pathLength);
        }

        if (usrConfig.statusbar || usrConfig.debug) {

            int untracked, unstaged, staged, committed, fetched;
            untracked = unstaged = staged = committed = fetched = 0;

            getStatusOf(&staged, &unstaged, &untracked);
            committed = Committed();
            // TODO: Add config setting to be able to enable and disable.
            fetched = Fetched(&usrConfig.fetchConfig);

            if (usrConfig.debug) {
                untracked = 2;
                unstaged = 3;
                staged = 3;
                committed = 3;
                fetched = 2;
            }

            if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 || committed > 0) {

                printf("  %s┗┳[%s", pointers.usr_color, pointers.reset);

                int ct = 0;
                if (untracked > 0)
                    ct++;
                if (unstaged > 0)
                    ct++;
                if (staged > 0)
                    ct++;
                if (committed > 0)
                    ct++;
                if (fetched > 0)
                    ct++;

                // Empty space used here and at end for a little
                // separation if more than 1 item populates statusbar.
                int space = 0;
                if (ct > 1) {
                    printf(" ");
                    space++;
                }
                if (untracked > 0) {
                    printf("%s%s %d", pointers.untracked_color, pointers.reset, untracked);
                    // We stop printing the dividers at ct = 1
                    // because we want the last print to be ']' not '|'.
                    if (ct > 0 && ct != 1) {
                        printf("%s | ", pointers.usr_color);
                        ct--;
                    }
                }
                if (unstaged > 0) {
                    printf("%s%s %d", pointers.unstaged_color, pointers.reset, unstaged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", pointers.usr_color);
                        ct--;
                    }
                }
                if (staged > 0) {
                    printf("%s󱝣%s %d", pointers.staged_color, pointers.reset, staged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", pointers.usr_color);
                        ct--;
                    }
                }
                if (committed > 0) {
                    printf("%s%s %d", pointers.committed_color, pointers.reset, committed);
                    if (ct > 0 && ct != 1) {
                        printf("%s | ", pointers.usr_color);
                        ct--;
                    }
                }
                if (fetched > 0) {
                    printf("%s%s %d", pointers.fetched_color, pointers.reset, fetched);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", pointers.usr_color);
                        ct--;
                    }
                }
                if (space) {
                    printf(" ");
                }
                printf("%s%s]\\n ", pointers.bold, pointers.usr_color);
            }
        }
    }
    printf("  %s%s┗>$ %s", pointers.bold, pointers.usr_color, pointers.reset);
    return 0;
}
