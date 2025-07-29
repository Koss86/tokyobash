#include "../../include/tokyobash.h"

void assignPointers(ColorPointers* pointers, Colors* colors, ConfigSettings* usrConfig) {

    pointers->bold = &colors->bold[0];
    pointers->reset = &colors->reset[0];
    pointers->txt_fg_color = &colors->black[0];
    pointers->untracked_color = &colors->yellow[0];

    if (usrConfig->background) {
        switch (usrConfig->theme) {
            case Tokyonight:
                pointers->usr_color = &colors->cyan[0];
                pointers->time_color = &colors->teal[0];
                pointers->branch_color = &colors->sky_blue[0];
                pointers->path_color = &colors->blue[0];
                pointers->mnt_color = &colors->orange[0];
                pointers->root_color = &colors->pink[0];

                pointers->unstaged_color = &colors->orange[0];
                pointers->staged_color = &colors->orchid[0];
                pointers->committed_color = &colors->lime_green[0];
                pointers->fetched_color = &colors->dark_orange[0];

                pointers->usr_bg = &colors->cyan_bg[0];
                pointers->time_bg = &colors->teal_bg[0];
                pointers->branch_bg = &colors->sky_blue_bg[0];
                pointers->path_bg = &colors->blue_bg[0];
                pointers->root_bg = &colors->pink_bg[0];
                pointers->mnt_bg = &colors->orange_bg[0];
                break;
            case Catppuccin:
                pointers->usr_color = &colors->peach[0];
                pointers->time_color = &colors->rose[0];
                pointers->branch_color = &colors->sky_blue[0];
                pointers->path_color = &colors->light_purple[0];
                pointers->mnt_color = &colors->blue[0];
                pointers->root_color = &colors->orange[0];

                pointers->untracked_color = &colors->yellow[0];
                pointers->unstaged_color = &colors->orange[0];
                pointers->staged_color = &colors->blue[0];
                pointers->committed_color = &colors->lime_green[0];
                pointers->fetched_color = &colors->pink[0];

                pointers->usr_bg = &colors->cyan_bg[0];
                pointers->time_bg = &colors->teal_bg[0];
                pointers->branch_bg = &colors->sky_blue_bg[0];
                pointers->path_bg = &colors->blue_bg[0];
                break;
            case Kanagawa:
                break;
            case Orange:
                break;
        }

    } else {

        switch (usrConfig->theme) {

            case Tokyonight:
                pointers->usr_color = &colors->cyan[0];
                pointers->time_color = &colors->sky_blue[0];
                pointers->path_color = &colors->blue[0];
                pointers->mnt_color = &colors->orange[0];
                pointers->root_color = &colors->pink[0];

                pointers->unstaged_color = &colors->orange[0];
                pointers->staged_color = &colors->orchid[0];
                pointers->committed_color = &colors->lime_green[0];
                pointers->fetched_color = &colors->dark_orange[0];
                break;

            case Catppuccin:
                pointers->usr_color = &colors->peach[0];
                pointers->time_color = &colors->rose[0];
                pointers->path_color = &colors->light_purple[0];
                pointers->mnt_color = &colors->blue[0];
                pointers->root_color = &colors->orange[0];

                pointers->unstaged_color = &colors->orange[0];
                pointers->staged_color = &colors->blue[0];
                pointers->committed_color = &colors->lime_green[0];
                pointers->fetched_color = &colors->pink[0];
                break;

            case Kanagawa:
                pointers->usr_color = &colors->peach[0];
                pointers->time_color = &colors->rose[0];
                pointers->path_color = &colors->light_purple[0];
                pointers->mnt_color = &colors->blue[0];
                pointers->root_color = &colors->orange[0];

                pointers->unstaged_color = &colors->orange[0];
                pointers->staged_color = &colors->blue[0];
                pointers->committed_color = &colors->lime_green[0];
                pointers->fetched_color = &colors->pink[0];
                break;

            case Orange:
                pointers->usr_color = &colors->dark_orange[0];
                pointers->time_color = &colors->beige[0];
                pointers->path_color = &colors->white[0];
                pointers->mnt_color = &colors->orange[0];
                pointers->root_color = &colors->pink[0];

                pointers->unstaged_color = &colors->red[0];
                pointers->staged_color = &colors->blue[0];
                pointers->committed_color = &colors->green[0];
                pointers->fetched_color = &colors->red[0];
                break;
        }
    }
}

void populateColors(Colors* colors) {
    strncpy(colors->bold, "\\[\\e[1m\\]", 15);
    strncpy(colors->reset, "\\[\\e[00m\\]", 15);
    strncpy(colors->red, "\\[\\e[38;2;255;77;77m\\]", 30);
    strncpy(colors->desat_firebrick, "\\[\\e[38;2;191;72;72m\\]", 30);
    strncpy(colors->pink, "\\[\\e[38;5;204m\\]", 30);
    strncpy(colors->rose, "\\[\\e[38;5;217m\\]", 30);
    strncpy(colors->peach, "\\[\\e[38;5;223m\\]", 30);
    strncpy(colors->dark_orange, "\\[\\e[38;2;255;149;20m\\]", 30);
    strncpy(colors->orange, "\\[\\e[38;5;214m\\]", 30);
    strncpy(colors->yellow, "\\[\\e[38;2;255;255;0m\\]", 30);
    strncpy(colors->green, "\\[\\e[38;2;0;255;0m\\]", 30);
    strncpy(colors->lime_green, "\\[\\e[38;2;172;255;47m\\]", 30);
    strncpy(colors->desat_lime, "\\[\\e[38;2;117;156;38m\\]", 30);
    strncpy(colors->cyan, "\\[\\e[38;5;86m\\]", 30);
    strncpy(colors->blue, "\\[\\e[38;5;4m\\]", 30);
    strncpy(colors->sky_blue, "\\[\\e[38;5;117m\\]", 30);
    strncpy(colors->teal, "\\[\\e[38;5;37m\\]", 30);
    strncpy(colors->orchid, "\\[\\e[38;2;166;121;210m\\]", 30);
    strncpy(colors->light_purple, "\\[\\e[38;5;182m\\]", 30);
    strncpy(colors->purple, "\\[\\e[38;2;153;102;204m\\]", 30);
    strncpy(colors->khaki, "\\[\\e[38;2;238;232;170m\\]", 30);
    strncpy(colors->white, "\\[\\e[38;2;255;255;255m\\]", 30);
    strncpy(colors->black, "\\[\\e[0;30m\\]", 30);
    strncpy(colors->beige, "\\[\\e[38;2;239;239;200m\\]", 30);

    strncpy(colors->blue_bg, "\\[\\e[48;5;4m\\]", 30);
    strncpy(colors->sky_blue_bg, "\\[\\e[48;5;117m\\]", 30);
    strncpy(colors->cyan_bg, "\\[\\e[48;5;86m\\]", 30);
    strncpy(colors->teal_bg, "\\[\\e[48;5;37m\\]", 30);
    strncpy(colors->pink_bg, "\\[\\e[48;5;204m\\]", 30);
    strncpy(colors->orange_bg, "\\[\\e[48;5;214m\\]", 30);
}

void printPathWithBg(ConfigSettings* usrConfig, ColorPointers* pointers, PathState pathState,
                     char* path, int pathLength) {

    if (usrConfig->time) {

        if (usrConfig->git && usrConfig->branchname) {

            if (usrConfig->inARepo) {
                switch (pathState) {
                    case Home:
                        printf("%s%s%s", pointers->path_color, pointers->branch_bg,
                               pointers->reset);
                        break;
                    case Mnt:
                        printf("%s%s%s", pointers->mnt_color, pointers->mnt_bg, pointers->reset);
                        break;
                    case Root:
                        printf("%s%s%s", pointers->root_color, pointers->root_bg,
                               pointers->reset);
                        break;
                }
            } else { /// Outside of Repo ///
                switch (pathState) {
                    case Home:
                        printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                               pointers->path_color, pointers->reset);
                        break;
                    case Mnt:
                        printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                               pointers->mnt_color, pointers->reset);
                        break;
                    case Root:
                        printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                               pointers->root_color, pointers->reset);
                        break;
                }
            }

        } else { /// Git or Branch Name Disabled ///

            switch (pathState) {
                case Home:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->path_color, pointers->reset);
                    break;
                case Mnt:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->mnt_color, pointers->reset);
                    break;
                case Root:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->root_color, pointers->reset);
                    break;
            }
        }
        switch (pathState) {
            case Home:
                if (pathLength > 1) {
                    printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->path_bg,
                           path, pointers->bold, pointers->reset, pointers->path_color,
                           pointers->reset);
                } else {
                    printf("%s%s%s\\W/%s%s\\n", pointers->txt_fg_color, pointers->path_bg,
                           pointers->bold, pointers->reset, pointers->path_color);
                }
                break;
            case Mnt:
                printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->mnt_bg, path,
                       pointers->bold, pointers->reset, pointers->mnt_color, pointers->reset);
                break;
            case Root:
                if (pathLength > 1) {
                    printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->root_bg,
                           path, pointers->bold, pointers->reset, pointers->root_color,
                           pointers->reset);
                } else {
                    printf("%s%s%s \\W%s%s\\n", pointers->txt_fg_color, pointers->root_bg,
                           pointers->bold, pointers->root_color, pointers->reset);
                }
                break;
        }

    } else { /// Time Disabled ///

        if (usrConfig->git && usrConfig->branchname) {

            if (usrConfig->inARepo) {
                switch (pathState) {
                    case Home:
                        printf("%s%s%s", pointers->path_color, pointers->branch_bg,
                               pointers->reset);
                        break;
                    case Mnt:
                        printf("%s%s%s", pointers->mnt_color, pointers->mnt_bg, pointers->reset);
                        break;
                    case Root:
                        printf("%s%s%s", pointers->root_color, pointers->root_bg,
                               pointers->reset);
                        break;
                }

            } else {

                switch (pathState) {
                    case Home:
                        printf("%s%s%s%s", pointers->usr_color, pointers->branch_bg,
                               pointers->path_color, pointers->reset);
                        break;
                    case Mnt:
                        printf("%s%s%s%s", pointers->usr_color, pointers->branch_bg,
                               pointers->mnt_color, pointers->reset);
                        break;
                    case Root:
                        printf("%s%s%s%s", pointers->usr_color, pointers->branch_bg,
                               pointers->root_color, pointers->reset);
                        break;
                }
            }

        } else {

            switch (pathState) {
                case Home:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->path_color, pointers->reset);
                    break;
                case Mnt:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->mnt_color, pointers->reset);
                    break;
                case Root:
                    printf("%s%s%s%s", pointers->time_color, pointers->branch_bg,
                           pointers->root_color, pointers->reset);
                    break;
            }
        }
        switch (pathState) {
            case Home:
                if (pathLength > 1) {
                    printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->path_bg,
                           path, pointers->bold, pointers->reset, pointers->path_color,
                           pointers->reset);
                } else {
                    printf("%s%s%s\\W/%s%s\\n", pointers->txt_fg_color, pointers->path_bg,
                           pointers->bold, pointers->reset, pointers->path_color);
                }
                break;
            case Mnt:
                printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->mnt_bg, path,
                       pointers->bold, pointers->reset, pointers->mnt_color, pointers->reset);
                break;
            case Root:
                if (pathLength > 1) {
                    printf("%s%s%s%s\\W/%s%s%s\\n", pointers->txt_fg_color, pointers->root_bg,
                           path, pointers->bold, pointers->reset, pointers->root_color,
                           pointers->reset);
                } else {
                    printf("%s%s%s \\W%s%s\\n", pointers->txt_fg_color, pointers->root_bg,
                           pointers->bold, pointers->root_color, pointers->reset);
                }
                break;
        }
    }
}

void printPathNoBg(ColorPointers* pointers, PathState pathState, char* path, int pathLength) {

    switch (pathState) {

        case Home:
            if (pathLength > 1) {
                printf(" %s%s%s\\W/\\n", pointers->path_color, path, pointers->bold);
            } else {
                printf(" %s%s\\W/\\n", pointers->path_color, pointers->bold);
            }
            break;

        case Mnt:
            printf(" %s%s%s\\W/\\n", pointers->mnt_color, path, pointers->bold);
            break;

        case Root:
            if (pathLength > 1) {
                printf(" %s%s%s\\W/\\n", pointers->root_color, path, pointers->bold);
            } else {
                printf(" %s%s\\W\\n", pointers->root_color, pointers->bold);
            }
            break;
    }
}

void parseConfig(ConfigSettings* usrConfig, char* pHome, int homeLength) {

    char path[PATH_MAX];
    char* pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[256] = "/tokyobash/config";
    int filePathLen = 17;

    if (pConfig == NULL || pConfig[0] == '\0') {
        filePathLen = 25;
        strncpy(filepath, "/.config/tokyobash/config", filePathLen + 1);

        for (int i = 0; i < homeLength + filePathLen; i++) {
            if (i < homeLength) {
                path[i] = pHome[i];
                continue;
            }
            path[i] = filepath[i - homeLength];
        }
        path[homeLength + filePathLen] = '\0';

    } else {

        int configPathLen = strlen(pConfig);
        // This check may not be needed but it's here just in case.
        if (pConfig[configPathLen - 1] == '/') {
            pConfig[configPathLen - 1] = '\0';
            configPathLen--;
        }

        for (int i = 0; i < configPathLen + filePathLen; i++) {
            if (i < configPathLen) {
                path[i] = pConfig[i];
                continue;
            }
            path[i] = filepath[i - configPathLen];
        }
        path[configPathLen + filePathLen] = '\0';
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    char c;
    bool inkey = true;
    bool invalue = false;
    bool incomment = false;
    int slash = 0;
    int indx = 0;
    char keybuf[64];
    char valbuf[64];

    while ((c = fgetc(file)) != EOF) {

        if (c == ' ' || c == '/' || c == '\'' || c == '"') {
            if (c == '/') {
                slash++;
            }
            if (slash == 2) {
                incomment = true;
                slash = 0;
            }
            continue;
        }
        if (c == '#') {
            incomment = true;
            continue;
        }

        if (c == '\n') {
            if (incomment) {
                inkey = true;
                incomment = false;
                invalue = false;
                indx = 0;
                continue;
            }
            inkey = true;
            invalue = false;
            valbuf[indx] = '\0';
            indx = 0;
            if (valbuf[0] >= 'a' && valbuf[0] <= 'z') {

                if ((strncmp(keybuf, "theme", 5)) == 0) {

                    if ((strncmp(valbuf, "tokyonight", 10)) == 0) {
                        usrConfig->theme = Tokyonight;

                    } else if ((strncmp(valbuf, "catppuccin", 10)) == 0) {
                        usrConfig->theme = Catppuccin;

                    } else if ((strncmp(valbuf, "kanagawa", 8)) == 0) {
                        usrConfig->theme = Kanagawa;

                    } else if ((strncmp(valbuf, "orange", 6)) == 0) {
                        usrConfig->theme = Orange;
                    }
                } // add else if's here for future options with char vals.

            } else {

                if ((strncmp(keybuf, "statusbar", 9)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->statusbar = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->statusbar = true;
                    }

                } else if ((strncmp(keybuf, "git", 3)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->git = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->git = true;
                    }

                } else if ((strncmp(keybuf, "branchname", 10)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->branchname = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->branchname = true;
                    }

                } else if ((strncmp(keybuf, "fetchtimer", 10)) == 0) {

                    if (valbuf[1] == 'm' || valbuf[2] == 'm') {

                        usrConfig->fetchConfig.modifier = Minute;

                    } else if (valbuf[1] == 'h' || valbuf[2] == 'h') {

                        usrConfig->fetchConfig.modifier = Hour;

                    } else if (valbuf[1] == 'd' || valbuf[2] == 'd') {

                        usrConfig->fetchConfig.modifier = Day;
                    }

                    usrConfig->fetchConfig.limit = atoi(&valbuf[0]);

                    switch (usrConfig->fetchConfig.modifier) {

                        case Minute:
                            if (usrConfig->fetchConfig.limit > 60) {
                                usrConfig->fetchConfig.limit = 60;
                            }
                            break;
                        case Hour:
                            if (usrConfig->fetchConfig.limit > 24) {
                                usrConfig->fetchConfig.limit = 24;
                            }
                            break;
                        case Day:
                            if (usrConfig->fetchConfig.limit > 30) {
                                usrConfig->fetchConfig.limit = 30;
                            }
                            break;
                    }

                } else if ((strncmp(keybuf, "time", 4)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->time = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->time = true;
                    }

                } else if ((strncmp(keybuf, "background", 10)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->background = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->background = true;
                    }

                } // add else if's here for future optoins with int vals.

                else if ((strncmp(keybuf, "debug", 5)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->debug = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->debug = true;
                    }
                }
            }
            continue;
        }

        if (incomment) {
            continue;
        }

        if (c == '=') {
            invalue = true;
            inkey = false;
            keybuf[indx] = '\0';
            indx = 0;
            continue;
        }

        if (inkey) {
            keybuf[indx++] = c;
        }
        if (invalue) {
            valbuf[indx++] = c;
        }
    }
    fclose(file);
    return;
}
// If path contains $HOME, replace it with '~'.
void replaceHome(char* path, int pathLength, int homeLength) {

    path[0] = '~';

    if (pathLength == homeLength) {
        path[1] = '\0';

    } else {
        int indx = homeLength;

        for (int i = 1; i < pathLength; i++) {
            path[i] = path[indx++];
        }
        path[indx] = '\0';
    }
    return;
}
// If path lenth is greater than 50, keep first 24 chars, add '...' then
// place last 23 chars after last '.' .
void abrvPath(char* path, int pathLength) {

    for (int i = ABV_PATH_LEN1; i < ABV_PATH_LEN1 + 3; i++) {
        path[i] = '.';
    }

    int indx = pathLength - ABV_PATH_LEN2;

    for (int i = ABV_PATH_LEN1 + 3; i < ABV_PATH_LEN_T; i++) {
        path[i] = path[indx++];
    }
    path[ABV_PATH_LEN_T] = '\0';
    return;
}
// Remove current directory from path. We add it back with \W after
// changing text to bold. This way the path is normal, while current dir is
// highlighted.
void remCurntDir(char* path, int pathLength) {

    for (int i = pathLength - 1; i > -1; i--) {

        if (path[i] == '/') {
            path[i + 1] = '\0';
            break;
        }
    }
    return;
}
