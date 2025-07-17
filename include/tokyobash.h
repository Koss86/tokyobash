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
    bool statusbar;
    bool branchname;
    FetchOpts fetchConfig;
} ConfigSettings;

typedef struct IntTimesAndDates {
    int curnt_year;
    int curnt_month;
    int curnt_day;
    int curnt_hour;
    int curnt_min;
    // int curnt_sec;
    int fetch_year;
    int fetch_month;
    int fetch_day;
    int fetch_hour;
    int fetch_min;
    // int fetch_sec;
} IntTimesnDates;

void parseConfig(ConfigSettings* usrConfig, char* pHome, int Hlen);
void replace_home(char* path, int Plen, int Hlen);
void abrv_path(char* path, int Plen);
void rem_curDir(char* path, int Plen);

bool git_is_accessible(void);
bool in_repo(void);
void get_branch(char* branch_name);
int Committed(void);
void get_status_of(int* staged, int* unstaged, int* untracked);

int Fetched(FetchOpts* fetchConfig);
bool shouldFetch(FetchOpts* fetchConfig);
void extractTimeData(IntTimesnDates*, char[], char[], char[], char[]);
void getDaysInMonth(int* daysInMonth, int month);

#endif
