#include "../include/tokyobash.h"
#include "../include/colors.h"

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

    if (usrConfig.background) {

        switch (usrConfig.theme) {

            case Tokyonight:
                usr_color = cyan;
                path_color = blue;
                mnt_color = orange;
                root_color = pink;
                color_unstaged = orange;
                color_staged = orchid;
                color_committed = lime_green;
                color_fetched = dark_orange;

                txt_fg_color = black;
                usr_bg = cyan_bg;
                time_bg = teal_bg;
                branch_bg = sky_blue_bg;
                path_bg = blue_bg;

                usr_icon_color = cyan;
                time_icon_color = teal;
                branch_icon_color = sky_blue;
                path_icon_color = blue;
                break;

            case Catppuccin:
                usr_color = peach;
                time_color = rose;
                path_color = light_purple;
                mnt_color = blue;
                root_color = orange;
                color_unstaged = orange;
                color_staged = blue;
                color_committed = lime_green;
                color_fetched = pink;
                break;

            case Kanagawa:
                usr_color = desat_firebrick;
                time_color = teal;
                path_color = khaki;
                mnt_color = desat_lime;
                root_color = light_purple;
                color_unstaged = dark_orange;
                color_staged = purple;
                color_committed = green;
                color_fetched = pink;
                break;

            case Orange:
                usr_color = dark_orange;
                time_color = beige;
                path_color = white;
                mnt_color = orange;
                root_color = pink;
                color_unstaged = red;
                color_staged = blue;
                color_committed = green;
                color_fetched = red;
                break;
        }

    } else {

        switch (usrConfig.theme) {

            case Tokyonight:
                usr_color = cyan;
                time_color = sky_blue;
                path_color = blue;
                mnt_color = orange;
                root_color = pink;
                color_unstaged = orange;
                color_staged = orchid;
                color_committed = lime_green;
                color_fetched = dark_orange;
                break;

            case Catppuccin:
                usr_color = peach;
                time_color = rose;
                path_color = light_purple;
                mnt_color = blue;
                root_color = orange;
                color_unstaged = orange;
                color_staged = blue;
                color_committed = lime_green;
                color_fetched = pink;
                break;

            case Kanagawa:
                usr_color = desat_firebrick;
                time_color = teal;
                path_color = khaki;
                mnt_color = desat_lime;
                root_color = light_purple;
                color_unstaged = dark_orange;
                color_staged = purple;
                color_committed = green;
                color_fetched = pink;
                break;

            case Orange:
                usr_color = dark_orange;
                time_color = beige;
                path_color = white;
                mnt_color = orange;
                root_color = pink;
                color_unstaged = red;
                color_staged = blue;
                color_committed = green;
                color_fetched = red;
                break;
        }
    }

    if (usrConfig.background) {
        printf("%s%s%s\\u@\\h%s%s", usr_color, txt_fg_color, usr_bg, usr_color, reset);

    } else {
        printf("%s%s\\u@\\h%s: ", bold, usr_color, reset);
    }

    if (usrConfig.time) {
        if (usrConfig.background) {
            printf("%s%s%s%s\\t%s", usr_color, time_bg, txt_fg_color, time_bg, reset);
        } else {
            printf("%s[\\t] ", time_color);
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
            printPathWithBg(&usrConfig, pathState, path, pathLength);
        } else {

            printPathNoBg(pathState, path, pathLength);
        }

    } else {

        if (usrConfig.branchname) {

            char branch_name[MAX_BRANCH_LEN];
            getBranch(&branch_name[0]);

            if (usrConfig.background) {
                printf("%s%s %s%s%s  %s", branch_bg, time_icon_color, txt_fg_color, branch_bg,
                       branch_name, reset);
            } else {
                printf("%s%s%s ", path_color, branch_name, usr_color);
            }
        }

        if (usrConfig.background) {
            printPathWithBg(&usrConfig, pathState, path, pathLength);

        } else {
            printPathNoBg(pathState, path, pathLength);
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

                printf("  %s┗┳[%s", usr_color, reset);

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
                        printf("%s | ", usr_color);
                        ct--;
                    }
                }
                if (unstaged > 0) {
                    printf("%s%s %d", color_unstaged, reset, unstaged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", usr_color);
                        ct--;
                    }
                }
                if (staged > 0) {
                    printf("%s󱝣%s %d", color_staged, reset, staged);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", usr_color);
                        ct--;
                    }
                }
                if (committed > 0) {
                    printf("%s%s %d", color_committed, reset, committed);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", usr_color);
                        ct--;
                    }
                }
                if (fetched > 0) {
                    printf("%s%s %d", color_fetched, reset, fetched);

                    if (ct > 0 && ct != 1) {
                        printf("%s | ", usr_color);
                        ct--;
                    }
                }
                if (space) {
                    printf(" ");
                }
                printf("%s%s]\\n ", bold, usr_color);
            }
        }
    }
    printf("  %s%s┗>$ %s", bold, usr_color, reset);
    return 0;
}
