#include "../../include/tokyobash.h"

static void extractTimeData(IntTimesnDates*, char[], char[], char[], char[]);
static void getDaysInMonth(int*, int);
static bool shouldFetch(FetchOpts*);

// Checks when last the repo was updated.
// Calls fetch if longer than specified in
// usrConfig.fetchConfig->limit. Then returns
// number of commits from remote.
int Fetched(FetchOpts* fetchConfig) {

    if (shouldFetch(fetchConfig)) {
        FILE* gitFetch = popen("git fetch 2>/dev/null", "r");
        pclose(gitFetch);
    }

    FILE* file = popen("git rev-list --count ..@{u} 2>/dev/null", "r");
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
static bool shouldFetch(FetchOpts* fetchConfig) {

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

        if (timeData.curnt_min >= timeData.fetch_min) {

            minDif = timeData.curnt_min - timeData.fetch_min;

        } else {

            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
        }

        if (fetchConfig->modifier == Minute /*&& dayDif > 0*/) {

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
static void getDaysInMonth(int* daysInMonth, int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        *daysInMonth = 30;
    } else if (month != 2) {
        *daysInMonth = 31;
    } else {
        *daysInMonth = 28;
    }
}
