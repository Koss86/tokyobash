#include <stdio.h>
#include <stdlib.h>

#include "../../include/tokyobash.h"

bool shouldFetch(FetchOpts *fetchConfig);
void extractTimeData(IntTimesnDates *, char[], char[], char[], char[]);
void getDaysInMonth(int *daysInMonth, int month);

int main() {
    FetchOpts settings;
    settings.state = Minute;
    settings.limit = 15;
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
    int secDif = 0;

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

        if (timeData.curnt_month > timeData.fetch_month) {
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

        if (timeData.curnt_day > timeData.fetch_day) {
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

            // if (timeData.curnt_hour > timeData.fetch_hour) {
            // hrDif = timeData.curnt_hour - timeData.fetch_hour;
            //} else {
            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
            //}

            printf("in Day: hrDif = %i\n", hrDif);

            if (hrDif > HOURS_IN_DAY) {
                return true;
            } else if (hrDif < HOURS_IN_DAY) {
                return false;
            } else {

                if (timeData.curnt_min > timeData.fetch_min) {
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

        if (timeData.curnt_hour > timeData.fetch_hour) {
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

        if (timeData.curnt_min > timeData.fetch_min) {
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

    char cbuf[3];
    char fbuf[3];
    int YrIndx = 2;
    int MonthIndx = 5;
    int DayIndx = 8;
    int HrIndx = 0;
    int MinIndx = 3;

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[YrIndx + i];
        fbuf[i] = fetch_date[YrIndx + i];
    }
    dateData->curnt_year = atoi(cbuf);
    dateData->fetch_year = atoi(fbuf);

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[MonthIndx + i];
        fbuf[i] = fetch_date[MonthIndx + i];
    }
    dateData->curnt_month = atoi(cbuf);
    dateData->fetch_month = atoi(fbuf);

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_date[DayIndx + i];
        fbuf[i] = fetch_date[DayIndx + i];
    }
    dateData->curnt_day = atoi(cbuf);
    dateData->fetch_day = atoi(fbuf);

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_time[HrIndx + i];
        fbuf[i] = fetch_time[HrIndx + i];
    }
    dateData->curnt_hour = atoi(cbuf);
    dateData->fetch_hour = atoi(fbuf);

    for (int i = 0; i < 3; i++) {
        if (i == 2) {
            cbuf[i] = '\0';
            fbuf[i] = '\0';
            break;
        }
        cbuf[i] = curnt_time[MinIndx + i];
        fbuf[i] = fetch_time[MinIndx + i];
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
