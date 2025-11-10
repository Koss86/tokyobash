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
    bool abv;
    bool time;
    bool fetch;
    bool debug;
    Themes theme;
    bool inARepo;
    char icon[20];
    bool statusbar;
    bool background;
    bool branchname;
    char leftSep[4];
    char rightSep[4];
    bool gitAccessible;
    char timeFormat[1];
    PathState pathState;
    FetchOpts fetchConfig;
    char leftLineSep[8];
    char rightLineSep[4];
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

void parseConfig(ConfigSettings*, char*);
void defineColors(ConfigSettings*, Colors*);
// Replace $HOME with '~'.
void replaceHome(char* path, int pLeng, int hLeng);
// If path length is greater than 50, keep first 24 chars, add '...' then
// place last 23 chars after '...' .
void abrvPath(char* path, int pLeng);
// Remove current directory from path. We add it back with \W after
// changing text to bold and a lighter color. This way the path is normal, while
// current dir is highlighted.
void remCurntDir(char* path, int pLeng);

void printUsrTime(ConfigSettings*, Colors*);
void printBranch(ConfigSettings*, Colors*);
void printPathWithBg(ConfigSettings*, Colors*, char* path, int pLeng);
void printPathNoBg(ConfigSettings*, Colors*, char* path, int pLeng);
void printStatusBar(Colors*, int ut, int us, int s, int c, int f);

// If git is available, return true.
bool isGitAccessible(void);
// If current directory is a repository, return true.
bool checkIfInRepo(void);
// Place branch name into provided buffer.
void getBranch(char*);
// Returns how many commits are ready to be pushed.
int Committed(void);
// Counts how many staged, unstaged, and untracked files in the repo
// on the current branch, and assigns the values to the provided variables.
void getStatusOf(int* s, int* us, int* ut);
// Checks when last the repo was updated. If longer than specified in
// fetchConfig.limit, fetch is called and return number of commits from remote.
int Fetched(FetchOpts*);
bool shouldFetch(FetchOpts*);

#endif
