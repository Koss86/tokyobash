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
    usrConfig.git = 1;
    usrConfig.time = 1;
    usrConfig.debug = 0;
    usrConfig.statusbar = 0;
    usrConfig.branchname = 1;
    usrConfig.background = 1;
    usrConfig.theme = Tokyonight;
    usrConfig.fetchConfig.limit = 1;
    usrConfig.fetchConfig.modifier = Day;

    parseConfig(&usrConfig, pHome, homeLength);

    char bold[] = "\\[\\e[1m\\]";
    char reset[] = "\\[\\e[00m\\]";
    char red[] = "\\[\\e[38;2;255;77;77m\\]";
    char desat_firebrick[] = "\\[\\e[38;2;191;72;72m\\]";
    char pink[] = "\\[\\e[38;5;211m\\]";
    char rose[] = "\\[\\e[38;5;217m\\]";
    char peach[] = "\\[\\e[38;5;223m\\]";
    char dark_orange[] = "\\[\\e[38;2;255;149;20m\\]";
    char orange[] = "\\[\\e[38;5;214m\\]";
    char yellow[] = "\\[\\e[38;2;255;255;0m\\]";
    char green[] = "\\[\\e[38;2;0;255;0m\\]";
    char lime_green[] = "\\[\\e[38;2;172;255;47m\\]";
    char desat_lime[] = "\\[\\e[38;2;117;156;38m\\]";
    char cyan[] = "\\[\\e[38;5;86m\\]";
    char blue[] = "\\[\\e[38;5;4m\\]";
    char sky_blue[] = "\\[\\e[38;5;117m\\]";
    char teal[] = "\\[\\e[38;2;104;155;196m\\]";
    char orchid[] = "\\[\\e[38;2;166;121;210m\\]";
    char light_purple[] = "\\[\\e[38;5;182m\\]";
    char purple[] = "\\[\\e[38;2;153;102;204m\\]";
    char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
    char white[] = "\\[\\e[38;2;255;255;255m\\]";
    char beige[] = "\\[\\e[38;2;239;239;200m\\]";

    char* color_usr = &reset[0];
    char* color_time = &reset[0];
    char* color_path = &reset[0];
    char* color_mnt = &reset[0];
    char* color_root = &reset[0];

    char* color_untracked = &yellow[0];
    char* color_unstaged = &reset[0];
    char* color_staged = &reset[0];
    char* color_committed = &reset[0];
    char* color_fetched = &reset[0];

    switch (usrConfig.theme) {

        case Tokyonight:
            color_usr = &cyan[0];
            color_time = &sky_blue[0];
            color_path = &blue[0];
            color_mnt = &orange[0];
            color_root = &pink[0];
            color_unstaged = &orange[0];
            color_staged = &orchid[0];
            color_committed = &lime_green[0];
            color_fetched = &dark_orange[0];
            break;

        case Catppuccin:
            color_usr = &peach[0];
            color_time = &rose[0];
            color_path = &light_purple[0];
            color_mnt = &blue[0];
            color_root = &orange[0];
            color_unstaged = &orange[0];
            color_staged = &blue[0];
            color_committed = &lime_green[0];
            color_fetched = &pink[0];
            break;

        case Kanagawa:
            color_usr = &desat_firebrick[0];
            color_time = &teal[0];
            color_path = &khaki[0];
            color_mnt = &desat_lime[0];
            color_root = &light_purple[0];
            color_unstaged = &dark_orange[0];
            color_staged = &purple[0];
            color_committed = &green[0];
            color_fetched = &pink[0];
            break;

        case Orange:
            color_usr = &dark_orange[0];
            color_time = &beige[0];
            color_path = &white[0];
            color_mnt = &orange[0];
            color_root = &pink[0];
            color_unstaged = &red[0];
            color_staged = &blue[0];
            color_committed = &green[0];
            color_fetched = &red[0];
            break;
    }

    if (usrConfig.background) {
        printf("%s%s\\u@\\h%s: ", bold, color_usr, reset);
    } else {
        printf("%s%s\\u@\\h%s: ", bold, color_usr, reset);
    }

    if (usrConfig.time) {
        printf("%s[\\t] ", color_time);
    }

    remCurntDir(path, pathLength);

    if ((!usrConfig.git || !isGitAccessible()) || !inRepo()) {
        // Skip branch name and status bar and just print the path.
        switch (pathState) {

            case Home:
                if (pathLength > 1) {
                    printf("%s%s%s\\W/\\n", color_path, path, bold);
                } else {
                    printf("%s%s\\W/\\n", color_path, bold);
                }
                break;

            case Mnt:
                printf("%s%s%s\\W/\\n", color_mnt, path, bold);
                break;

            case Root:
                if (pathLength > 1) {
                    printf("%s%s%s\\W/\\n", color_root, path, bold);
                } else {
                    printf("%s%s\\W\\n", color_root, bold);
                }
                break;
        }

    } else {

        if (usrConfig.branchname) {
            char branch_name[MAX_BRANCH_LEN];
            getBranch(&branch_name[0]);
            printf("%s%s%s  ", color_path, branch_name, color_usr);
        }

        switch (pathState) {

            case Home:
                if (pathLength > 1) {
                    printf("%s%s%s\\W/\\n", color_path, path, bold);
                } else {
                    printf("%s%s\\W/\\n", color_path, bold);
                }
                break;

            case Mnt:
                printf("%s%s%s\\W/\\n", color_mnt, path, bold);
                break;

            case Root:
                if (pathLength > 1) {
                    printf("%s%s%s\\W/\\n", color_root, path, bold);
                } else {
                    printf("%s%s\\W\\n", color_root, bold);
                }
                break;
        }

        if (usrConfig.statusbar || usrConfig.debug) {

            int untracked, unstaged, staged, committed, fetched;
            untracked = unstaged = staged = committed = fetched = 0;

            getStatusOf(&staged, &unstaged, &untracked);
            committed = Committed();
            fetched = Fetched(&usrConfig.fetchConfig);

            if (usrConfig.debug) {
                untracked = 2;
                unstaged = 3;
                staged = 3;
                committed = 3;
                fetched = 2;
            }

            if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 || committed > 0) {

                printf("  %s┗┳[%s", color_usr, reset);

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

                int space = 0;
                if (ct > 1) {
                    // Empty space used here and at end for a little
                    // separation if more than 1 item populates statusbar.
                    printf(" ");
                    space++;
                }
                if (untracked > 0) {
                    printf("%s%s %d", color_untracked, reset, untracked);
                    // We stop printing the dividers at ct = 1
                    // because we want the last print to be ']' not '|'.
                    if (ct > 0 && ct != 1) {
                        printf("%s | ", color_usr);
                        ct--;
                    }
                }
                if (unstaged > 0) {
                    printf("%s%s %d", color_unstaged, reset, unstaged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", color_usr);
                        ct--;
                    }
                }
                if (staged > 0) {
                    printf("%s󱝣%s %d", color_staged, reset, staged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", color_usr);
                        ct--;
                    }
                }
                if (committed > 0) {
                    printf("%s%s %d", color_committed, reset, committed);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", color_usr);
                        ct--;
                    }
                }
                if (fetched > 0) {
                    printf("%s%s %d", color_fetched, reset, fetched);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", color_usr);
                        ct--;
                    }
                }
                if (space) {
                    printf(" ");
                }
                printf("%s%s]\\n ", bold, color_usr);
            }
        }
    }
    printf("  %s%s┗>$ %s", bold, color_usr, reset);
    return 0;
}
