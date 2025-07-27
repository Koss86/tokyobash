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
    usrConfig.background = 0;
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
    char teal[] = "\\[\\e[38;5;37m\\]";
    char orchid[] = "\\[\\e[38;2;166;121;210m\\]";
    char light_purple[] = "\\[\\e[38;5;182m\\]";
    char purple[] = "\\[\\e[38;2;153;102;204m\\]";
    char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
    char white[] = "\\[\\e[38;2;255;255;255m\\]";
    char black[] = "\\[\\e[0;30m\\]";
    char beige[] = "\\[\\e[38;2;239;239;200m\\]";

    char blue_bg[] = "\\[\\e[48;5;4m\\]";
    char sky_blue_bg[] = "\\[\\e[48;5;117m\\]";
    char cyan_bg[] = "\\[\\e[48;5;86m\\]";
    char teal_bg[] = "\\[\\e[48;5;37m\\]";

    char* usr_color = reset;
    char* time_color = reset;
    char* path_color = reset;
    char* mnt_color = reset;
    char* root_color = reset;

    char* color_untracked = yellow;
    char* color_unstaged = reset;
    char* color_staged = reset;
    char* color_committed = reset;
    char* color_fetched = reset;

    char* txt_fg_color = reset;
    char* usr_bg = reset;
    char* time_bg = reset;
    char* branch_bg = reset;
    char* path_bg = reset;
    // char* mnt_bg = reset;
    // char* root_bg = reset;

    if (usrConfig.background) {

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

                txt_fg_color = black;
                usr_bg = cyan_bg;
                branch_bg = teal_bg;
                time_bg = sky_blue_bg;
                path_bg = blue_bg;
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
            printf("%s%s%s%s\\t%s", time_bg, usr_color, txt_fg_color, time_bg, reset);
        } else {
            printf("%s[\\t] ", time_color);
        }
    }

    remCurntDir(path, pathLength);

    if ((!usrConfig.git || !isGitAccessible()) || !inRepo()) {
        // Skip branch name and status bar and just print the path.
        switch (pathState) {

            case Home:
                if (pathLength > 1) {
                    printf(" %s%s%s\\W/\\n", path_color, path, bold);
                } else {
                    printf(" %s%s\\W/\\n", path_color, bold);
                }
                break;

            case Mnt:
                printf(" %s%s%s\\W/\\n", mnt_color, path, bold);
                break;

            case Root:
                if (pathLength > 1) {
                    printf(" %s%s%s\\W/\\n", root_color, path, bold);
                } else {
                    printf(" %s%s\\W\\n", root_color, bold);
                }
                break;
        }

    } else {

        if (usrConfig.branchname) {
            char branch_name[MAX_BRANCH_LEN];
            getBranch(&branch_name[0]);
            if (usrConfig.background) {
                printf("%s%s%s%s %s%s  %s%s", time_bg, teal, txt_fg_color, branch_bg,
                       branch_name, usr_color, reset, teal);
            } else {
                printf(" %s%s%s ", path_color, branch_name, usr_color);
            }
        }

        switch (pathState) {

            case Home:
                if (pathLength > 1) {
                    printf(" %s%s%s\\W/\\n", path_color, path, bold);
                } else {
                    printf(" %s%s\\W/\\n", path_color, bold);
                }
                break;

            case Mnt:
                printf(" %s%s%s\\W/\\n", mnt_color, path, bold);
                break;

            case Root:
                if (pathLength > 1) {
                    printf(" %s%s%s\\W/\\n", root_color, path, bold);
                } else {
                    printf(" %s%s\\W\\n", root_color, bold);
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
