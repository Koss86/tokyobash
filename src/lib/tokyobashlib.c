#include "../../include/tokyobash.h"

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
                    }

                } else if ((strncmp(keybuf, "style", 5)) == 0) {

                    if ((strncmp(valbuf, "pointed", 7)) == 0) {

                        usrConfig->style = Pointed;

                    } else if ((strncmp(valbuf, "rounded", 7)) == 0) {

                        usrConfig->style = Rounded;

                    } else if ((strncmp(valbuf, "slanted", 7)) == 0) {

                        usrConfig->style = Slanted;
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
    setSeparator(usrConfig);
    return;
}
// Set the look of separators.
void setSeparator(ConfigSettings* usrConfig) {

    switch (usrConfig->style) {

        case Pointed:
            strcpy(usrConfig->sep_left, "");
            strcpy(usrConfig->sep_right, "");
            strcpy(usrConfig->right_line_sep, "");
            strcpy(usrConfig->left_line_sep, "");
            // strcpy(usrConfig->thin_sep_left, "");
            break;

        case Rounded:
            strcpy(usrConfig->sep_left, "");
            strcpy(usrConfig->sep_right, "");
            strcpy(usrConfig->right_line_sep, "");
            strcpy(usrConfig->left_line_sep, "");
            // strcpy(usrConfig->thin_sep_left, "");
            break;

        case Slanted:
            strcpy(usrConfig->sep_left, "");
            strcpy(usrConfig->sep_right, "");
            strcpy(usrConfig->right_line_sep, "");
            strcpy(usrConfig->left_line_sep, "");
            // actual slanted thin separator
            // left = "" right = ""
            // 
            break;
    }
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
// changing text to bold and a lighter color. This way the path is normal, while current dir is
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
void defineColors(ConfigSettings* usrConfig, Colors* colorDefs) {

    char lime_green[] = "\\[\\e[38;2;87;214;87m\\]";
    char blue[] = "\\[\\e[38;2;122;162;247m\\]";
    char pink[] = "\\[\\e[38;5;204m\\]";

    if (usrConfig->theme == Tokyonight) {

        char dark_blue[] = "\\[\\e[38;2;0;103;103m\\]";
        char dark_orange[] = "\\[\\e[38;2;134;85;57m\\]";
        char dark_rose[] = "\\[\\e[38;2;130;66;77m\\]";
        char cyan[] = "\\[\\e[38;2;115;218;202m\\]";
        char cyan_bg[] = "\\[\\e[48;2;115;218;202m\\]";
        char sandy_orange[] = "\\[\\e[38;2;255;158;100m\\]";
        char sandy_orange_bg[] = "\\[\\e[48;2;255;158;100m\\]";
        char rose[] = "\\[\\e[38;2;247;118;142m\\]";
        char rose_bg[] = "\\[\\e[48;2;247;118;142m\\]";
        char light_slate[] = "\\[\\e[38;2;86;95;137m\\]";
        char light_slate_bg[] = "\\[\\e[48;2;86;95;137m\\]";
        char slate[] = "\\[\\e[38;2;65;72;104m\\]";
        char slate_bg[] = "\\[\\e[48;2;65;72;104m\\]";
        char sky_blue[] = "\\[\\e[38;5;117m\\]";
        char smoke[] = "\\[\\e[38;2;98;146;182m\\]";
        char light_blue[] = "\\[\\e[38;2;140;172;248m\\]";
        char orange[] = "\\[\\e[38;2;255;149;20m\\]";
        char mocha_mauve[] = "\\[\\e[38;2;212;181;248m\\]";

        if (usrConfig->background) {

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, dark_blue);
                    strcpy(colorDefs->usr_color, cyan);
                    strcpy(colorDefs->usr_bg, cyan_bg);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, dark_orange);
                    strcpy(colorDefs->usr_color, sandy_orange);
                    strcpy(colorDefs->usr_bg, sandy_orange_bg);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, dark_rose);
                    strcpy(colorDefs->usr_color, rose);
                    strcpy(colorDefs->usr_bg, rose_bg);
                    break;
            }

            strcpy(colorDefs->time_color, slate);
            strcpy(colorDefs->time_bg, slate_bg);
            strcpy(colorDefs->branch_color, light_slate);
            strcpy(colorDefs->branch_bg, light_slate_bg);
            strcpy(colorDefs->path_color, light_slate);
            strcpy(colorDefs->path_bg, light_slate_bg);

        } else { // No Bg

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, cyan);
                    strcpy(colorDefs->usr_color, cyan);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, sandy_orange);
                    strcpy(colorDefs->usr_color, sandy_orange);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, rose);
                    strcpy(colorDefs->usr_color, rose);
                    break;
            }
        }

        strcpy(colorDefs->time_txt, sky_blue);
        strcpy(colorDefs->branch_txt, smoke);

        strcpy(colorDefs->path_txt, blue);
        strcpy(colorDefs->curDir_txt, light_blue);

        strcpy(colorDefs->unstaged, orange);
        strcpy(colorDefs->staged, mocha_mauve);
        strcpy(colorDefs->committed, lime_green);
        strcpy(colorDefs->fetched, pink);

    } else if (usrConfig->theme == Catppuccin) {

        char dark_peach[] = "\\[\\e[38;2;131;96;74m\\]";
        char dark_mocha_blue[] = "\\[\\e[38;2;76;96;131m\\]";
        char dark_mocha_green[] = "\\[\\e[38;2;89;119;87m\\]";
        char peach[] = "\\[\\e[38;2;250;179;135m\\]";
        char peach_bg[] = "\\[\\e[48;2;250;179;135m\\]";
        char mocha_blue[] = "\\[\\e[38;2;137;180;250m\\]";
        char mocha_blue_bg[] = "\\[\\e[48;2;137;180;250m\\]";
        char mocha_green[] = "\\[\\e[38;2;166;227;161m\\]";
        char mocha_green_bg[] = "\\[\\e[48;2;166;227;161m\\]";
        char rosewater[] = "\\[\\e[38;2;245;224;220m\\]";
        char surface1[] = "\\[\\e[38;2;69;71;90m\\]";
        char surface1_bg[] = "\\[\\e[48;2;69;71;90m\\]";
        char surface2[] = "\\[\\e[38;2;88;91;112m\\]";
        char surface2_bg[] = "\\[\\e[48;2;88;91;112m\\]";
        char mocha_yellow[] = "\\[\\e[38;2;249;226;175m\\]";
        char mocha_mauve[] = "\\[\\e[38;2;212;181;248m\\]";
        char light_mauve[] = "\\[\\e[38;2;222;197;250m\\]";
        char sandy_orange[] = "\\[\\e[38;2;255;158;100m\\]";

        if (usrConfig->background) {

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, dark_peach);
                    strcpy(colorDefs->usr_color, peach);
                    strcpy(colorDefs->usr_bg, peach_bg);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, dark_mocha_blue);
                    strcpy(colorDefs->usr_color, mocha_blue);
                    strcpy(colorDefs->usr_bg, mocha_blue_bg);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, dark_mocha_green);
                    strcpy(colorDefs->usr_color, mocha_green);
                    strcpy(colorDefs->usr_bg, mocha_green_bg);
                    break;
            }

            strcpy(colorDefs->time_color, surface1);
            strcpy(colorDefs->time_bg, surface1_bg);
            strcpy(colorDefs->branch_color, surface2);
            strcpy(colorDefs->branch_bg, surface2_bg);
            strcpy(colorDefs->path_color, surface2);
            strcpy(colorDefs->path_bg, surface2_bg);

        } else { // No Bg

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, peach);
                    strcpy(colorDefs->usr_color, peach);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, mocha_blue);
                    strcpy(colorDefs->usr_color, mocha_blue);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, mocha_green);
                    strcpy(colorDefs->usr_color, mocha_green);
                    break;
            }
        }

        strcpy(colorDefs->time_txt, rosewater);
        strcpy(colorDefs->branch_txt, mocha_yellow);

        strcpy(colorDefs->path_txt, mocha_mauve);
        strcpy(colorDefs->curDir_txt, light_mauve);

        strcpy(colorDefs->unstaged, sandy_orange);
        strcpy(colorDefs->staged, blue);
        strcpy(colorDefs->committed, lime_green);
        strcpy(colorDefs->fetched, pink);

    } else { // Kanagawa

        char dark_autumn_red[] = "\\[\\e[38;2;105;40;39m\\]";
        char dark_waveAqua2[] = "\\[\\e[38;2;75;100;100m\\]";
        char dark_surimiOrange[] = "\\[\\e[38;2;150;95;60m\\]";

        char autumn_red[] = "\\[\\e[38;2;195;64;66m\\]";
        char autumn_red_bg[] = "\\[\\e[48;2;195;64;66m\\]";
        char waveAqua2[] = "\\[\\e[38;2;122;168;159m\\]";
        char waveAqua2_bg[] = "\\[\\e[48;2;122;168;159m\\]";
        char surimiOrange[] = "\\[\\e[38;2;255;161;102m\\]";
        char surimiOrange_bg[] = "\\[\\e[48;2;255;161;102m\\]";
        char crystal_blue[] = "\\[\\e[38;2;126;156;216m\\]";
        char winter_yellow[] = "\\[\\e[38;2;73;68;60m\\]";
        char winter_yellow_bg[] = "\\[\\e[48;2;73;68;60m\\]";
        char boat_yellow2[] = "\\[\\e[38;2;192;163;110m\\]";
        char winter_green[] = "\\[\\e[38;2;43;51;40m\\]";
        char winter_green_bg[] = "\\[\\e[48;2;43;51;40m\\]";
        char carp_yellow[] = "\\[\\e[38;2;230;196;132m\\]";
        char light_carp_yellow[] = "\\[\\e[38;2;234;201;145m\\]";

        if (usrConfig->background) {

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, dark_autumn_red);
                    strcpy(colorDefs->usr_color, autumn_red);
                    strcpy(colorDefs->usr_bg, autumn_red_bg);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, dark_waveAqua2);
                    strcpy(colorDefs->usr_color, waveAqua2);
                    strcpy(colorDefs->usr_bg, waveAqua2_bg);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, dark_surimiOrange);
                    strcpy(colorDefs->usr_color, surimiOrange);
                    strcpy(colorDefs->usr_bg, surimiOrange_bg);
                    break;
            }

            strcpy(colorDefs->time_color, winter_yellow);
            strcpy(colorDefs->time_bg, winter_yellow_bg);
            strcpy(colorDefs->branch_color, winter_green);
            strcpy(colorDefs->branch_bg, winter_green_bg);
            strcpy(colorDefs->path_color, winter_green);
            strcpy(colorDefs->path_bg, winter_green_bg);

        } else { // No Bg

            switch (usrConfig->pathState) {
                case Home:
                    strcpy(colorDefs->usr_txt, autumn_red);
                    strcpy(colorDefs->usr_color, autumn_red);
                    break;
                case Mnt:
                    strcpy(colorDefs->usr_txt, waveAqua2);
                    strcpy(colorDefs->usr_color, waveAqua2);
                    break;
                case Root:
                    strcpy(colorDefs->usr_txt, surimiOrange);
                    strcpy(colorDefs->usr_color, surimiOrange);
                    break;
            }
        }
        strcpy(colorDefs->time_txt, crystal_blue);
        strcpy(colorDefs->branch_txt, boat_yellow2);

        strcpy(colorDefs->path_txt, carp_yellow);
        strcpy(colorDefs->curDir_txt, light_carp_yellow);

        strcpy(colorDefs->unstaged, surimiOrange);
        strcpy(colorDefs->staged, blue);
        strcpy(colorDefs->committed, lime_green);
        strcpy(colorDefs->fetched, pink);
    }

    strcpy(colorDefs->untracked, "\\[\\e[38;2;255;255;0m\\]"); // yellow
    strcpy(colorDefs->bold, "\\[\\e[1m\\]");
    strcpy(colorDefs->reset, "\\[\\e[00m\\]");

    return;
}
