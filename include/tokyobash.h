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

typedef enum Styles {
    Pointed,
    Rounded,
    Slanted,
} Styles;

typedef struct FetchOpts {
    FetchModifier modifier;
    int limit;
} FetchOpts;

typedef struct ConfigSettings {
    bool time;
    bool fetch;
    bool debug;
    Themes theme;
    bool inARepo;
    bool statusbar;
    Styles bgstyle;
    bool background;
    bool branchname;
    char sep_left[4];
    char sep_right[4];
    bool gitAccessible;
    char timeformat[1];
    PathState pathState;
    FetchOpts fetchConfig;
    char left_line_sep[8];
    char right_line_sep[4];
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
    char bold[13];
    char reset[14];
    char usr_color[28];
    char usr_bg[28];
    char usr_txt[28];
    char time_color[28];
    char time_bg[28];
    char time_txt[28];
    char branch_color[28];
    char branch_bg[28];
    char branch_txt[28];
    char path_color[28];
    char path_bg[28];
    char path_txt[28];
    char curDir_txt[28];
    char untracked[28];
    char unstaged[28];
    char staged[28];
    char committed[28];
    char fetched[28];
} Colors;

void parseConfig(ConfigSettings* usrConfig, char* pHome);
void setSeparator(ConfigSettings* usrConfig);
void defineColors(ConfigSettings* usrConfig, Colors* colorDefs);
void replaceHome(char* path, int Plen, int Hlen);
void abrvPath(char* path, int Plen);
void remCurntDir(char* path, int Plen);

void printUsrTime(ConfigSettings* usrConfig, Colors* colorDefs);
void printBranch(ConfigSettings* usrConfig, Colors* colorDefs);
void printPathWithBg(ConfigSettings* usrConfig, Colors* colorDefs, char* path, int pathLength);
void printPathNoBg(ConfigSettings* usrConfig, Colors* colorDefs, char* path, int pathLength);
void printStatusBar(Colors* colorDefs, int untracked, int unstaged, int staged, int committed,
                    int fetched);

bool isGitAccessible(void);
bool checkIfInRepo(void);
void getBranch(char* branch_name);
int Committed(void);
void getStatusOf(int* staged, int* unstaged, int* untracked);
int Fetched(FetchOpts* fetchConfig);
bool shouldFetch(FetchOpts* fetchConfig);

#endif
