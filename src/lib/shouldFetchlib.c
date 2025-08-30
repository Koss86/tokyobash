#include "../../include/tokyobash.h"

static bool getFetchTime(char*, char*);
static void extractTimeData(IntTimesnDates*, char[], char[], char[], char[]);
static void getDaysInMonth(int*, int);

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

    if (!getFetchTime(&fetch_date[0], &fetch_time[0])) {
        return false;
    }

    IntTimesnDates time;
    extractTimeData(&time, curnt_date, curnt_time, fetch_date, fetch_time);

    FetchModifier modifier = fetchConfig->modifier;
    int limit = fetchConfig->limit;

    const int MONTHS_IN_YR = 12;
    const int HOURS_IN_DAY = 24;
    const int MINS_IN_HOUR = 60;
    int days_in_month = 0;

    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    if (time.curnt_year != time.fetch_year) { // Year

        yearDif = time.curnt_year - time.fetch_year;
        if (yearDif > 1) {
            return true;
        }
    }

    if (time.curnt_month != time.fetch_month) { // Month

        if (yearDif == 0) {
            monthDif = time.curnt_month - time.fetch_month;
        } else {
            monthDif = (MONTHS_IN_YR - time.fetch_month) + time.curnt_month;
        }

        if (monthDif > 1) {

            if ((time.curnt_month != 3 && time.fetch_month != 1) || modifier != Day) {
                return true;
            } else {
                getDaysInMonth(&days_in_month, time.fetch_month);
                dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
                dayDif += 28; // add month of Febuary.

                if (dayDif >= limit) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    if (time.curnt_day != time.fetch_day || monthDif > 0) { // Day

        if (monthDif == 0) {
            dayDif = time.curnt_day - time.fetch_day;
        } else {
            getDaysInMonth(&days_in_month, time.fetch_month);
            dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
        }

        if (modifier != Day && dayDif > 1) {
            return true;
        }

        else if (modifier == Day) {

            if (dayDif > limit) {
                return true;
            } else if (dayDif < limit) {
                return false;
            }

            if (time.curnt_hour < time.fetch_hour) {
                return false;
            }

            if (time.curnt_hour > time.fetch_hour) {
                return true;
            }

            if (time.curnt_min < time.fetch_min) {
                return false;
            } else {
                return true;
            }
        }
    }

    if (modifier == Hour || time.curnt_hour != time.fetch_hour) { // Hour

        if (dayDif == 0) {
            hrDif = time.curnt_hour - time.fetch_hour;
        } else {
            hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
        }

        if (modifier == Hour) {

            if ((hrDif == limit && time.curnt_min >= time.fetch_min) || hrDif > limit) {
                return true;
            }
            if (hrDif <= limit) {
                return false;
            }

        } else if (hrDif > 1) {
            return true;
        }
    }

    if (time.curnt_min != time.fetch_min) { // Minute

        if (hrDif == 0) {
            minDif = time.curnt_min - time.fetch_min;
        } else {
            minDif = (MINS_IN_HOUR - time.fetch_min) + time.curnt_min;
        }

        if (minDif >= limit) {
            return true;
        }
    }
    return false;
}
static bool getFetchTime(char* fetch_date, char* fetch_time) {

    char c;
    int indx = 0;
    char buf[64];
    char path[256];
    FILE* file;
    FILE* fetch_status;

    file = popen("git rev-parse --show-cdup", "r");

    if (file == NULL) {
        return false;
    }
    while ((c = fgetc(file)) != EOF) {
        buf[indx++] = c;
    }
    pclose(file);

    if (buf[0] == '\n') {
        fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    } else {
        int len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        path[0] = '\0';
        strcat(path, "stat ");
        strcat(path, buf);
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
            space++;
            if (space == 1) {
                inDate = true;
            } else if (space == 2) {
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
static void extractTimeData(IntTimesnDates* dateData, char curnt_date[], char curnt_time[],
                            char fetch_date[], char fetch_time[]) {

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
static void getDaysInMonth(int* daysInMonth, int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11)
        *daysInMonth = 30;
    else if (month != 2)
        *daysInMonth = 31;
    else
        *daysInMonth = 28;
    return;
}
