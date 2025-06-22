#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "include/tokyobash.h"

int main(int argc, char **argv) {

    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    // Check to make sure we got the path
    if (getcwd(path, sizeof(path)) == NULL) {
        perror("tokyobash error: failed to retrieve path\\n");
        exit(1);
    }

    Themes theme;
    bool statusbar_enabled;
    bool git_enabled;
    char *pHome = getenv("HOME");
    if (pHome == NULL) {
        perror("tokyobash error: failed to retrieve $HOME\\n");
        exit(1);
    }

    parse_config(pHome, &theme, &statusbar_enabled, &git_enabled);

    char bold[] = "\\[\\e[1m\\]";
    char reset[] = "\\[\\e[00m\\]";
    char red[] = "\\[\\e[38;2;255;77;77m\\]";
    char light_red[] = "\\[\\e[38;5;211m\\]";
    char pink[] = "\\[\\e[38;5;217m\\]";
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

    char *color_usr = &reset[0];
    char *color_time = &reset[0];
    char *color_path = &reset[0];
    char *color_mnt = &reset[0];
    char *color_root = &reset[0];

    switch (theme) {

        case Tokyonight:
            color_usr = &cyan[0];
            color_time = &sky_blue[0];
            color_path = &blue[0];
            color_mnt = &orange[0];
            color_root = &light_red[0];
            break;

        case Catppuccin:
            color_usr = &peach[0];
            color_time = &pink[0];
            color_path = &light_purple[0];
            color_mnt = &blue[0];
            color_root = &orange[0];
            break;

        case Kanagawa:
            color_usr = &red[0];
            color_time = &teal[0];
            color_path = &khaki[0];
            color_mnt = &desat_lime[0];
            color_root = &light_purple[0];
            break;

        case Orange:
            color_usr = &dark_orange[0];
            color_time = &beige[0];
            color_path = &white[0];
            color_mnt = &orange[0];
            color_root = &light_red[0];
            break;
    }

    int Plen = strlen(path);
    PathState pathstate;

    if (pHome == NULL) {
        pathstate = noHome;

    } else {
        int Hlen = strlen(pHome);

        if (strstr(path, pHome) != NULL) {

            replace_home(path, pHome, Plen, Hlen);
            Plen = (Plen - Hlen) + 1;
        }

        if (path[0] == '~') {
            pathstate = Home;

        } else {

            if (strstr(path, "/mnt") != NULL) {
                pathstate = Mnt;

            } else {

                pathstate = Root;
            }
        }
    }

    if (Plen > ABV_PATH_LEN_T) {
        abrv_path(path, Plen);
        Plen = ABV_PATH_LEN_T;
    }

    printf("%s%s\\u@\\h%s:%s [\\t] ", bold, color_usr, reset, color_time);

    rem_curDir(path, Plen);

    if (git_enabled) {
        if (git_is_accessible() && in_repo()){

            char branch_name[MAX_BRANCH_LEN];
            get_branch(&branch_name[0]);

            printf("%s%s%s  ", color_path, branch_name, color_usr);

            printpath(pathstate, path, color_path, color_mnt, color_root, bold, Plen);

            if (statusbar_enabled) {

                int untracked, unstaged, staged, committed, fetched;
                untracked = unstaged = staged = committed = fetched = 0;

                get_status_of(&staged, &unstaged, &untracked);
                committed = Committed();
                fetched = Fetched();

                if (false) { // Change to true to test status bar.
                    untracked = 2;
                    unstaged = 3;
                    staged = 3;
                    committed = 3;
                    fetched = 2;
                }

                if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 || committed > 0) {

                    printf("  %s┗┳[%s", color_usr, reset);

                    // Pointers to assign colors of the icons for each theme.
                    char *color_untracked = &yellow[0];
                    char *color_unstaged = &reset[0];
                    char *color_staged = &reset[0];
                    char *color_committed = &reset[0];
                    char *color_fetched = &reset[0];

                    switch (theme) {

                        case Tokyonight:
                            color_unstaged = &orange[0];
                            color_staged = &orchid[0];
                            color_committed = &lime_green[0];
                            color_fetched = &dark_orange[0];
                            break;

                        case Catppuccin:
                            color_unstaged = &orange[0];
                            color_staged = &blue[0];
                            color_committed = &lime_green[0];
                            color_fetched = &light_red[0];
                            break;

                        case Kanagawa:
                            color_unstaged = &dark_orange[0];
                            color_staged = &purple[0];
                            color_committed = &green[0];
                            color_fetched = &light_red[0];
                            break;

                        case Orange:
                            color_unstaged = &orange[0];
                            color_staged = &blue[0];
                            color_committed = &green[0];
                            color_fetched = &red[0];
                            break;
                    }

                    int ct = 0;
                    if (untracked > 0)  ct++;
                    if (unstaged > 0)  ct++;
                    if (staged > 0)  ct++;
                    if (committed > 0)  ct++;
                    if (fetched > 0)  ct++;


                    if (untracked > 0) {
                        printf(" %s%s %d ", color_untracked, reset, untracked);
                        // We stop printing the separators at ct = 1
                        // because we want the last print to be ']' not '|'.
                        if (ct > 0 && ct != 1)  {
                            printf("%s|", color_usr);
                            ct--;
                        }
                    }
                    if (unstaged > 0) {
                        printf(" %s%s %d ", color_unstaged, reset, unstaged);
                        if (ct > 0 && ct != 1)  {
                            printf("%s|", color_usr);
                            ct--;
                        }
                    }
                    if (staged > 0) {
                        printf(" %s󱝣%s %d ", color_staged, reset, staged);

                        if (ct > 0 && ct != 1)  {
                            printf("%s|", color_usr);
                            ct--;
                        }
                    }
                    if (committed > 0) {
                        printf(" %s%s %d ", color_committed, reset, committed);

                        if (ct > 0 && ct != 1)  {
                            printf("%s|", color_usr);
                            ct--;
                        }
                    }
                    if (fetched > 0) {
                        printf(" %s%s %d ", color_fetched, reset, fetched);

                        if (ct > 0 && ct != 1)  {
                            printf("%s|", color_usr);
                            ct--;
                        }
                    }
                    printf("%s]\\n ", color_usr);
                }
            }
        } else { // Not in repo

            printpath(pathstate, path, color_path, color_mnt, color_root, bold, Plen);

        }

    } else { // If git is disabled

        printpath(pathstate, path, color_path, color_mnt, color_root, bold, Plen);
    }
    printf("  %s%s┗>$ %s", bold, color_usr, reset);
    return 0;
}
