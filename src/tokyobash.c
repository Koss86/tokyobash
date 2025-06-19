#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/tokyobash.h"

int main(int argc, char **argv) {

    char path[PATH_MAX];
    getcwd(path, sizeof(path));
    // Check to make sure we got the path
    if (getcwd(path, sizeof(path)) == NULL) {
        perror("Error: getcwd() failed to retrieve path\\n");
        exit(1);
    }

    // Assign tokyonight colors as default.
    char *clr_usr = &cyan[0];
    char *clr_time = &sky_blue[0];
    char *clr_path = &blue[0];
    char *clr_mnt = &orange[0];
    char *clr_root = &light_red[0];

    Themes theme = Tokyonight;

    if (argc > 1) {

        if (argv[1][0] >= 'a' && argv[1][0] <= 'z') {

            if (strcmp(argv[1], "tokyonight") == 0) {
                //
            } else if (strcmp(argv[1], "catppuccin") == 0) {
                clr_usr = &peach[0];
                clr_time = &pink[0];
                clr_path = &purple[0];
                clr_mnt = &blue[0];
                clr_root = &orange[0];
                theme = Catppuccin;

            } else if (strcmp(argv[1], "kanagawa") == 0) {
                clr_usr = &red[0];
                clr_time = &teal[0];
                clr_path = &khaki[0];
                clr_mnt = &lime[0];
                clr_root = &purple[0];
                theme = Kanagawa;

            } else if (strcmp(argv[1], "koss") == 0) {
                clr_usr = &dark_orange[0];
                clr_time = &beige[0];
                clr_path = &white[0];
                theme = Koss;
            }

            if (argc > 2 && (atoi(argv[2]) > 1)) {
                printf("\\n");
            }

        } else {

            if (atoi(argv[1]) > 1) {
                printf("\\n");
            }

            if (argc > 2) {

                if (strcmp(argv[2], "catppuccin") == 0) {
                    clr_usr = &peach[0];
                    clr_time = &pink[0];
                    clr_path = &purple[0];
                    clr_mnt = &blue[0];
                    clr_root = &orange[0];
                    theme = Catppuccin;

                } else if (strcmp(argv[2], "kanagawa") == 0) {
                    clr_usr = &red[0];
                    clr_time = &teal[0];
                    clr_path = &khaki[0];
                    clr_mnt = &lime[0];
                    clr_root = &purple[0];
                    theme = Kanagawa;

                } else if (strcmp(argv[2], "koss") == 0) {
                    clr_usr = &dark_orange[0];
                    clr_time = &beige[0];
                    clr_path = &white[0];
                    theme = Koss;
                }
            }
        }
    }

    int Plen = strlen(path);
    char *pHome = getenv("HOME");
    bool pHomeState = true;

    // Check that getenv() didn't return NULL
    if (pHome == NULL) {

        pHomeState = false;

    } else {

        int Hlen = strlen(pHome);

        // If path contains $HOME replace it with '~'
        if (strstr(path, pHome) != NULL) {

            replace_home(path, pHome, Plen, Hlen);
            Plen = (Plen - Hlen) + 1;
        }
    }

    if (Plen > ABV_PATH_LEN_T) {

        abrv_path(path, Plen);
        Plen = ABV_PATH_LEN_T;
    }

    printf("%s%s\\u@\\h%s:%s [\\t] ", bold, clr_usr, reset, clr_time);

    // If getenv() returned NULL, just print standard prompt
    if (pHomeState == false) {

        rem_curDir(path, Plen);
        printf("%s%s%s\\W/\\n", clr_path, path, bold);

    } else {

        if (path[0] == '~') {

            // Removing current directoy from path to change
            // text to bold before adding it back with \\W.
            rem_curDir(path, Plen);

            if (Plen > 1) {
                printf("%s%s%s\\W/\\n", clr_path, path, bold);

            } else {
                printf("%s%s\\W/\\n", clr_path, bold);
            }

        } else {

            bool inMnt;
            if (strstr(path, "/mnt") != NULL) {

                inMnt = true;

            } else {

                inMnt = false;
            }

            rem_curDir(path, Plen);

            if (inMnt) {
                printf("%s%s%s\\W/\\n", clr_mnt, path, bold);

            } else if (Plen > 1) {
                printf("%s%s%s\\W/\\n", clr_root, path, bold);

            } else {
                printf("%s%s\\W\\n", clr_root, bold);
            }
        }
    }

    // Check if git is available and current directory is a repo.
    // If yes, get current branch name for prompt.
    // Then determine if there are any files that are untracked,
    // unstaged, staged, # of commits and the # of
    // commits ready to pull.
    bool inRepo = false;
    if (git_is_accessible() && in_repo()) {

        inRepo = true;
        char branch_name[MAX_BRANCH_LEN];
        get_branch(&branch_name[0]);

        int untracked, unstaged, staged, committed, fetched;
        untracked = unstaged = staged = committed = fetched = 0;

        // Currently Unstaged() causes a noticable slow down on larger repos.
        // Going to disable for now, and see if I can fix.
        //unstaged = Unstaged();
        untracked = Untracked();
        staged = Staged();
        committed = Committed();
        fetched = Fetched();

        printf("  %s%s└┬ %s%s%s  %s[", reset, clr_usr, clr_time, branch_name,  clr_path, clr_usr);

        char *clr_ok = &reset[0];
        switch (theme) {
            case Tokyonight:
                clr_ok = &lime[0];
                break;
            case Catppuccin:
                clr_ok = &lime[0];
                break;
            case Kanagawa:
                clr_ok = &green[0];
                break;
            case Koss:
                clr_ok = &green[0];
                break;
        }

        ///////////// Uncomment to debug status bar. //////////////
        //untracked = fetched = unstaged = staged = committed = 5;
        ///////////////////////////////////////////////////////////

        if (untracked > 0 || fetched > 0 || unstaged > 0 || staged > 0 || committed > 0) {

            // Pointers to assign colors for each theme.
            char *clr_untracked = &reset[0];
            char *clr_fetched = &reset[0];
            char *clr_unstaged = &reset[0];
            char *clr_staged = &reset[0];
            char *clr_committed = &reset[0];

            switch (theme) {

                case Tokyonight:
                    clr_untracked = &yellow[0];
                    clr_fetched = &red[0];
                    clr_unstaged = &light_red[0];
                    clr_staged = &dark_purp[0];
                    clr_committed = &lime[0];
                    break;

                case Catppuccin:
                    clr_untracked = &yellow[0];
                    clr_fetched = &red[0];
                    clr_unstaged = &orange[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;

                case Kanagawa:
                    clr_untracked = &yellow[0];
                    clr_fetched = &red[0];
                    clr_unstaged = &purple[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;

                case Koss:
                    clr_untracked = &yellow[0];
                    clr_fetched = &red[0];
                    clr_unstaged = &orange[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;
            }

            // ct tracks how many items will go into
            // the status bar.
            int ct = 0;
            if (untracked > 0)  ct++;
            if (fetched > 0)  ct++;
            if (unstaged > 0)  ct++;
            if (staged > 0)  ct++;
            if (committed > 0)  ct++;

            if (untracked > 0) {
                printf(" %s %s%d ", clr_untracked, reset, untracked);

                // We stop printing at ct = 1 because we
                // want the last one to be ']'.
                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }

            if (fetched > 0) {
                printf(" %s %s%d ", clr_fetched, reset, fetched);

                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }

            if (unstaged > 0) {
                printf(" %s %s%d ", clr_unstaged, reset, unstaged);

                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }

            if (staged > 0) {
                printf(" %s󱝣 %s%d ", clr_staged, reset, staged);

                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }

            if (committed > 0) {
                printf(" %s %s%d ", clr_committed, reset, committed);

                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }
        } else {
            printf("%s󰄬", clr_ok);
        }

        printf("%s]\\n", clr_usr);
    }

    if (inRepo) {
        printf(" ");
    }
    printf("  %s%s└ >$ %s", bold, clr_usr, reset);
    // ─
    return 0;
}
