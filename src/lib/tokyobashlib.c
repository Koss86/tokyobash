#include "../../include/tokyobash.h"

void parseConfig(ConfigSettings* usrConfig, char* pHome, int homeLength) {

    char path[PATH_MAX];
    char* pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[64] = "/tokyobash/config";
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
                            if (usrConfig->fetchConfig.limit > 31) {
                                usrConfig->fetchConfig.limit = 31;
                            }
                            break;
                    }

                } else if ((strncmp(keybuf, "time", 4)) == 0) {

                    if (valbuf[0] == '0') {
                        usrConfig->time = false;
                    } else if (valbuf[0] == '1') {
                        usrConfig->time = true;
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
}
