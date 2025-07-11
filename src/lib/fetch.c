#include "../../include/tokyobash.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Checks when last the repo was updated,
// calls fetch if longer than user specified in
// usrConfig.fetchSettings->limit. Then, returns
// number of commits if any from remote.
int Fetched(FetchOpts *fetchSettings) {

    if (shouldFetch(fetchSettings)) {
        FILE *gitFetch = popen("git fetch 2>/dev/null", "r");
        pclose(gitFetch);
    }

    FILE *file = popen("git rev-list --count ..@{u} 2>/dev/null", "r");
    if (file == NULL) {
        return -1;
    }

    char buf[16];
    if (fgets(buf, sizeof(buf), file) == NULL) {
        pclose(file);
        return -1;
    }

    pclose(file);
    return atoi(buf);
}
bool shouldFetch(FetchOpts *fetchSettings) {

    time_t now = time(0);
    struct tm *time_struct = localtime(&now);

    char curnt_date[11];
    char curnt_time[9];

    if (!strftime(curnt_date, sizeof(curnt_date), "%Y-%m-%d", time_struct)) {
        return false;
    }

    if (!strftime(curnt_time, sizeof(curnt_time), "%X", time_struct)) {
        return false;
    }

    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");

    if (fetch_status == NULL) {
        return false;
    }

    char c;
    int indx = 0;
    int newline = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
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
                inDate = 1;
            } else if (space == 2) {
                inTime = 1;
                inDate = 0;
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

    int daysInFMonth = 0;
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

        if (timeData.curnt_month > timeData.fetch_month) {
            monthDif = timeData.curnt_month - timeData.fetch_month;
        } else {
            monthDif = (MONTHS_IN_YR - timeData.fetch_month) + timeData.curnt_month;
        }

        if (monthDif > 1) {

            return true;
        }
    }

    if (timeData.curnt_day != timeData.fetch_day) { // Day

        getDaysInMonth(&daysInFMonth, timeData.fetch_month);

        if (timeData.curnt_day > timeData.fetch_day) {
            dayDif = timeData.curnt_day - timeData.fetch_day;
        } else {
            dayDif = (daysInFMonth - timeData.fetch_day) + timeData.curnt_day;
        }

        if ((fetchSettings->state == Day && dayDif > fetchSettings->limit) ||
            (fetchSettings->state != Day && dayDif > 1)) {

            return true;

        } else if (fetchSettings->state == Day && dayDif < fetchSettings->limit) {

            return false;

        } else if (fetchSettings->state == Day) {

            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;

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

                if (timeData.curnt_min >= timeData.fetch_min) {

                    return true;

                } else {

                    return false;
                }
            }
        }
    }

    if (timeData.curnt_hour != timeData.fetch_hour || fetchSettings->state == Hour) { // Hour

        if (timeData.curnt_hour > timeData.fetch_hour) {
            hrDif = timeData.curnt_hour - timeData.fetch_hour;
        } else {
            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
        }

        if ((fetchSettings->state == Hour && hrDif > fetchSettings->limit) ||
            (fetchSettings->state != Hour && hrDif > 1)) {

            return true;

        } else if (fetchSettings->state == Hour && hrDif < fetchSettings->limit) {

            return false;

        } else if (fetchSettings->state == Hour) {

            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;

            if (timeData.curnt_min >= timeData.fetch_min) {
                return true;
            } else {
                return false;
            }
        }
    }

    if (timeData.curnt_min != timeData.fetch_min || fetchSettings->state == Minute) { // Minute

        if (timeData.curnt_min > timeData.fetch_min) {
            minDif = timeData.curnt_min - timeData.fetch_min;
        } else {
            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
        }

        if (fetchSettings->state == Minute) {
            if (minDif >= fetchSettings->limit) {
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
    int MonIndx = 5;
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
        cbuf[i] = curnt_date[MonIndx + i];
        fbuf[i] = fetch_date[MonIndx + i];
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
