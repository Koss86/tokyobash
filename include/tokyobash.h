#ifndef TOKYOBASH_H
#define TOKYOBASH_H

#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_BRANCH_LEN 256
#define ABV_PATH_LEN1 24
#define ABV_PATH_LEN2 23
#define ABV_PATH_LEN_T 50

typedef enum PathState {
    Home,
    Mnt,
    Root,
} PathState;

typedef enum Themes {
    Tokyonight,
    Catppuccin,
    Kanagawa,
    Orange,
} Themes;

typedef enum FetchModifier {
    Minute,
    Hour,
    Day,
} FetchModifier;

typedef struct FetchOpts {
    FetchModifier modifier;
    int limit;
} FetchOpts;

typedef struct ConfigSettings {
    bool git;
    bool time;
    bool debug;
    Themes theme;
    bool inARepo;
    bool statusbar;
    bool background;
    bool branchname;
    bool gitAccessible;
    FetchOpts fetchConfig;
} ConfigSettings;

typedef struct IntTimesAndDates {
    int curnt_year;
    int curnt_month;
    int curnt_day;
    int curnt_hour;
    int curnt_min;
    int fetch_year;
    int fetch_month;
    int fetch_day;
    int fetch_hour;
    int fetch_min;
} IntTimesnDates;

typedef struct Colors {
    char bold[15];
    char reset[15];
    char red[30];
    char desat_firebrick[30];
    char pink[30];
    char rose[30];
    char peach[30];
    char dark_orange[30];
    char orange[30];
    char yellow[30];
    char green[30];
    char lime_green[30];
    char desat_lime[30];
    char cyan[30];
    char blue[30];
    char sky_blue[30];
    char teal[30];
    char orchid[30];
    char light_purple[30];
    char purple[30];
    char khaki[30];
    char white[30];
    char black[30];
    char beige[30];

    char blue_bg[30];
    char sky_blue_bg[30];
    char cyan_bg[30];
    char teal_bg[30];
    char pink_bg[30];
    char orange_bg[30];
} Colors;

typedef struct ColorPointers {
    char* bold;
    char* reset;
    char* usr_color;
    char* time_color;
    char* branch_color;
    char* path_color;
    char* mnt_color;
    char* root_color;

    char* untracked_color;
    char* unstaged_color;
    char* staged_color;
    char* committed_color;
    char* fetched_color;

    char* txt_fg_color;
    char* usr_bg;
    char* time_bg;
    char* branch_bg;
    char* path_bg;
    char* mnt_bg;
    char* root_bg;
} ColorPointers;

void parseConfig(ConfigSettings* usrConfig, char* pHome, int Hlen);
void populateColors(Colors* colors);
void assignPointers(ColorPointers* pointers, Colors* colors, ConfigSettings* usrConfig);
void replaceHome(char* path, int Plen, int Hlen);
void abrvPath(char* path, int Plen);
void remCurntDir(char* path, int Plen);
void printPathWithBg(ConfigSettings* usrConfig, ColorPointers* pointers, PathState pathState,
                     char* path, int pathLength);
void printPathNoBg(ColorPointers* pointers, PathState pathState, char* path, int pathLength);

bool isGitAccessible(void);
bool checkIfInRepo(void);
void getBranch(char* branch_name);
int Committed(void);
void getStatusOf(int* staged, int* unstaged, int* untracked);
int Fetched(FetchOpts* fetchConfig);
bool shouldFetch(FetchOpts* fetchConfig);

#endif
