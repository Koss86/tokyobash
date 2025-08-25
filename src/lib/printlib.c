#include "../../include/tokyobash.h"

void printUsrTime(ConfigSettings* usrConfig, Colors* colorDefs) {

    if (usrConfig->background) {
        printf("%s%s%s", colorDefs->usr_color, usrConfig->sep_left, colorDefs->usr_txt);
        printf("%s\\u@\\h%s%s", colorDefs->usr_bg, colorDefs->usr_color, colorDefs->reset);
    } else {
        printf("%s%s\\u@\\h%s:", colorDefs->bold, colorDefs->usr_txt, colorDefs->reset);
    }

    if (usrConfig->time) {
        if (usrConfig->background) {
            printf("%s%s%s%s ", colorDefs->usr_color, colorDefs->time_bg, usrConfig->sep_right,
                   usrConfig->left_line_sep);
            printf("%s%s\\%c %s", colorDefs->time_txt, colorDefs->time_bg, usrConfig->timeformat[0],
                   colorDefs->reset);
        } else {
            printf("%s [\\%c]", colorDefs->time_txt, usrConfig->timeformat[0]);
        }
    }
    return;
}

void printBranch(ConfigSettings* usrConfig, Colors* colorDefs) {

    char branch_name[MAX_BRANCH_LEN];
    getBranch(&branch_name[0]);

    if (usrConfig->background) {

        if (usrConfig->time) {
            printf("%s%s%s ", colorDefs->branch_bg, colorDefs->time_color, usrConfig->sep_right);
            printf("%s%s%s  %s", colorDefs->branch_txt, colorDefs->branch_bg, branch_name,
                   colorDefs->reset);
        } else {
            printf("%s%s%s%s ", colorDefs->branch_bg, colorDefs->usr_color, usrConfig->sep_right,
                   usrConfig->left_line_sep);
            printf("%s%s%s  %s", colorDefs->branch_txt, colorDefs->branch_bg, branch_name,
                   colorDefs->reset);
        }

    } else {
        printf(" %s%s %s", colorDefs->branch_txt, branch_name, colorDefs->usr_color);
    }
    return;
}

void printPathWithBg(ConfigSettings* usrConfig, Colors* colorDefs, char* path, int pathLength) {

    if (usrConfig->inARepo && usrConfig->branchname) {

        printf("%s%s%s%s", colorDefs->path_txt, colorDefs->branch_bg, usrConfig->right_line_sep,
               colorDefs->reset);

    } else {

        if (usrConfig->time) {

            printf("%s%s%s", colorDefs->time_color, colorDefs->path_bg, usrConfig->sep_right);
            printf("%s%s", colorDefs->path_color, colorDefs->reset);
        } else {
            printf("%s%s", colorDefs->usr_color, colorDefs->path_bg);
            printf("%s%s%s", usrConfig->sep_right, usrConfig->left_line_sep, colorDefs->reset);
        }
    }

    if (pathLength > 1) {

        printf("%s%s %s", colorDefs->path_txt, colorDefs->path_bg, path);
        printf("%s%s\\W/ %s%s%s%s\\n", colorDefs->curDir_txt, colorDefs->bold, colorDefs->reset,
               colorDefs->path_color, usrConfig->sep_right, colorDefs->reset);

    } else {

        if (usrConfig->pathState == Root) {

            printf("%s%s %s%s\\W %s%s", colorDefs->path_txt, colorDefs->path_bg,
                   colorDefs->curDir_txt, colorDefs->bold, colorDefs->reset, colorDefs->path_color);
            printf("%s%s\\n", usrConfig->sep_right, colorDefs->reset);

        } else {

            printf("%s%s %s%s\\W/ %s%s", colorDefs->path_txt, colorDefs->path_bg,
                   colorDefs->curDir_txt, colorDefs->bold, colorDefs->reset, colorDefs->path_color);
            printf("%s%s\\n", usrConfig->sep_right, colorDefs->reset);
        }
    }
    return;
}

void printPathNoBg(ConfigSettings* usrConfig, Colors* colorDefs, char* path, int pathLength) {

    if (pathLength > 1) {

        printf(" %s%s%s%s\\W/\\n", colorDefs->path_txt, path, colorDefs->curDir_txt,
               colorDefs->bold);

    } else {

        if (usrConfig->pathState == Root) {

            printf(" %s%s\\W\\n", colorDefs->curDir_txt, colorDefs->bold);

        } else {

            printf(" %s%s\\W/\\n", colorDefs->curDir_txt, colorDefs->bold);
        }
    }
    return;
}

void printStatusBar(Colors* colorDefs, int untracked, int unstaged, int staged, int committed,
                    int fetched) {

    printf("  %s┗┳[%s", colorDefs->usr_color, colorDefs->reset);

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
        printf(" ");
        space++;
    }

    if (untracked > 0) {
        printf("%s%s %d", colorDefs->untracked, colorDefs->reset, untracked);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colorDefs->usr_color);
            ct--;
        }
    }
    if (unstaged > 0) {
        printf("%s%s %d", colorDefs->unstaged, colorDefs->reset, unstaged);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colorDefs->usr_color);
            ct--;
        }
    }
    if (staged > 0) {
        printf("%s󱝣%s %d", colorDefs->staged, colorDefs->reset, staged);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colorDefs->usr_color);
            ct--;
        }
    }
    if (committed > 0) {
        printf("%s%s %d", colorDefs->committed, colorDefs->reset, committed);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colorDefs->usr_color);
            ct--;
        }
    }
    if (fetched > 0) {
        printf("%s%s %d", colorDefs->fetched, colorDefs->reset, fetched);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colorDefs->usr_color);
            ct--;
        }
    }
    if (space) {
        printf(" ");
    }
    printf("%s%s]\\n ", colorDefs->bold, colorDefs->usr_color);
    return;
}
