#include "../../include/tokyobash.h"

void parseConfig(ConfigSettings* usrConfig, char* pHome) {

    char path[PATH_MAX];
    char* pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[256] = "/tokyobash/config";
    path[0] = '\0';

    if (pConfig == NULL || pConfig[0] == '\0') {

        strcpy(filepath, "/.config/tokyobash/config");
        strcat(path, pHome);
        strcat(path, filepath);

    } else {

        int configPathLen = strlen(pConfig);
        // This check may not be needed but it's here just in case.
        if (pConfig[configPathLen - 1] == '/') {
            pConfig[configPathLen - 1] = '\0';
        }
        strcat(path, pConfig);
        strcat(path, filepath);
    }

    FILE* file = fopen(path, "r");
    if (file == NULL)
        return;

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
                    }
                } // add else if's here for future options with char vals.

            } else {

                if ((strncmp(keybuf, "statusbar", 9)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->statusbar = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->statusbar = true;
                    }

                } else if ((strncmp(keybuf, "debug", 5)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->debug = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->debug = true;
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

                } else if ((strncmp(keybuf, "fetch", 5)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->fetch = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->fetch = true;
                    }
                } // add else if's here for future optoins with int vals.
            }
            continue;
        }

        if (incomment)
            continue;

        if (c == '=') {
            invalue = true;
            inkey = false;
            keybuf[indx] = '\0';
            indx = 0;
            continue;
        }

        if (inkey)
            keybuf[indx++] = c;

        if (invalue)
            valbuf[indx++] = c;
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
void assignPointers(ColorPointers* colors, Colors* colorDefs, ConfigSettings* usrConfig) {

    colors->bold = &colorDefs->bold[0];
    colors->reset = &colorDefs->reset[0];
    colors->untracked = &colorDefs->yellow[0];

    if (usrConfig->background) {

        switch (usrConfig->theme) {

            case Tokyonight:
                switch (usrConfig->pathState) {
                    case Home:
                        colors->usr_txt = &colorDefs->teal[0];
                        colors->usr_color = &colorDefs->cyan[0];
                        colors->usr_bg = &colorDefs->cyan_bg[0];
                        break;
                    case Mnt:
                        colors->usr_txt = &colorDefs->beige[0];
                        colors->usr_color = &colorDefs->sandy_orange[0];
                        colors->usr_bg = &colorDefs->sandy_orange_bg[0];
                        break;
                    case Root:
                        colors->usr_txt = &colorDefs->khaki[0];
                        colors->usr_color = &colorDefs->rose[0];
                        colors->usr_bg = &colorDefs->rose_bg[0];
                        break;
                }
                colors->time_txt = &colorDefs->smoke[0];
                colors->time_color = &colorDefs->slate[0];
                colors->time_bg = &colorDefs->slate_bg[0];

                colors->branch_txt = &colorDefs->gray[0];
                colors->branch_color = &colorDefs->light_slate[0];
                colors->branch_bg = &colorDefs->light_slate_bg[0];

                colors->path_txt = &colorDefs->blue[0];
                colors->path_color = colors->branch_color;
                colors->path_bg = colors->branch_bg;

                colors->unstaged = &colorDefs->gold[0];
                colors->staged = &colorDefs->orchid[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->orange[0];
                break;

            case Catppuccin:
                switch (usrConfig->pathState) {
                    case Home:
                        colors->usr_txt = &colorDefs->surface1[0];
                        colors->usr_color = &colorDefs->peach[0];
                        colors->usr_bg = &colorDefs->peach_bg[0];
                        break;
                    case Mnt:
                        colors->usr_txt = &colorDefs->surface1[0];
                        colors->usr_color = &colorDefs->gold[0];
                        colors->usr_bg = &colorDefs->gold_bg[0];
                        break;
                    case Root:
                        colors->usr_txt = &colorDefs->surface1[0];
                        colors->usr_color = &colorDefs->rose[0];
                        colors->usr_bg = &colorDefs->rose_bg[0];
                        break;
                }
                colors->time_txt = &colorDefs->rosewater[0];
                colors->time_color = &colorDefs->surface1[0];
                colors->time_bg = &colorDefs->surface1_bg[0];

                colors->branch_txt = &colorDefs->khaki[0];
                colors->branch_color = &colorDefs->surface2[0];
                colors->branch_bg = &colorDefs->surface2_bg[0];

                colors->path_txt = &colorDefs->orchid[0];
                colors->path_color = colors->branch_color;
                colors->path_bg = colors->branch_bg;

                colors->mnt_txt = &colorDefs->beige[0];
                colors->root_txt = &colorDefs->light_purple[0];

                colors->unstaged = &colorDefs->sandy_orange[0];
                colors->staged = &colorDefs->blue[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->pink[0];
                break;

            case Kanagawa:
                switch (usrConfig->pathState) {
                    case Home:
                        colors->usr_txt = &colorDefs->sumiInk3[0];
                        colors->usr_color = &colorDefs->firebrick[0];
                        colors->usr_bg = &colorDefs->firebrick_bg[0];
                        break;
                    case Mnt:
                        colors->usr_txt = &colorDefs->surface1[0];
                        colors->usr_color = &colorDefs->surimiOrange[0];
                        colors->usr_bg = &colorDefs->surimiOrange_bg[0];
                        break;
                    case Root:
                        colors->usr_txt = &colorDefs->surface1[0];
                        colors->usr_color = &colorDefs->orchid[0];
                        colors->usr_bg = &colorDefs->orchid_bg[0];
                        break;
                }
                colors->time_txt = &colorDefs->waveAqua2[0];
                colors->time_color = &colorDefs->sumiInk3[0];
                colors->time_bg = &colorDefs->sumiInk3_bg[0];

                colors->branch_txt = &colorDefs->surimiOrange[0];
                colors->branch_color = &colorDefs->sumiInk4[0];
                colors->branch_bg = &colorDefs->sumiInk4_bg[0];

                colors->path_txt = &colorDefs->light_gold[0];
                colors->path_color = colors->branch_color;
                colors->path_bg = colors->branch_bg;

                colors->mnt_txt = colors->path_txt;
                colors->root_txt = colors->path_txt;

                colors->unstaged = &colorDefs->surimiOrange[0];
                colors->staged = &colorDefs->blue[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->pink[0];
                break;
        }

    } else {

        switch (usrConfig->theme) {

            case Tokyonight:
                colors->usr_color = &colorDefs->cyan[0];
                colors->usr_txt = &colorDefs->cyan[0];
                colors->time_txt = &colorDefs->sky_blue[0];
                colors->branch_txt = &colorDefs->blue[0];
                colors->path_txt = &colorDefs->blue[0];
                colors->mnt_txt = &colorDefs->gold[0];
                colors->root_txt = &colorDefs->pink[0];

                colors->unstaged = &colorDefs->gold[0];
                colors->staged = &colorDefs->orchid[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->orange[0];
                break;

            case Catppuccin:
                colors->usr_color = &colorDefs->peach[0];
                colors->usr_txt = &colorDefs->peach[0];
                colors->time_txt = &colorDefs->rosewater[0];
                colors->branch_txt = &colorDefs->light_purple[0];
                colors->path_txt = &colorDefs->light_purple[0];
                colors->mnt_txt = &colorDefs->blue[0];
                colors->root_txt = &colorDefs->gold[0];

                colors->unstaged = &colorDefs->gold[0];
                colors->staged = &colorDefs->blue[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->pink[0];
                break;

            case Kanagawa:
                colors->usr_color = &colorDefs->firebrick[0];
                colors->usr_txt = &colorDefs->firebrick[0];
                colors->time_txt = &colorDefs->smoke[0];
                colors->branch_txt = &colorDefs->khaki[0];
                colors->path_txt = &colorDefs->khaki[0];
                colors->mnt_txt = &colorDefs->orange[0];
                colors->root_txt = &colorDefs->light_purple[0];

                colors->unstaged = &colorDefs->gold[0];
                colors->staged = &colorDefs->blue[0];
                colors->committed = &colorDefs->lime_green[0];
                colors->fetched = &colorDefs->pink[0];
                break;
        }
    }
    return;
}
void colorDefinitions(Colors* colorDefs) {
    strncpy(colorDefs->bold, "\\[\\e[1m\\]", 13);
    strncpy(colorDefs->reset, "\\[\\e[00m\\]", 14);
    strncpy(colorDefs->red, "\\[\\e[38;2;255;77;77m\\]", 26);
    strncpy(colorDefs->yellow, "\\[\\e[38;2;255;255;0m\\]", 26);
    strncpy(colorDefs->green, "\\[\\e[38;2;0;255;0m\\]", 24);
    strncpy(colorDefs->lime_green, "\\[\\e[38;2;172;255;47m\\]", 27);
    strncpy(colorDefs->desat_lime, "\\[\\e[38;2;101;135;33m\\]", 27);
    strncpy(colorDefs->white, "\\[\\e[38;2;255;255;255m\\]", 28);
    strncpy(colorDefs->beige, "\\[\\e[38;2;239;239;200m\\]", 28);
    strncpy(colorDefs->black, "\\[\\e[0;30m\\]", 16);
    strncpy(colorDefs->gray, "\\[\\e[38;2;154;165;206m\\]", 28);
    strncpy(colorDefs->pale_teal, "\\[\\e[38;2;148;226;213m\\]", 28);
    strncpy(colorDefs->dark_peach, "\\[\\e[38;2;246;117;37m\\]", 28);
    strncpy(colorDefs->rosewater, "\\[\\e[38;2;245;224;220m\\]", 28);
    strncpy(colorDefs->waveAqua2, "\\[\\e[38;2;122;168;159m\\]", 28);

    strncpy(colorDefs->surimiOrange, "\\[\\e[38;2;255;161;102m\\]", 28);
    strncpy(colorDefs->surimiOrange_bg, "\\[\\e[48;2;255;161;102m\\]", 28);
    strncpy(colorDefs->winter_green, "\\[\\e[38;2;55;65;51m\\]", 25);
    strncpy(colorDefs->winter_green_bg, "\\[\\e[48;2;55;65;51m\\]", 25);
    strncpy(colorDefs->sumiInk3, "\\[\\e[38;2;54;54;70m\\]", 25);
    strncpy(colorDefs->sumiInk3_bg, "\\[\\e[48;2;54;54;70m\\]", 25);
    strncpy(colorDefs->sumiInk4, "\\[\\e[38;2;84;84;109m\\]", 26);
    strncpy(colorDefs->sumiInk4_bg, "\\[\\e[48;2;84;84;109m\\]", 26);
    strncpy(colorDefs->surface1, "\\[\\e[38;2;69;71;90m\\]", 25);
    strncpy(colorDefs->surface1_bg, "\\[\\e[48;2;69;71;90m\\]", 25);
    strncpy(colorDefs->surface2, "\\[\\e[38;2;88;91;112m\\]", 26);
    strncpy(colorDefs->surface2_bg, "\\[\\e[48;2;88;91;112m\\]", 26);
    strncpy(colorDefs->firebrick, "\\[\\e[38;2;255;67;74m\\]", 26);
    strncpy(colorDefs->firebrick_bg, "\\[\\e[48;2;255;67;74m\\]", 26);
    strncpy(colorDefs->pink, "\\[\\e[38;5;204m\\]", 20);
    strncpy(colorDefs->pink_bg, "\\[\\e[48;5;204m\\]", 20);
    strncpy(colorDefs->rose, "\\[\\e[38;2;247;118;142m\\]", 28);
    strncpy(colorDefs->rose_bg, "\\[\\e[48;2;247;118;142m\\]", 28);
    strncpy(colorDefs->peach, "\\[\\e[38;2;250;179;135m\\]", 28);
    strncpy(colorDefs->peach_bg, "\\[\\e[48;2;250;179;135m\\]", 28);
    strncpy(colorDefs->orange, "\\[\\e[38;2;255;149;20m\\]", 27);
    strncpy(colorDefs->orange_bg, "\\[\\e[48;2;255;149;20m\\]", 27);
    strncpy(colorDefs->khaki, "\\[\\e[38;2;238;232;170m\\]", 28);
    strncpy(colorDefs->khaki_bg, "\\[\\e[48;2;238;232;170m\\]", 28);
    strncpy(colorDefs->cyan, "\\[\\e[38;2;115;218;202m\\]", 28);
    strncpy(colorDefs->cyan_bg, "\\[\\e[48;2;115;218;202m\\]", 28);
    strncpy(colorDefs->blue, "\\[\\e[38;2;122;162;247m\\]", 28);
    strncpy(colorDefs->blue_bg, "\\[\\e[48;2;122;162;247m\\]", 28);
    strncpy(colorDefs->sky, "\\[\\e[38;2;137;220;250m\\]", 28);
    strncpy(colorDefs->sky_blue, "\\[\\e[38;5;117m\\]", 20);
    strncpy(colorDefs->sky_blue_bg, "\\[\\e[48;5;117m\\]", 20);
    strncpy(colorDefs->teal, "\\[\\e[38;2;0;103;103m\\]", 26);
    strncpy(colorDefs->teal_bg, "\\[\\e[48;2;0;103;103m\\]", 26);
    strncpy(colorDefs->orchid, "\\[\\e[38;2;186;154;247m\\]", 28);
    strncpy(colorDefs->orchid_bg, "\\[\\e[48;2;186;154;247m\\]", 28);
    strncpy(colorDefs->light_purple, "\\[\\e[38;5;182m\\]", 20);
    strncpy(colorDefs->light_purple_bg, "\\[\\e[48;5;182m\\]", 20);
    strncpy(colorDefs->purple, "\\[\\e[38;2;153;102;204m\\]", 28);
    strncpy(colorDefs->purple_bg, "\\[\\e[48;2;153;102;204m\\]", 28);
    strncpy(colorDefs->gold, "\\[\\e[38;2;245;181;61m\\]", 27);
    strncpy(colorDefs->gold_bg, "\\[\\e[48;2;245;181;61m\\]", 27);
    strncpy(colorDefs->light_gold, "\\[\\e[38;2;248;199;109m\\]", 28);
    strncpy(colorDefs->light_gold_bg, "\\[\\e[48;2;248;199;109m\\]", 28);
    strncpy(colorDefs->smoke, "\\[\\e[38;2;104;155;196m\\]", 29);
    strncpy(colorDefs->smoke_bg, "\\[\\e[48;2;104;155;196m\\]", 29);
    strncpy(colorDefs->slate, "\\[\\e[38;2;65;72;104m\\]", 26);
    strncpy(colorDefs->slate_bg, "\\[\\e[48;2;65;72;104m\\]", 26);
    strncpy(colorDefs->light_slate, "\\[\\e[38;2;86;95;137m\\]", 26);
    strncpy(colorDefs->light_slate_bg, "\\[\\e[48;2;86;95;137m\\]", 26);
    strncpy(colorDefs->sandy_orange, "\\[\\e[38;2;255;158;100m\\]", 28);
    strncpy(colorDefs->sandy_orange_bg, "\\[\\e[48;2;255;158;100m\\]", 28);
    return;
}
