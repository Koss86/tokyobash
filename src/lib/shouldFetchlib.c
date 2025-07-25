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
    FetchModifier modifier = fetchConfig->modifier;
    int limit = fetchConfig->limit;

    IntTimesnDates time;
    extractTimeData(&time, curnt_date, curnt_time, fetch_date, fetch_time);

    if (time.curnt_year != time.fetch_year) { // Year

        yearDif = time.curnt_year - time.fetch_year;

        if (yearDif > 1) {

            return true;
        }
    }

    if (time.curnt_month != time.fetch_month) { // Month

        if (time.curnt_month >= time.fetch_month) {

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

    if (time.curnt_day != time.fetch_day) { // Day

        if (monthDif == 0) {

            dayDif = time.curnt_day - time.fetch_day;

        } else {

            getDaysInMonth(&days_in_month, time.fetch_month);
            dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
        }

        if ((modifier == Day && dayDif > limit) || (modifier != Day && dayDif > 1)) {

            return true;

        } else if (modifier == Day && dayDif < limit) {

            return false;

        } else if (modifier == Day) { // and dayDif and limit are equal

            if (dayDif == 0) {

                hrDif = time.curnt_hour - time.fetch_hour;

            } else {

                hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
            }

            if (hrDif > HOURS_IN_DAY) {

                return false;

            } else {

                if (time.curnt_min >= time.fetch_min) {

                    return true;

                } else {

                    return false;
                }
            }
        }
    }

    if (time.curnt_hour != time.fetch_hour || modifier == Hour) { // Hour

        if (time.curnt_hour >= time.fetch_hour) {

            hrDif = time.curnt_hour - time.fetch_hour;

        } else {

            hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
        }

        if ((modifier == Hour && hrDif > limit) || (modifier != Hour && hrDif > 1)) {

            return true;

        } else if (modifier == Hour && hrDif < limit) {

            return false;

        } else if (modifier == Hour) {

            if (time.curnt_min >= time.fetch_min) {

                return true;

            } else {

                return false;
            }
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
