#include "../../include/tokyobash.h"

static void checkKeyValue(ConfigSettings* usrConfig, char* keybuf,
                          char* valbuf);

void parseConfig(ConfigSettings* usrConfig, char* pHome) {

    char path[PATH_MAX];
    char* pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[26] = "/tokyobash/config";
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

    FILE* configFile = fopen(path, "r");
    if (configFile == NULL) {
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

    while ((c = fgetc(configFile)) != EOF) {

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
            if (incomment && indx == 0) {

                inkey = true;
                incomment = false;
                invalue = false;
                continue;

            } else if (incomment) {
                incomment = false;
            }

            inkey = true;
            invalue = false;
            valbuf[indx] = '\0';
            indx = 0;
            checkKeyValue(usrConfig, &keybuf[0], &valbuf[0]);
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

        } else if (invalue) {
            valbuf[indx++] = c;
        }
    }
    fclose(configFile);

    switch (usrConfig->bgstyle) {

        case Pointed:
            strcpy(usrConfig->leftSep, "");
            strcpy(usrConfig->rightSep, "");
            strcpy(usrConfig->rightLineSep, "");
            strcpy(usrConfig->leftLineSep, "");
            //  ""
            break;

        case Rounded:
            strcpy(usrConfig->leftSep, "");
            strcpy(usrConfig->rightSep, "");
            strcpy(usrConfig->rightLineSep, "");
            strcpy(usrConfig->leftLineSep, "");
            // ""
            break;

        case Slanted:
            strcpy(usrConfig->leftSep, "");
            strcpy(usrConfig->rightSep, "");
            strcpy(usrConfig->rightLineSep, "");
            strcpy(usrConfig->leftLineSep, "");
            //  
            break;
    }
    return;
}
static void checkKeyValue(ConfigSettings* usrConfig, char* keybuf,
                          char* valbuf) {

    if ((strncmp(keybuf, "theme", 5)) == 0) {

        if ((strncmp(valbuf, "catppuccin", 10)) == 0) {
            usrConfig->theme = Catppuccin;

        } else if ((strncmp(valbuf, "kanagawa", 8)) == 0) {
            usrConfig->theme = Kanagawa;
        }

    } else if ((strncmp(keybuf, "background", 10)) == 0) {

        if (valbuf[0] == '1') {

            usrConfig->background = true;

        } else if (valbuf[0] == '2') {

            usrConfig->background = true;
            usrConfig->bgstyle = Rounded;

        } else if (valbuf[0] == '3') {

            usrConfig->background = true;
            usrConfig->bgstyle = Slanted;
        }

    } else if ((strncmp(keybuf, "statusbar", 9)) == 0) {

        if (valbuf[0] == '0') {
            usrConfig->statusbar = false;
        }

    } else if ((strncmp(keybuf, "debug", 5)) == 0) {

        if (valbuf[0] == '1') {
            usrConfig->debug = true;
        }

    } else if ((strncmp(keybuf, "branchname", 10)) == 0) {

        if (valbuf[0] == '0') {
            usrConfig->branchname = false;
        }

    } else if ((strncmp(keybuf, "fetchtimer", 10)) == 0) {

        if (valbuf[1] == 'h' || valbuf[2] == 'h') {

            usrConfig->fetchConfig.modifier = Hour;

        } else if (valbuf[1] == 'm' || valbuf[2] == 'm') {

            usrConfig->fetchConfig.modifier = Minute;
        }

        usrConfig->fetchConfig.limit = atoi(&valbuf[0]);

        if (usrConfig->fetchConfig.limit < 1) {

            usrConfig->fetchConfig.limit = 1;

        } else {

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
        }

    } else if ((strncmp(keybuf, "time", 4)) == 0) {

        if (valbuf[0] == '0') {
            usrConfig->time = false;
        } else if (valbuf[0] == '2') {
            usrConfig->timeFormat[0] = 'T';
        }

    } else if ((strncmp(keybuf, "fetch", 5)) == 0) {

        if (valbuf[0] == '1') {
            usrConfig->fetch = true;
        }
    }
    return;
}
// Replace $HOME with '~'.
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
// place last 23 chars after '...' .
void abrvPath(char* path, int pathLength) {

    const int ABV_PATH_LEN1 = 24;
    const int ABV_PATH_LEN2 = 23;

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
// changing text to bold and a lighter color. This way the path is normal, while
// current dir is highlighted.
void remCurntDir(char* path, int pathLength) {

    for (int i = pathLength - 1; i > -1; i--) {

        if (path[i] == '/') {
            path[i + 1] = '\0';
            break;
        }
    }
    return;
}
void defineColors(ConfigSettings* usrConfig, Colors* colorDefs) {

    strcpy(colorDefs->bold, "\\[\\e[1m\\]");
    strcpy(colorDefs->reset, "\\[\\e[00m\\]");
    strcpy(colorDefs->untracked, "\\[\\e[38;2;255;255;0m\\]"); // yellow
    strcpy(colorDefs->fetched, "\\[\\e[38;5;204m\\]");         // pink

    switch (usrConfig->theme) {

        case Tokyonight:

            if (usrConfig->background) {

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;0;103;103m\\]"); // dark_blue
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;115;218;202m\\]"); // cyan
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;115;218;202m\\]"); // cyan_bg
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;134;85;57m\\]"); // dark_orange
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;255;158;100m\\]"); // sandy_orange
                        strcpy(
                            colorDefs->usr_bg,
                            "\\[\\e[48;2;255;158;100m\\]"); // sandy_orange_bg
                        break;
                    case Root:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;130;66;77m\\]"); // dark_rose
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;247;118;142m\\]"); // rose
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;247;118;142m\\]"); // rose_bg
                        break;
                }

                strcpy(colorDefs->time_color,
                       "\\[\\e[38;2;65;72;104m\\]"); // slate
                strcpy(colorDefs->time_bg,
                       "\\[\\e[48;2;65;72;104m\\]"); // slate_bg
                strcpy(colorDefs->branch_color,
                       "\\[\\e[38;2;86;95;137m\\]"); // light_slate
                strcpy(colorDefs->branch_bg,
                       "\\[\\e[48;2;86;95;137m\\]"); // light_slate_bg
                strcpy(colorDefs->path_color, colorDefs->branch_color);
                strcpy(colorDefs->path_bg, colorDefs->branch_bg);

            } else { // No Bg

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;115;218;202m\\]"); // cyan
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;115;218;202m\\]"); // cyan
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;255;158;100m\\]"); // sandy_orange
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;255;158;100m\\]"); // sandy_orange
                        break;
                    case Root:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;247;118;142m\\]"); // rose
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;247;118;142m\\]"); // rose
                        break;
                }
            }

            strcpy(colorDefs->time_txt, "\\[\\e[38;5;117m\\]"); // sky_blue
            strcpy(colorDefs->branch_txt,
                   "\\[\\e[38;2;98;146;182m\\]");                       // smoke
            strcpy(colorDefs->path_txt, "\\[\\e[38;2;122;162;247m\\]"); // blue
            strcpy(colorDefs->curDir_txt,
                   "\\[\\e[38;2;140;172;248m\\]"); // light_blue

            strcpy(colorDefs->unstaged, "\\[\\e[38;2;255;149;20m\\]"); // orange
            strcpy(colorDefs->staged,
                   "\\[\\e[38;2;212;181;248m\\]"); // mocha_mauve
            strcpy(colorDefs->committed,
                   "\\[\\e[38;2;87;214;87m\\]"); // lime_green

            break;

        case Catppuccin:

            strcpy(colorDefs->untracked, "\\[\\e[38;2;255;255;0m\\]"); // yellow
            strcpy(colorDefs->bold, "\\[\\e[1m\\]");
            strcpy(colorDefs->reset, "\\[\\e[00m\\]");

            if (usrConfig->background) {

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;131;96;74m\\]"); // dark_peach
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;250;179;135m\\]"); // peach
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;250;179;135m\\]"); // peach_bg
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;76;96;131m\\]"); // dark_mocha_blue
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;137;180;250m\\]"); // mocha_blue
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;137;180;250m\\]"); // mocha_blue_bg
                        break;
                    case Root:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;89;119;87m\\]"); // dark_mocha_green
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;166;227;161m\\]"); // mocha_green
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;166;227;161m\\]"); // mocha_green_bg
                        break;
                }

                strcpy(colorDefs->time_color,
                       "\\[\\e[38;2;69;71;90m\\]"); // surface1
                strcpy(colorDefs->time_bg,
                       "\\[\\e[48;2;69;71;90m\\]"); // surface1_bg
                strcpy(colorDefs->branch_color,
                       "\\[\\e[38;2;88;91;112m\\]"); // surface2
                strcpy(colorDefs->branch_bg,
                       "\\[\\e[48;2;88;91;112m\\]"); // surface2_bg
                strcpy(colorDefs->path_color, colorDefs->branch_color);
                strcpy(colorDefs->path_bg, colorDefs->branch_bg);

            } else { // No Bg

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;250;179;135m\\]"); // peach
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;250;179;135m\\]"); // peach
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;137;180;250m\\]"); // mocha_blue
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;137;180;250m\\]"); // mocha_blue
                        break;
                    case Root:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;166;227;161m\\]"); // mocha_green
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[48;2;166;227;161m\\]"); // mocha_green
                        break;
                }
            }

            strcpy(colorDefs->time_txt,
                   "\\[\\e[38;2;245;224;220m\\]"); // rosewater
            strcpy(colorDefs->branch_txt,
                   "\\[\\e[38;2;249;226;175m\\]"); // mocha_yellow

            strcpy(colorDefs->path_txt,
                   "\\[\\e[38;2;212;181;248m\\]"); // mocha_mauve
            strcpy(colorDefs->curDir_txt,
                   "\\[\\e[38;2;222;197;250m\\]"); // light_mauve

            strcpy(colorDefs->unstaged,
                   "\\[\\e[38;2;255;158;100m\\]"); // sandy_orange
            strcpy(colorDefs->staged, "\\[\\e[38;2;122;162;247m\\]"); // blue
            strcpy(colorDefs->committed,
                   "\\[\\e[38;2;87;214;87m\\]"); // lime_green

            break;

        case Kanagawa:

            if (usrConfig->background) {

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;105;40;39m\\]"); // dark_autumn_red
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;195;64;66m\\]"); // autumn_red
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;195;64;66m\\]"); // autumn_red_bg
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;75;100;100m\\]"); // dark_waveAqua2
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;122;168;159m\\]"); // waveAqua2
                        strcpy(colorDefs->usr_bg,
                               "\\[\\e[48;2;122;168;159m\\]"); // waveAqua2_bg
                        break;
                    case Root:
                        strcpy(
                            colorDefs->usr_txt,
                            "\\[\\e[38;2;150;95;60m\\]"); // dark_surimiOrange
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;255;161;102m\\]"); // surimiOrange
                        strcpy(
                            colorDefs->usr_bg,
                            "\\[\\e[48;2;255;161;102m\\]"); // surimiOrange_bg
                        break;
                }

                strcpy(colorDefs->time_color,
                       "\\[\\e[38;2;73;68;60m\\]"); // winter_yellow
                strcpy(colorDefs->time_bg,
                       "\\[\\e[48;2;73;68;60m\\]"); // winter_yellow_bg
                strcpy(colorDefs->branch_color,
                       "\\[\\e[38;2;43;51;40m\\]"); // winter_green
                strcpy(colorDefs->branch_bg,
                       "\\[\\e[48;2;43;51;40m\\]"); // winter_green_bg
                strcpy(colorDefs->path_color, colorDefs->branch_color);
                strcpy(colorDefs->path_bg, colorDefs->branch_bg);

            } else { // No Bg

                switch (usrConfig->pathState) {
                    case Home:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;195;64;66m\\]"); // autumn_red
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;195;64;66m\\]"); // autumn_red
                        break;
                    case Mnt:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;122;168;159m\\]"); // waveAqua2
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;122;168;159m\\]"); // waveAqua2
                        break;
                    case Root:
                        strcpy(colorDefs->usr_txt,
                               "\\[\\e[38;2;255;161;102m\\]"); // surimiOrange
                        strcpy(colorDefs->usr_color,
                               "\\[\\e[38;2;255;161;102m\\]"); // surimiOrange
                        break;
                }
            }
            strcpy(colorDefs->time_txt,
                   "\\[\\e[38;2;126;156;216m\\]"); // crystal_blue
            strcpy(colorDefs->branch_txt,
                   "\\[\\e[38;2;192;163;110m\\]"); // boat_yellow2

            strcpy(colorDefs->path_txt,
                   "\\[\\e[38;2;230;196;132m\\]"); // carp_yellow
            strcpy(colorDefs->curDir_txt,
                   "\\[\\e[38;2;234;201;145m\\]"); // light_carp_yellow

            strcpy(colorDefs->unstaged,
                   "\\[\\e[38;2;255;161;102m\\]"); // surimiOrange
            strcpy(colorDefs->staged, "\\[\\e[38;2;122;162;247m\\]"); // blue
            strcpy(colorDefs->committed,
                   "\\[\\e[38;2;87;214;87m\\]"); // lime_green

            break;
    }
    return;
}
