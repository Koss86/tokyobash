#include <stdio.h>
#include <stdlib.h>

#include "../../include/tokyobash.h"

bool shouldFetch(FetchOpts *fetchConfig);
void extractTimeData(IntTimesnDates *, char[], char[], char[], char[]);
void getDaysInMonth(int *daysInMonth, int month);

int main() {
    FetchOpts settings;
    settings.state = Hour;
    settings.limit = 3;
    if (shouldFetch(&settings)) {
        printf("True\n");
    } else {
        printf("False\n");
    }
}

bool shouldFetch(FetchOpts *fetchConfig) {

    char curnt_date[] = "2025-03-31";
    char fetch_date[] = "2025-03-31";

    char curnt_time[] = "23:55:43";
    char fetch_time[] = "23:55:22";

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
        printf("in Year: yearDif = %i\n", yearDif);

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

        printf("in Month: monthDif = %i\n", monthDif);

        if (monthDif > 1) {

            return true;
        }
    }

    if (timeData.curnt_day != timeData.fetch_day) { // Day

        if (timeData.curnt_day >= timeData.fetch_day) {
            dayDif = timeData.curnt_day - timeData.fetch_day;
        } else {
            getDaysInMonth(&days_in_month, timeData.fetch_month);
            dayDif = (days_in_month - timeData.fetch_day) + timeData.curnt_day;
        }

        printf("in Day: dayDif = %i\n", dayDif);

        if ((fetchConfig->state == Day && dayDif > fetchConfig->limit) ||
            (fetchConfig->state != Day && dayDif > 1)) {

            return true;

        } else if (fetchConfig->state == Day && dayDif < fetchConfig->limit) {

            return false;

        } else if (fetchConfig->state == Day) {

            if (timeData.curnt_hour >= timeData.fetch_hour) {
                hrDif = timeData.curnt_hour - timeData.fetch_hour;
            } else {
                hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
            }

            printf("in Day: hrDif = %i\n", hrDif);

            if (hrDif > HOURS_IN_DAY) {
                return true;
            } else if (hrDif < HOURS_IN_DAY) {
                return false;
            } else {

                if (timeData.curnt_min >= timeData.fetch_min) {
                    minDif = timeData.curnt_min - timeData.fetch_min;
                } else {
                    minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
                }
                printf("in Day: minDif = %i\n", minDif);

                if (timeData.curnt_min >= timeData.fetch_min) {

                    return true;

                } else {

                    return false;
                }
            }
        }
    }

    if (timeData.curnt_hour != timeData.fetch_hour || fetchConfig->state == Hour) { // Hour

        // iss:calling fetch when current and fetch times are the same.
        // Which means once fetch is called, its called EVERY time. So thats bad.
        // fix: chage '>' to '>=' in 'if (timeData.curnt_hour > timeData.fetch_hour)'
        if (timeData.curnt_hour >= timeData.fetch_hour) {
            hrDif = timeData.curnt_hour - timeData.fetch_hour;
        } else {
            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
        }

        printf("in Hour: hrDif = %i\n", hrDif);

        if ((fetchConfig->state == Hour && hrDif > fetchConfig->limit) ||
            (fetchConfig->state != Hour && hrDif > 1)) {

            return true;

        } else if (fetchConfig->state == Hour && hrDif < fetchConfig->limit) {

            return false;

        } else if (fetchConfig->state == Hour) {

            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;

            printf("in Hour: minDif = %i\n", minDif);

            if (timeData.curnt_min >= timeData.fetch_min) {
                return true;
            } else {
                return false;
            }
        }
    }

    if (timeData.curnt_min != timeData.fetch_min || fetchConfig->state == Minute) { // Minute

        if (timeData.curnt_min >= timeData.fetch_min) {
            minDif = timeData.curnt_min - timeData.fetch_min;
        } else {
            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
        }

        printf("in Minute: minDif = %i\n", minDif);

        // fix:fetch called when curnt & fetch times are equal
        //  - added dayDiff check
        if (fetchConfig->state == Minute && dayDif > 0) {
            if (minDif >= fetchConfig->limit) {
                return true;
            }
        }
    }
    return false;
}

void extractTimeData(IntTimesnDates *dateData, char curnt_date[], char curnt_time[],
                     char fetch_date[], char fetch_time[]) {

    const int YR_INDX = 2;
    const int MONTH_INDX = 5;
    const int DAY_INDX = 8;
    const int HR_INDX = 0;
    const int MIN_INDX = 3;

    const int BUF_SIZE = 3;

    char cbuf[BUF_SIZE];
    char fbuf[BUF_SIZE];

    for (int i = 0; i < BUF_SIZE; i++) {
        if (i == BUF_SIZE - 1) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[YR_INDX + i];
        fbuf[i] = fetch_date[YR_INDX + i];
    }
    dateData->curnt_year = atoi(cbuf);
    dateData->fetch_year = atoi(fbuf);

    for (int i = 0; i < BUF_SIZE; i++) {
        if (i == BUF_SIZE - 1) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[MONTH_INDX + i];
        fbuf[i] = fetch_date[MONTH_INDX + i];
    }
    dateData->curnt_month = atoi(cbuf);
    dateData->fetch_month = atoi(fbuf);

    for (int i = 0; i < BUF_SIZE; i++) {
        if (i == BUF_SIZE - 1) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[DAY_INDX + i];
        fbuf[i] = fetch_date[DAY_INDX + i];
    }
    dateData->curnt_day = atoi(cbuf);
    dateData->fetch_day = atoi(fbuf);

    for (int i = 0; i < BUF_SIZE; i++) {
        if (i == BUF_SIZE - 1) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_time[HR_INDX + i];
        fbuf[i] = fetch_time[HR_INDX + i];
    }
    dateData->curnt_hour = atoi(cbuf);
    dateData->fetch_hour = atoi(fbuf);

    for (int i = 0; i < BUF_SIZE; i++) {
        if (i == BUF_SIZE - 1) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_time[MIN_INDX + i];
        fbuf[i] = fetch_time[MIN_INDX + i];
    }
    dateData->curnt_min = atoi(cbuf);
    dateData->fetch_min = atoi(fbuf);
}

void getDaysInMonth(int *daysInMonth, int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        *daysInMonth = 30;
    } else if (month != 2) {
        *daysInMonth = 31;
    } else {
        *daysInMonth = 28;
    }
}
