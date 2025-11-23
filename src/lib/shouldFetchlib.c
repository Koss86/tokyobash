#include "../../include/tokyobash.h"

#define MONTHS_IN_YR 12
#define HOURS_IN_DAY 24
#define MINS_IN_HOUR 60

static int getDaysInMonth(int);
static bool getFetchTime(char*, char*);
static bool checkTimeDay(int, IntTimesnDates*);
static bool checkTimeHr(int, IntTimesnDates*);
static bool checkTimeMin(int, IntTimesnDates*);
static void charTimeToInt(IntTimesnDates*, char[], char[], char[], char[]);

bool shouldFetch(FetchOpts* fetchConfig) {

    time_t now = time(0);
    struct tm* time_struct = localtime(&now);

    char curnt_date[11];
    char curnt_time[9];

    if (!strftime(curnt_date, sizeof(curnt_date), "%Y-%m-%d", time_struct)) {
        return false;
    }
    if (!strftime(curnt_time, sizeof(curnt_time), "%X", time_struct)) {
        return false;
    }

    char fetch_date[11];
    char fetch_time[9];

    if (!getFetchTime(fetch_date, fetch_time)) {
        return false;
    }

    IntTimesnDates time;
    charTimeToInt(&time, curnt_date, curnt_time, fetch_date, fetch_time);

    switch (fetchConfig->modifier) {

        case Day:
            return checkTimeDay(fetchConfig->limit, &time);

        case Hour:
            return checkTimeHr(fetchConfig->limit, &time);

        case Minute:
            return checkTimeMin(fetchConfig->limit, &time);
    }

    return false;
}

static bool checkTimeDay(int limit, IntTimesnDates* time) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;

    yearDif = time->curnt_year - time->fetch_year;
    if (yearDif > 1) {
        return true;
    }

    if (yearDif == 0) {
        monthDif = time->curnt_month - time->fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time->fetch_month) + time->curnt_month;
    }

    if (monthDif > 1) {

        if (time->curnt_month != 3 && time->fetch_month != 1) {
            return true;
        } else {
            days_in_month = getDaysInMonth(time->fetch_month);
            dayDif = (days_in_month - time->fetch_day) + time->curnt_day;
            dayDif += 28; // add month of Febuary.

            if (dayDif > limit) {
                return true;
            }
        }
    }

    if (monthDif == 0) {
        dayDif = time->curnt_day - time->fetch_day;
    } else {
        days_in_month = getDaysInMonth(time->fetch_month);
        dayDif = (days_in_month - time->fetch_day) + time->curnt_day;
    }

    if (dayDif > limit) {
        return true;
    } else if (dayDif < limit) {
        return false;
    }

    if (time->curnt_hour > time->fetch_hour) {
        return true;
    } else if (time->curnt_hour < time->fetch_hour) {
        return false;
    }

    if (time->curnt_min >= time->fetch_min) {
        return true;
    }
    return false;
}

static bool checkTimeHr(int limit, IntTimesnDates* time) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;

    yearDif = time->curnt_year - time->fetch_year;

    if (yearDif > 1) {
        return true;
    }

    if (yearDif == 0) {
        monthDif = time->curnt_month - time->fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time->fetch_month) + time->curnt_month;
    }

    if (monthDif > 1) {
        return true;
    }

    if (monthDif == 0) {
        dayDif = time->curnt_day - time->fetch_day;
    } else {
        days_in_month = getDaysInMonth(time->fetch_month);
        dayDif = (days_in_month - time->fetch_day) + time->curnt_day;
    }

    if (dayDif > 1) {
        return true;
    }

    if (dayDif == 0) {
        hrDif = time->curnt_hour - time->fetch_hour;
    } else {
        hrDif = (HOURS_IN_DAY - time->fetch_hour) + time->curnt_hour;
    }

    if (hrDif > limit) {
        return true;
    } else if (hrDif < limit) {
        return false;
    }

    if (time->curnt_min >= time->fetch_min) {
        return true;
    }
    return false;
}

static bool checkTimeMin(int limit, IntTimesnDates* time) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    yearDif = time->curnt_year - time->fetch_year;

    if (yearDif > 1) {
        return true;
    }

    if (yearDif == 0) {
        monthDif = time->curnt_month - time->fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time->fetch_month) + time->curnt_month;
    }

    if (monthDif > 1) {
        return true;
    }

    if (monthDif == 0) {
        dayDif = time->curnt_day - time->fetch_day;
    } else {
        days_in_month = getDaysInMonth(time->fetch_month);
        dayDif = (days_in_month - time->fetch_day) + time->curnt_day;
    }

    if (dayDif > 1) {
        return true;
    }

    if (dayDif == 0) {
        hrDif = time->curnt_hour - time->fetch_hour;
    } else {
        hrDif = (HOURS_IN_DAY - time->fetch_hour) + time->curnt_hour;
    }

    if (hrDif > 1) {
        return true;
    }

    if (hrDif == 0) {
        minDif = time->curnt_min - time->fetch_min;
    } else {
        minDif = (MINS_IN_HOUR - time->fetch_min) + time->curnt_min;
    }

    if (minDif >= limit) {
        return true;
    }
    return false;
}

static bool getFetchTime(char* fetch_date, char* fetch_time) {

    char c;
    int indx = 0;
    char dist_buf[64];
    char path[MAX_BRANCH_LEN];
    FILE* dist_to_root;
    FILE* fetch_status;

    dist_to_root = popen("git rev-parse --show-cdup", "r");

    if (dist_to_root == NULL) {
        return false;
    }
    if ((fgets(&dist_buf[0], 64, dist_to_root)) == NULL) {
        return false;
    }

    pclose(dist_to_root);

    if (dist_buf[0] == '\n' || dist_buf[0] == '\0') {

        fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");

    } else {

        int len = strlen(dist_buf);
        if (dist_buf[len - 1] == '\n') {
            dist_buf[len - 1] = '\0';
        }
        path[0] = '\0';
        strcat(path, "stat ");
        strcat(path, dist_buf);
        strcat(path, ".git/FETCH_HEAD 2>/dev/null");
        fetch_status = popen(path, "r");
    }
    if (fetch_status == NULL) {
        return false;
    }

    indx = 0;
    int newline = 0;
    int space = 0;
    bool inDate = false;
    bool inTime = false;

    while ((c = fgetc(fetch_status)) != EOF) {

        if (c == '\n') {
            newline++;
            continue;
        }

        if (newline == 6 && c == ' ') {
            inDate = true;
            if (++space == 2) {
                inTime = true;
                inDate = false;
            }
            continue;
        }

        if (inDate) {
            fetch_date[indx++] = c;
            if (indx == 10) {
                fetch_date[indx] = '\0';
                indx = 0;
            }
            continue;
        }

        if (inTime) {
            fetch_time[indx++] = c;
            if (indx == 8) {
                fetch_time[indx] = '\0';
                break;
            }
        }
    }
    pclose(fetch_status);
    return true;
}
static void charTimeToInt(IntTimesnDates* dateData, char curnt_date[],
                          char curnt_time[], char fetch_date[],
                          char fetch_time[]) {
    const int YR_INDX = 2;
    const int MONTH_INDX = 5;
    const int DAY_INDX = 8;
    const int HR_INDX = 0;
    const int MIN_INDX = 3;

    dateData->curnt_year = atoi(&curnt_date[YR_INDX]);
    dateData->fetch_year = atoi(&fetch_date[YR_INDX]);

    dateData->curnt_month = atoi(&curnt_date[MONTH_INDX]);
    dateData->fetch_month = atoi(&fetch_date[MONTH_INDX]);

    dateData->curnt_day = atoi(&curnt_date[DAY_INDX]);
    dateData->fetch_day = atoi(&fetch_date[DAY_INDX]);

    dateData->curnt_hour = atoi(&curnt_time[HR_INDX]);
    dateData->fetch_hour = atoi(&fetch_time[HR_INDX]);

    dateData->curnt_min = atoi(&curnt_time[MIN_INDX]);
    dateData->fetch_min = atoi(&fetch_time[MIN_INDX]);
    return;
}

static int getDaysInMonth(int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month != 2) {
        return 31;
    } else {
        return 28;
    }
}
