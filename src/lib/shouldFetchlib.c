#include "../../include/tokyobash.h"

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

    FILE* fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");

    if (fetch_status == NULL) {
        return false;
    }

    char c;
    int indx = 0;
    int newline = 0;
    int space = 0;
    bool inDate = false;
    bool inTime = false;
    char fetch_date[11];
    char fetch_time[9];

    // If the return of 'stat .git/FETCH_HEAD' ever
    // changes, this will need to be refactored.
    while ((c = fgetc(fetch_status)) != EOF) {

        if (c == '\n') {
            newline++;
            continue;
        }

        // Date and time we want are on the 7th line,
        // so after 6 \n we are on the line we want to be.
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

    const int MONTHS_IN_YR = 12;
    const int HOURS_IN_DAY = 24;
    const int MINS_IN_HOUR = 60;
    int days_in_month = 0;

    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    IntTimesnDates timeData;
    extractTimeData(&timeData, curnt_date, curnt_time, fetch_date, fetch_time);

    if (timeData.curnt_year != timeData.fetch_year) { // Year

        yearDif = timeData.curnt_year - timeData.fetch_year;

        if (yearDif > 1) {

            return true;
        }
    }

    if (timeData.curnt_month != timeData.fetch_month) { // Month

        if (timeData.curnt_month >= timeData.fetch_month) {

            monthDif = timeData.curnt_month - timeData.fetch_month;

        } else {

            monthDif = (MONTHS_IN_YR - timeData.fetch_month) + timeData.curnt_month;
        }

        if (monthDif > 1) {

            if ((timeData.curnt_month != 3 && timeData.fetch_month != 1) ||
                fetchConfig->modifier != Day) {

                return true;

            } else {

                getDaysInMonth(&days_in_month, timeData.fetch_month);
                dayDif = (days_in_month - timeData.fetch_day) + timeData.curnt_day;
                dayDif += 28; // add month of Febuary.

                if (dayDif >= fetchConfig->limit) {

                    return true;

                } else {

                    return false;
                }
            }
        }
    }

    if (timeData.curnt_day != timeData.fetch_day) { // Day

        if (monthDif == 0) {

            dayDif = timeData.curnt_day - timeData.fetch_day;

        } else {

            getDaysInMonth(&days_in_month, timeData.fetch_month);
            dayDif = (days_in_month - timeData.fetch_day) + timeData.curnt_day;
        }

        if ((fetchConfig->modifier == Day && dayDif > fetchConfig->limit) ||
            (fetchConfig->modifier != Day && dayDif > 1)) {

            return true;

        } else if (fetchConfig->modifier == Day && dayDif < fetchConfig->limit) {

            return false;

        } else if (fetchConfig->modifier == Day) { // and dayDif and limit are equal

            if (timeData.curnt_hour >= timeData.fetch_hour) {

                hrDif = timeData.curnt_hour - timeData.fetch_hour;

            } else {

                hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
            }

            if (hrDif > 1) {

                return false;

            } else {

                if (timeData.curnt_min >= timeData.fetch_min) {

                    return true;

                } else {

                    return false;
                }
            }
        }
    }

    if (timeData.curnt_hour != timeData.fetch_hour || fetchConfig->modifier == Hour) { // Hour

        if (timeData.curnt_hour >= timeData.fetch_hour) {

            hrDif = timeData.curnt_hour - timeData.fetch_hour;

        } else {

            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
        }

        if ((fetchConfig->modifier == Hour && hrDif > fetchConfig->limit) ||
            (fetchConfig->modifier != Hour && hrDif > 1)) {

            return true;

        } else if (fetchConfig->modifier == Hour && hrDif < fetchConfig->limit) {

            return false;

        } else if (fetchConfig->modifier == Hour) {

            if (timeData.curnt_min >= timeData.fetch_min) {

                return true;

            } else {

                return false;
            }
        }
    }

    if (timeData.curnt_min != timeData.fetch_min || fetchConfig->modifier == Minute) { // Minute

        if (hrDif == 0) {

            minDif = timeData.curnt_min - timeData.fetch_min;

        } else {

            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
        }

        if (fetchConfig->modifier == Minute) {

            if (minDif >= fetchConfig->limit) {

                return true;
            }
        }
    }
    return false;
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
}
static void getDaysInMonth(int* daysInMonth, int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        *daysInMonth = 30;
    } else if (month != 2) {
        *daysInMonth = 31;
    } else {
        *daysInMonth = 28;
    }
}
