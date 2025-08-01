#include "../../include/tokyobash.h"

void printUsrTime(ConfigSettings* usrConfig, ColorPointers* colors) {

    if (usrConfig->background) {
        printf("%s%s%s%s\\u@\\h%s%s", colors->bold, colors->usr_color, colors->usr_txt,
               colors->usr_bg, colors->usr_color, colors->reset);
    } else {
        printf("%s%s\\u@\\h%s:", colors->bold, colors->usr_txt, colors->reset);
    }

    if (usrConfig->time) {
        if (usrConfig->background) {
            printf("%s%s %s%s\\t %s", colors->usr_color, colors->time_bg, colors->time_txt,
                   colors->time_bg, colors->reset);
        } else {
            printf("%s [\\t]", colors->time_txt);
        }
    }
    return;
}

void printBranch(ConfigSettings* usrConfig, ColorPointers* colors) {

    char branch_name[MAX_BRANCH_LEN];
    getBranch(&branch_name[0]);

    if (usrConfig->background) {

        if (usrConfig->time) {
            printf("%s%s %s%s%s  %s", colors->branch_bg, colors->time_color,
                   colors->branch_txt, colors->branch_bg, branch_name, colors->reset);
        } else {
            printf("%s%s %s%s%s  %s", colors->branch_bg, colors->usr_color,
                   colors->branch_txt, colors->branch_bg, branch_name, colors->reset);
        }

    } else {
        printf(" %s%s %s", colors->branch_txt, branch_name, colors->usr_color);
    }
    return;
}

void printPathWithBg(ConfigSettings* usrConfig, ColorPointers* colors, PathState pathState,
                     char* path, int pathLength) {

    if (usrConfig->time) {

        if (usrConfig->inARepo) {

            switch (pathState) {

                case Home:
                    printf("%s%s%s", colors->path_txt, colors->branch_bg, colors->reset);
                    break;

                case Mnt:
                    printf("%s%s%s", colors->mnt_txt, colors->mnt_bg, colors->reset);
                    break;

                case Root:
                    printf("%s%s%s", colors->root_txt, colors->root_bg, colors->reset);
                    break;
            }

        } else {

            switch (pathState) {

                case Home:
                    printf("%s%s%s%s", colors->time_color, colors->path_bg, colors->path_color,
                           colors->reset);
                    break;

                case Mnt:
                    printf("%s%s%s%s", colors->time_color, colors->mnt_bg, colors->mnt_color,
                           colors->reset);
                    break;

                case Root:
                    printf("%s%s%s%s", colors->time_color, colors->root_bg, colors->root_color,
                           colors->reset);
                    break;
            }
        }
    } else {

        if (usrConfig->inARepo) {

            switch (pathState) {

                case Home:
                    printf("%s%s%s", colors->branch_txt, colors->branch_bg, colors->reset);
                    break;

                case Mnt:
                    printf("%s%s%s", colors->branch_txt, colors->mnt_bg, colors->reset);
                    break;

                case Root:
                    printf("%s%s%s", colors->branch_txt, colors->root_bg, colors->reset);
                    break;
            }

        } else {

            switch (pathState) {

                case Home:
                    printf("%s%s%s", colors->usr_color, colors->path_bg, colors->reset);
                    break;

                case Mnt:
                    printf("%s%s%s", colors->usr_color, colors->mnt_bg, colors->reset);
                    break;

                case Root:
                    printf("%s%s%s", colors->usr_color, colors->root_bg, colors->reset);
                    break;
            }
        }
    }

    switch (pathState) {

        case Home:
            if (pathLength > 1) {
                printf("%s%s %s%s\\W/ %s%s%s\\n", colors->path_txt, colors->path_bg, path,
                       colors->bold, colors->reset, colors->path_color, colors->reset);
            } else {
                printf("%s%s %s\\W/ %s%s\\n", colors->path_txt, colors->path_bg, colors->bold,
                       colors->reset, colors->path_color);
            }
            break;

        case Mnt:
            printf("%s%s %s%s\\W/ %s%s%s\\n", colors->mnt_txt, colors->mnt_bg, path,
                   colors->bold, colors->reset, colors->mnt_color, colors->reset);
            break;

        case Root:
            if (pathLength > 1) {
                printf("%s%s %s%s\\W/ %s%s%s\\n", colors->root_txt, colors->root_bg, path,
                       colors->bold, colors->reset, colors->root_color, colors->reset);
            } else {
                printf("%s%s %s\\W %s%s%s\\n", colors->root_txt, colors->root_bg, colors->bold,
                       colors->reset, colors->root_color, colors->reset);
            }
            break;
    }
    return;
}

void printPathNoBg(ColorPointers* colors, PathState pathState, char* path, int pathLength) {

    switch (pathState) {

        case Home:
            if (pathLength > 1) {
                printf(" %s%s%s\\W/\\n", colors->path_txt, path, colors->bold);
            } else {
                printf(" %s%s\\W/\\n", colors->path_txt, colors->bold);
            }
            break;

        case Mnt:
            printf(" %s%s%s\\W/\\n", colors->mnt_txt, path, colors->bold);
            break;

        case Root:
            if (pathLength > 1) {
                printf(" %s%s%s\\W/\\n", colors->root_txt, path, colors->bold);
            } else {
                printf(" %s%s\\W\\n", colors->root_txt, colors->bold);
            }
            break;
    }
    return;
}

void printStatusBar(ColorPointers* colors, int untracked, int unstaged, int staged, int committed,
                    int fetched) {

    printf("  %s┗┳[%s", colors->usr_color, colors->reset);

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
        printf("%s%s %d", colors->untracked, colors->reset, untracked);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colors->usr_color);
            ct--;
        }
    }
    if (unstaged > 0) {
        printf("%s%s %d", colors->unstaged, colors->reset, unstaged);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colors->usr_color);
            ct--;
        }
    }
    if (staged > 0) {
        printf("%s󱝣%s %d", colors->staged, colors->reset, staged);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colors->usr_color);
            ct--;
        }
    }
    if (committed > 0) {
        printf("%s%s %d", colors->committed, colors->reset, committed);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colors->usr_color);
            ct--;
        }
    }
    if (fetched > 0) {
        printf("%s%s %d", colors->fetched, colors->reset, fetched);

        if (ct > 0 && ct != 1) {
            printf("%s | ", colors->usr_color);
            ct--;
        }
    }
    if (space) {
        printf(" ");
    }
    printf("%s%s]\\n ", colors->bold, colors->usr_color);
    return;
}
