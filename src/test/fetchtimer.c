#include "../../include/tokyobash.h"

typedef struct {
    char curnt_date[11];
    char fetch_date[11];
    char curnt_time[9];
    char fetch_time[9];
    bool expected;
    FetchOpts settings;
} Tester;

bool shouldFetchTest(Tester* fetchConfig);
void extractTimeData(IntTimesnDates*, char[], char[], char[], char[]);
void getDaysInMonth(int* daysInMonth, int month);

#define TEST_SIZE 18

int main() {

    Tester* times;
    if ((times = malloc(sizeof(Tester) * TEST_SIZE)) == NULL) {
        perror("malloc failed.");
        exit(1);
    }

    int in = 0;
    ///////// Day Tests ///////////
    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-03");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-03");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "23:58:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-03");
    strcpy(times[in].fetch_date, "2025-02-27");
    strcpy(times[in].curnt_time, "23:55:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-02");
    strcpy(times[in].fetch_date, "2025-02-27");
    strcpy(times[in].curnt_time, "23:35:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-04-03");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "23:23:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    ///////// Hour Tests ///////////
    times[in].settings.modifier = Hour;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "02:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "02:58:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 1;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "00:58:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 3;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "02:23:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "02:55:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 3;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "00:35:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    ///////// Minute Tests ///////////
    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "00:05:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "00:09:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 1;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "00:00:43");
    strcpy(times[in].fetch_time, "23:59:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 1;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-31");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "23:59:43");
    strcpy(times[in].fetch_time, "23:58:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "00:05:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "00:05:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "00:02:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    int pass = 0;
    int expectedtrue = 0;
    for (int i = 0; i < TEST_SIZE; i++) {
        printf("Test %i\nModifier: ", i + 1);
        if (times[i].settings.modifier == Day) {
            printf("Day");
        } else if (times[i].settings.modifier == Hour) {
            printf("Hour");
        } else {
            printf("Minute");
        }
        printf("  Limit: %i\n", times[i].settings.limit);
        printf("Current Date: %s\n", times[i].curnt_date);
        printf("Fetch Date: %s\n", times[i].fetch_date);
        printf("Current Time: %s\n", times[i].curnt_time);
        printf("Fetch Time: %s\n\n", times[i].fetch_time);

        if (shouldFetchTest(&times[i])) {

            printf("\nResult: True");
            pass++;

            if (times[i].expected == false) {
                printf(" TEST FAILED");
            }

        } else {

            printf("\nResult: False");

            if (times[i].expected == true) {
                printf(" TEST FAILED");
            }
        }

        printf("\n");

        printf("Expected: ");
        if (times[i].expected == true) {
            printf("True\n");
            expectedtrue++;
        } else {
            printf("False\n");
        }

        printf("\n");
    }
    printf("Passed: %i  Expected: %i \n", pass, expectedtrue);
}
bool shouldFetchTest(Tester* fetchConfig) {

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
    extractTimeData(&timeData, fetchConfig->curnt_date, fetchConfig->curnt_time,
                    fetchConfig->fetch_date, fetchConfig->fetch_time);

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

        if ((fetchConfig->settings.modifier == Day && dayDif > fetchConfig->settings.limit) ||
            (fetchConfig->settings.modifier != Day && dayDif > 1)) {

            return true;

        } else if (fetchConfig->settings.modifier == Day && dayDif < fetchConfig->settings.limit) {

            return false;

        } else if (fetchConfig->settings.modifier == Day) { // and dayDif and limit are equal

            if (timeData.curnt_hour >= timeData.fetch_hour) {
                hrDif = timeData.curnt_hour - timeData.fetch_hour;
            } else {
                hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
            }

            printf("in Day: hrDif = %i\n", hrDif);

            if (hrDif > 1) {

                return false;

            } else {

                if (timeData.curnt_min >= timeData.fetch_min) {
                    minDif = timeData.curnt_min - timeData.fetch_min;
                } else {
                    minDif = timeData.fetch_min - timeData.curnt_min;
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

    if (timeData.curnt_hour != timeData.fetch_hour ||
        fetchConfig->settings.modifier == Hour) { // Hour

        if (timeData.curnt_hour >= timeData.fetch_hour) {
            hrDif = timeData.curnt_hour - timeData.fetch_hour;
        } else {
            hrDif = (HOURS_IN_DAY - timeData.fetch_hour) + timeData.curnt_hour;
        }

        printf("in Hour: hrDif = %i\n", hrDif);

        if ((fetchConfig->settings.modifier == Hour && hrDif > fetchConfig->settings.limit) ||
            (fetchConfig->settings.modifier != Hour && hrDif > 1)) {

            return true;

        } else if (fetchConfig->settings.modifier == Hour && hrDif < fetchConfig->settings.limit) {

            return false;

        } else if (fetchConfig->settings.modifier == Hour) {

            if (timeData.curnt_min >= timeData.fetch_min) {
                minDif = timeData.curnt_min - timeData.fetch_min;
            } else {
                minDif = timeData.fetch_min - timeData.curnt_min;
            }

            printf("in Hour: minDif = %i\n", minDif);

            if (timeData.curnt_min >= timeData.fetch_min) {
                return true;
            } else {
                return false;
            }
        }
    }

    if (timeData.curnt_min != timeData.fetch_min ||
        fetchConfig->settings.modifier == Minute) { // Minute

        if (timeData.curnt_min >= timeData.fetch_min) {
            minDif = timeData.curnt_min - timeData.fetch_min;
        } else {
            minDif = (MINS_IN_HOUR - timeData.fetch_min) + timeData.curnt_min;
        }

        printf("in Minute: minDif = %i\n", minDif);

        // fix:fetch called when curnt & fetch times are equal
        //  - added dayDiff check
        if (fetchConfig->settings.modifier == Minute /*&& dayDif > 0*/) {
            if (minDif >= fetchConfig->settings.limit) {
                return true;
            }
        }
    }
    return false;
}

void extractTimeData(IntTimesnDates* dateData, char curnt_date[], char curnt_time[],
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

void getDaysInMonth(int* daysInMonth, int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        *daysInMonth = 30;
    } else if (month != 2) {
        *daysInMonth = 31;
    } else {
        *daysInMonth = 28;
    }
}
