#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50 // 24 + 23 + "..."

void replace_home(char *path, char *home, int Plen, int Hlen);
void abrv_path(char *path, int Plen);
void rem_curDir(char *path, int Plen);
bool git_is_accessible();
bool in_repo();
bool get_branch(char *branch_name);
int Untracked();
int Unstaged();
int Staged();
int Committed();
int Fetched(int seconds);

int main(int argc, char **argv) {


    char path[PATH_MAX];
    // Check to make sure we got the path
    if (getcwd(path, sizeof(path)) == NULL) {
        perror("Error: getcwd() failed to retrieve path\\n");
        exit(1);
    }

    char reset[] = "\\[\\e[00m\\]";
    char bold[] = "\\[\\e[1m\\]";
    char yellow[] = "\\[\\e[38;2;255;255;0m\\]";
    char green[] = "\\[\\e[38;2;0;255;0m\\]";

    char cyan[] = "\\[\\e[38;5;86m\\]";
    char sky_blue[] = "\\[\\e[38;5;117m\\]";
    char blue[] = "\\[\\e[38;5;4m\\]";
    char peach[] = "\\[\\e[38;5;223m\\]";
    char light_red[] = "\\[\\e[38;5;211m\\]";

    char pink[] = "\\[\\e[38;5;217m\\]";
    char purple[] = "\\[\\e[38;5;182m\\]";
    char orange[] = "\\[\\e[38;5;214m\\]";

    char red[] = "\\[\\e[38;2;255;77;77m\\]";
    char teal[] = "\\[\\e[38;2;104;155;196m\\]";
    char khaki[] = "\\[\\e[38;2;238;232;170m\\]";
    char lime[] = "\\[\\e[38;2;117;156;38m\\]";

    char dark_orange[] = "\\[\\e[38;2;255;149;20m\\]";
    char beige[] = "\\[\\e[38;2;239;239;200m\\]";
    char white[] = "\\[\\e[38;2;255;255;255m\\]";

    // Assign tokyonight colors as default.
    char *clr_usr = &cyan[0];
    char *clr_time = &sky_blue[0];
    char *clr_path = &blue[0];
    char *clr_mnt = &orange[0];
    char *clr_root = &light_red[0];

    typedef enum Themes {
        Tokyonight, Catppuccin,
        Kanagawa, Koss,
    }Themes;
    Themes theme = Tokyonight;

    int seconds = 0;

    if (argc > 1) {

        if (argv[1][0] >= 'a' && argv[1][0] <= 'z') {

            if (strcmp(argv[1], "catppuccin") == 0) {
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

            if (argc > 2 && ((seconds = atoi(argv[2])) > 1)) {
                printf("\\n");
            }

        } else {

            if ((seconds = atoi(argv[1])) > 1) {
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
    // unstaged, staged, # of commits and (if known) the # of
    // commits ready to pull.
    bool inRepo = false;
    if (git_is_accessible() && in_repo()) {

        inRepo = true;
        char branch_name[MAX_BRANCH_LEN];
        get_branch(&branch_name[0]);

        int untracked, unstaged, staged, committed, fetched;
        untracked = unstaged = staged = committed = fetched = 0;

        untracked = Untracked();
        unstaged = Unstaged();
        staged = Staged();
        committed = Committed();
        fetched = Fetched(seconds);

        printf("  %s%s└┬ %s%s%s  %s[", reset, clr_usr, clr_time, branch_name,  clr_path, clr_usr);

        ///////////// Uncomment to debug status bar. //////////////
        // untracked = fetched = unstaged = staged = committed = 5;
        ///////////////////////////////////////////////////////////

        if (untracked > 0 || fetched > 0 || unstaged > 0 ||
            staged > 0 || committed > 0) {

            char *clr_untracked = &yellow[0];
            char *clr_fetched = &red[0];
            char *clr_unstaged = NULL;
            char *clr_staged = NULL;
            char *clr_committed = NULL;

            switch (theme) {

                case Tokyonight:
                    //clr_untracked = &yellow[0];
                    //clr_fetched = &red[0];
                    clr_unstaged = &orange[0];
                    clr_staged = &pink[0];
                    clr_committed = &green[0];
                    break;

                case Catppuccin:
                    //clr_untracked = &yellow[0];
                    //clr_fetched = &red[0];
                    clr_unstaged = &orange[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;

                case Kanagawa:
                    //clr_untracked = &yellow[0];
                    //clr_fetched = &red[0];
                    clr_unstaged = &purple[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;

                case Koss:
                    //clr_untracked = &yellow[0];
                    //clr_fetched = &red[0];
                    clr_unstaged = &orange[0];
                    clr_staged = &blue[0];
                    clr_committed = &green[0];
                    break;
            }


            int ct = 0;
            if (untracked > 0)  ct++;
            if (fetched > 0)  ct++;
            if (unstaged > 0)  ct++;
            if (staged > 0)  ct++;
            if (committed > 0)  ct++;

            if (untracked > 0) {
                printf(" %s %s%d ", clr_untracked, reset, untracked);

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
                printf(" %s %s%d ", clr_unstaged, reset, unstaged);

                if (ct > 0 && ct != 1)  {
                    printf("%s|", clr_usr);
                    ct--;
                }
            }

            if (staged > 0) {
                printf(" %s󰄳 %s%d ", clr_staged, reset, staged);

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
            printf(" %s ", green);
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

int Untracked() {

    FILE *file = popen("git ls-files --others --exclude-standard | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    int untracked = atoi(buf);
    return untracked;
}

int Fetched(int seconds) {

    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        printf("Error with FETCH_HEAD");
        return -1;
    }
    char fbuf[512];
    if (fgets(fbuf, sizeof(fbuf), fetch_status) == NULL) {
        pclose(fetch_status);
        printf("Error with fgets");
        return -1;
    }

    char c;
    int indx = 0;
    int nl = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
    char date[11];
    char time[9];

    for (int i = 0; i < 512; i++) {

        c = fbuf[i];

        if (c == EOF) break;

        if (c == '\n') {
            nl++;
            continue;
        }

        if (nl == 6 && c == ' ') {
            space++;
            if (space == 1) {
                inDate = 1;
            } else if (space == 2) {
                inTime = 1;
                inDate = 0;
            }
            continue;
        }

        if (inDate) {
            date[indx++] = c;
            if (indx == 10) {
                date[indx] = '\0';
                indx = 0;
            }
        }

        if (inTime) {
            time[indx++] = c;
            if (indx == 8) {
                time[indx] = '\0';
                break;
            }
        }
    }

    //popen("git fetch 2>/dev/null", "r");

    FILE *file = popen("git rev-list --count ..@{u} 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    int fetched = atoi(buf);
    return fetched;
}

int Committed() {

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
    int committed = atoi(buf);
    return committed;
}

int Staged() {

    FILE *file = popen("git diff --cached --name-only | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    int staged = atoi(buf);
    return staged;
}

int Unstaged() {

    FILE *file = popen("git diff --name-only | wc -l 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    int unstaged = atoi(buf);
    return unstaged;
}

void replace_home(char *path, char *home, int Plen, int Hlen) {

    path[0] = '~';

    if (Plen == Hlen) {
        path[1] = '\0';

    } else {
        int indx = Hlen;

        for (int i = 1; i < Plen; i++) {
            path[i] = path[indx++];
        }
        path[indx] = '\0';
    }
}

void abrv_path(char *path, int Plen) {

    int i;
    for (i = ABV_PATH_LEN1; i < ABV_PATH_LEN1 + 3; i++) {
        path[i] = '.';
    }

    int indx = Plen - ABV_PATH_LEN2;

    for (i = ABV_PATH_LEN1 + 3; i < ABV_PATH_LEN_T; i++) {
        path[i] = path[indx++];
    }
    path[ABV_PATH_LEN_T] = '\0';
}

void rem_curDir(char *path, int Plen) {

    for (int i = Plen - 1; i > -1; i--) {

        if (path[i] == '/') {
            path[i + 1] = '\0';
            break;
        }
    }
}

bool git_is_accessible() {

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

bool in_repo() {

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

bool get_branch(char *branch_name) {

    FILE *file = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (file == NULL) {

        return false;
    }
    if (fgets(branch_name, MAX_BRANCH_LEN, file) == NULL) {

        pclose(file);
        return false;
    }
    int len = strlen(branch_name);
    if (len > 0 && branch_name[len - 1] == '\n') {

        branch_name[len - 1] = '\0';
    }
    pclose(file);
    return true;
}
