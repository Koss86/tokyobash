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
void generateTests(Tester* times);

#define TEST_SIZE 36

int main() {

    Tester* times;
    if ((times = malloc(sizeof(Tester) * TEST_SIZE)) == NULL) {
        perror("malloc failed.");
        exit(1);
    }

    generateTests(times);

    int failed;
    char buffer[16];
    int return_true = 0;
    int return_false = 0;
    int expectedtrue = 0;
    int expectedfalse = 0;
    char failedTests[TEST_SIZE * 3];

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
            return_true++;

            if (times[i].expected == false) {
                printf(" TEST FAILED");
                sprintf(&buffer[0], "%d ", i + 1);
                strcat(failedTests, buffer);
                failed++;
            }

        } else {

            printf("\nResult: False");
            return_false++;

            if (times[i].expected == true) {
                printf(" TEST FAILED");
                sprintf(&buffer[0], "%d ", i + 1);
                strcat(failedTests, buffer);
                failed++;
            }
        }

        printf("\n");
        printf("Expected: ");

        if (times[i].expected == true) {

            printf("True\n");
            expectedtrue++;

        } else {

            expectedfalse++;
            printf("False\n");
        }
        printf("\n");
    }

    printf("%2d Tests Ran\n", TEST_SIZE);
    printf("%2d Test Failed\n", failed);
    printf("%2d Returned True  %2d Expected to Return True\n", return_true, expectedtrue);
    printf("%2d Retruned False %2d Expected to Return False\n", return_false,
           TEST_SIZE - expectedtrue);
    if (failed != 0) {
        printf("Tests That Failed: %s\n", failedTests);
    }
    free(times);
    return 0;
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
    FetchModifier modifier = fetchConfig->settings.modifier;
    int limit = fetchConfig->settings.limit;

    IntTimesnDates time;
    extractTimeData(&time, fetchConfig->curnt_date, fetchConfig->curnt_time,
                    fetchConfig->fetch_date, fetchConfig->fetch_time);

    if (time.curnt_year != time.fetch_year) { // Year

        yearDif = time.curnt_year - time.fetch_year;
        printf("in Year: yearDif = %i\n", yearDif);

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

        printf("in Month: monthDif = %i\n", monthDif);

        if (monthDif > 1) {

            if ((time.curnt_month != 3 && time.fetch_month != 1) || modifier != Day) {

                return true;

            } else {

                getDaysInMonth(&days_in_month, time.fetch_month);
                dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
                dayDif += 28; // add month of Febuary.

                printf("in Month: dayDif = %d\n", dayDif);

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

        printf("in Day: dayDif = %i\n", dayDif);

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

            printf("in Day: hrDif = %i\n", hrDif);

            if (hrDif > HOURS_IN_DAY) {

                return false;

            } else {

                ////////////// REMOVE FROM MAIN LIB ////////////////
                if (time.curnt_min >= time.fetch_min) {

                    minDif = time.curnt_min - time.fetch_min;

                } else {

                    minDif = time.fetch_min - time.curnt_min;
                }

                printf("in Day: minDif = %i\n", minDif);
                /////////////////////////////////////////////////////
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

        printf("in Hour: hrDif = %i\n", hrDif);
        if ((modifier == Hour && hrDif > limit) || (modifier != Hour && hrDif > 1)) {

            return true;

        } else if (modifier == Hour && hrDif < limit) {

            return false;

        } else if (modifier == Hour) {

            ////////////// REMOVE FROM MAIN LIB ////////////////
            if (time.curnt_min >= time.fetch_min) {
                // if (hrDif > 0){

                minDif = time.curnt_min - time.fetch_min;

            } else {

                minDif = time.fetch_min - time.curnt_min;
            }

            printf("in Hour: minDif = %i\n", minDif);
            ////////////////////////////////////////////////////
            if (time.curnt_min >= time.fetch_min) {

                return true;

            } else {

                return false;
            }
        }
    }

    if (time.curnt_min != time.fetch_min) { // Minute

        if (hrDif == 0) {
            // if (time.curnt_min >= time.fetch_min) {

            minDif = time.curnt_min - time.fetch_min;

        } else {

            minDif = (MINS_IN_HOUR - time.fetch_min) + time.curnt_min;
        }

        printf("in Minute: minDif = %i\n", minDif);

        if (minDif >= limit) {

            return true;
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
// Be sure to update TEST_SIZE after adding or removing tests.
void generateTests(Tester* times) {
    int in = 0;
    ///////// Day Tests ///////////
    times[in].settings.modifier = Day;
    times[in].settings.limit = 5;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2026-01-05");
    strcpy(times[in].fetch_date, "2025-12-31");
    strcpy(times[in].curnt_time, "00:01:43");
    strcpy(times[in].fetch_time, "13:59:43");
    in++;

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

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-30");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-02-28");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-29");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 15;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-13");
    strcpy(times[in].fetch_date, "2025-02-26");
    strcpy(times[in].curnt_time, "23:40:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-13");
    strcpy(times[in].fetch_date, "2025-02-26");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 13;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-24");
    strcpy(times[in].fetch_date, "2025-01-04");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-24");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "23:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 30;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "23:50:43");
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
    strcpy(times[in].curnt_time, "02:35:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 5;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "00:02:43");
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

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-01-31");
    strcpy(times[in].curnt_time, "00:02:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 60;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "00:35:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 55;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "00:55:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 1;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-04-01");
    strcpy(times[in].curnt_time, "11:00:43");
    strcpy(times[in].fetch_time, "10:59:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 1;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2026-01-01");
    strcpy(times[in].fetch_date, "2025-12-31");
    strcpy(times[in].curnt_time, "00:01:43");
    strcpy(times[in].fetch_time, "23:59:43");
    in++;
}
