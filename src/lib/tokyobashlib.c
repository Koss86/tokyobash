#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/tokyobash.h"

void parse_config(ConfigSettings *usrConfig, char *pHome, int Hleng) {

    char path[PATH_MAX];
    char *pConfig = getenv("XDG_CONFIG_HOME");
    char filepath[PATH_MAX] = "/tokyobash/config";

    int Flen = 17;
    if (pConfig == NULL || pConfig[0] == '\0') {
        Flen = 25;
        strncpy(filepath, "/.config/tokyobash/config", Flen + 1);

        for (int i = 0; i < Hleng + Flen; i++) {
            if (i < Hleng) {
                path[i] = pHome[i];
                continue;
            }
            path[i] = filepath[i - Hleng];
        }
        path[Hleng + Flen] = '\0';

    } else {

        int Cleng = strlen(pConfig);
        // This check may not be needed but it's here just in case.
        if (pConfig[Cleng - 1] == '/') {
            pConfig[Cleng - 1] = '\0';
            Cleng--;
        }

        for (int i = 0; i < Cleng + Flen; i++) {
            if (i < Cleng) {
                path[i] = pConfig[i];
                continue;
            }
            path[i] = filepath[i - Cleng];
        }
        path[Cleng + Flen] = '\0';
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    char c;
    int inkey = 1;
    int invalue = 0;
    int incomment = 0;
    int slash = 0;
    int indx = 0;
    char keybuf[64];
    char valbuf[64];
    char fetchbuf[3];

    while ((c = fgetc(file)) != EOF) {

        if (c == ' ' || c == '/' || c == '\'' || c == '"') {
            if (c == '/') {
                slash++;
            }
            if (slash == 2) {
                incomment = 1;
                slash = 0;
            }
            continue;
        }
        if (c == '#') {
            incomment = 1;
            continue;
        }

        if (c == '\n') {
            if (incomment) {
                incomment = 0;
                inkey = 1;
                invalue = 0;
                indx = 0;
                continue;
            }
            inkey = 1;
            invalue = 0;
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

                    if ((valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'm') ||
                        valbuf[2] == 'm') {

                        usrConfig->fetchConfig.modifier = Minute;

                        if (valbuf[2] == 'm') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                            if (usrConfig->fetchConfig.limit > 60) {
                                usrConfig->fetchConfig.limit = 60;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                        }

                    } else if ((valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'h') ||
                               valbuf[2] == 'h') {

                        usrConfig->fetchConfig.modifier = Hour;

                        if (valbuf[2] == 'h') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                            if (usrConfig->fetchConfig.limit > 24) {
                                usrConfig->fetchConfig.limit = 24;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                        }

                    } else if ((valbuf[0] != '-' && valbuf[0] != '0' && valbuf[1] == 'd') ||
                               valbuf[2] == 'd') {

                        usrConfig->fetchConfig.modifier = Day;

                        if (valbuf[2] == 'd') {
                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = valbuf[1];
                            fetchbuf[2] = '\0';

                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                            if (usrConfig->fetchConfig.limit > 31) {
                                usrConfig->fetchConfig.limit = 31;
                            }

                        } else {

                            fetchbuf[0] = valbuf[0];
                            fetchbuf[1] = '\0';
                            usrConfig->fetchConfig.limit = atoi(fetchbuf);
                        }

                    } else {
                        // Justin Case
                        usrConfig->fetchConfig.modifier = Hour;
                        usrConfig->fetchConfig.limit = 1;
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
            invalue = 1;
            inkey = 0;
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
void replace_home(char *path, int Plen, int Hlen) {

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
// If path lenth is greater than 50, keep first 24 chars, add '...' then
// place last 23 chars after last '.' .
void abrv_path(char *path, int Plen) {

    for (int i = ABV_PATH_LEN1; i < ABV_PATH_LEN1 + 3; i++) {
        path[i] = '.';
    }

    int indx = Plen - ABV_PATH_LEN2;

    for (int i = ABV_PATH_LEN1 + 3; i < ABV_PATH_LEN_T; i++) {
        path[i] = path[indx++];
    }
    path[ABV_PATH_LEN_T] = '\0';
}
// Remove current directory from path. We add it back with \\W after
// changing text to bold. This way the path is normal, while current dir is
// highlighted.
void rem_curDir(char *path, int Plen) {

    for (int i = Plen - 1; i > -1; i--) {

        if (path[i] == '/') {
            path[i + 1] = '\0';
            break;
        }
    }
}
