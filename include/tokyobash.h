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
    bool fetch;
    bool debug;
    Themes theme;
    bool inARepo;
    bool statusbar;
    bool background;
    bool branchname;
    bool gitAccessible;
    PathState pathState;
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
    char tok_usr_txt[26];
    char tok_mnt_txt[26];
    char tok_root_txt[26];

    char cat_usr_txt[26];
    char cat_mnt_txt[26];
    char cat_root_txt[26];

    char kan_usr_txt[26];
    char kan_mnt_txt[27];
    char kan_root_txt[27];

    char bold[13];
    char reset[14];
    char red[26];
    char yellow[26];
    char lime_green[27];
    char sky_blue[20];
    char pink[20];
    char spring_green[28];
    char carp_yellow[28];
    char crystal_blue[28];
    char rosewater[28];
    char mocha_mauve[28];
    char mocha_yellow[28];
    char boat_yellow2[28];
    char orange[27];
    char blue[28];
    char smoke[28];
    char gold[27];

    char winter_green[25];
    char winter_green_bg[25];
    char winter_yellow[25];
    char winter_yellow_bg[25];
    char mocha_green[28];
    char mocha_green_bg[28];
    char mocha_blue[28];
    char mocha_blue_bg[28];
    char surface1[25];
    char surface1_bg[25];
    char surface2[26];
    char surface2_bg[26];
    char rose[28];
    char rose_bg[28];
    char peach[28];
    char peach_bg[28];
    char cyan[28];
    char cyan_bg[28];
    char slate[26];
    char slate_bg[26];
    char light_slate[26];
    char light_slate_bg[26];
    char autumn_red[26];
    char autumn_red_bg[26];
    char surimiOrange[28];
    char surimiOrange_bg[28];
    char sandy_orange[28];
    char sandy_orange_bg[28];
    char waveAqua2[28];
    char waveAqua2_bg[28];
} Colors;

typedef struct ColorPointers {

    char* bold;
    char* reset;

    char* usr_color;
    char* usr_bg;
    char* usr_txt;
    char* time_color;
    char* time_bg;
    char* time_txt;
    char* branch_color;
    char* branch_bg;
    char* branch_txt;
    char* path_color;
    char* path_bg;
    char* path_txt;

    char* untracked;
    char* unstaged;
    char* staged;
    char* committed;
    char* fetched;
} ColorPointers;

void parseConfig(ConfigSettings* usrConfig, char* pHome);
void colorDefinitions(Colors* colors);
void assignPointers(ColorPointers* colors, Colors* colorsDefs, ConfigSettings* usrConfig);
void replaceHome(char* path, int Plen, int Hlen);
void abrvPath(char* path, int Plen);
void remCurntDir(char* path, int Plen);

void printUsrTime(ConfigSettings* usrConfig, ColorPointers* colors);
void printBranch(ConfigSettings* usrConfig, ColorPointers* colors);
void printPathWithBg(ConfigSettings* usrConfig, ColorPointers* colors, char* path, int pathLength);
void printPathNoBg(ConfigSettings* usrConfig, ColorPointers* colors, char* path, int pathLength);
void printStatusBar(ColorPointers* colors, int untracked, int unstaged, int staged, int committed,
                    int fetched);

bool isGitAccessible(void);
bool checkIfInRepo(void);
void getBranch(char* branch_name);
int Committed(void);
void getStatusOf(int* staged, int* unstaged, int* untracked);
int Fetched(FetchOpts* fetchConfig);
bool shouldFetch(FetchOpts* fetchConfig);

#endif
