#include "../../include/tokyobash.h"

#define MONTHS_IN_YR 12
#define HOURS_IN_DAY 24
#define MINS_IN_HOUR 60

typedef struct {
    char curnt_date[11];
    char fetch_date[11];
    char curnt_time[9];
    char fetch_time[9];
    bool expected;
    FetchOpts settings;
} Tester;

bool checkTimeDay(int, Tester*);
bool checkTimeHr(int, Tester*);
bool checkTimeMin(int, Tester*);
void charTimeToInt(IntTimesnDates*, char[], char[], char[], char[]);
int getDaysInMonth(int month);
void generateTests(Tester* times);

#define TEST_SIZE 51

int main() {

    Tester* times;
    if ((times = malloc(sizeof(Tester) * TEST_SIZE)) == NULL) {
        perror("malloc failed.");
        exit(1);
    }

    generateTests(times);

    bool result;
    int failed = 0;
    char buffer[16];
    char reset[] = "\e[00m";
    char green[] = "\e[32m";
    char red[] = "\e[31m";
    int return_true = 0;
    int return_false = 0;
    int expectedtrue = 0;
    int expectedfalse = 0;
    char failedTests[TEST_SIZE * 5];
    failedTests[0] = '\0';

    for (int i = 0; i < TEST_SIZE; i++) {

        printf("Test %i\nModifier: ", i + 1);

        switch (times[i].settings.modifier) {

            case Day:
                printf("Day");
                break;

            case Hour:
                printf("Hour");
                break;

            case Minute:
                printf("Minute");
                break;
        }

        printf("  Limit: %i\n", times[i].settings.limit);
        printf("Current Date: %s\n", times[i].curnt_date);
        printf("  Fetch Date: %s\n", times[i].fetch_date);
        printf("Current Time: %s\n", times[i].curnt_time);
        printf("  Fetch Time: %s\n\n", times[i].fetch_time);

        switch (times[i].settings.modifier) {

            case Day:
                result = checkTimeDay(times[i].settings.limit, &times[i]);
                break;

            case Hour:
                result = checkTimeHr(times[i].settings.limit, &times[i]);
                break;

            case Minute:
                result = checkTimeMin(times[i].settings.limit, &times[i]);
                break;
        }

        if (result) {

            printf("\nResult: True");
            return_true++;

            if (times[i].expected == false) {
                printf(" %sTEST FAILED%s", red, reset);
                sprintf(&buffer[0], "Test #%d ", i + 1);
                strcat(failedTests, buffer);
                failed++;
            }

        } else {

            printf("\nResult: False");
            return_false++;

            if (times[i].expected == true) {
                printf(" %sTEST FAILED%s", red, reset);
                sprintf(&buffer[0], "Test #%d ", i + 1);
                strcat(failedTests, buffer);
                failed++;
            }
        }

        printf("\nExpected: ");

        if (times[i].expected == true) {

            printf("True\n");
            expectedtrue++;

        } else {

            expectedfalse++;
            printf("False\n");
        }
        printf("------------------\n");
    }

    printf("%2d Tests Ran\n", TEST_SIZE);
    if (failed <= 0) {
        printf("%s", green);
    } else {
        printf("%s", red);
    }
    printf("%2d Tests Failed%s\n", failed, reset);
    printf("%2d Returned True  %2d Expected to Return True\n", return_true,
           expectedtrue);
    printf("%2d Retruned False %2d Expected to Return False\n", return_false,
           TEST_SIZE - expectedtrue);
    if (failed > 0) {
        printf("%sTests That Failed: %s%s\n", red, failedTests, reset);
    }
    free(times);
    return 0;
}
bool checkTimeDay(int limit, Tester* fetchConfig) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    IntTimesnDates time;
    charTimeToInt(&time, fetchConfig->curnt_date, fetchConfig->curnt_time,
                  fetchConfig->fetch_date, fetchConfig->fetch_time);

    yearDif = time.curnt_year - time.fetch_year;
    if (yearDif > 1) {
        return true;
    }
    //////////// REMOVE ///////////////
    printf(" yearDif = %i\n", yearDif);
    ///////////////////////////////////

    if (yearDif == 0) {
        monthDif = time.curnt_month - time.fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time.fetch_month) + time.curnt_month;
    }
    //////////// REMOVE ///////////////
    printf("monthDif = %i\n", monthDif);
    ///////////////////////////////////

    if (monthDif > 1) {

        if (time.curnt_month != 3 && time.fetch_month != 1) {
            return true;
        } else {
            days_in_month = getDaysInMonth(time.fetch_month);
            dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
            dayDif += 28; // add month of Febuary.

            if (dayDif > limit) {
                return true;
            }
        }
    }

    if (monthDif == 0) {
        dayDif = time.curnt_day - time.fetch_day;
    } else {
        days_in_month = getDaysInMonth(time.fetch_month);
        dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
    }
    //////////// REMOVE ///////////////
    printf("  dayDif = %i\n", dayDif);
    if (dayDif == 0) {
        hrDif = time.curnt_hour - time.fetch_hour;
    } else {
        hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
    }
    printf("   hrDif = %i\n", hrDif);
    if (hrDif == 0) {
        minDif = time.curnt_min - time.fetch_min;
    } else {
        minDif = (MINS_IN_HOUR - time.fetch_min) + time.curnt_min;
    }
    printf("  minDif = %i\n", minDif);
    ///////////////////////////////////

    if (dayDif > limit) {
        return true;
    } else if (dayDif < limit) {
        return false;
    }

    if (time.curnt_hour > time.fetch_hour) {
        return true;
    } else if (time.curnt_hour < time.fetch_hour) {
        return false;
    }

    if (time.curnt_min >= time.fetch_min) {
        return true;
    }
    return false;
}

bool checkTimeHr(int limit, Tester* fetchConfig) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    IntTimesnDates time;
    charTimeToInt(&time, fetchConfig->curnt_date, fetchConfig->curnt_time,
                  fetchConfig->fetch_date, fetchConfig->fetch_time);

    yearDif = time.curnt_year - time.fetch_year;
    //////////// REMOVE ///////////////
    printf(" yearDif = %i\n", yearDif);
    ///////////////////////////////////

    if (yearDif > 1) {
        return true;
    }

    if (yearDif == 0) {
        monthDif = time.curnt_month - time.fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time.fetch_month) + time.curnt_month;
    }
    //////////// REMOVE ///////////////
    printf("monthDif = %i\n", monthDif);
    ///////////////////////////////////

    if (monthDif > 1) {
        return true;
    }

    if (monthDif == 0) {
        dayDif = time.curnt_day - time.fetch_day;
    } else {
        days_in_month = getDaysInMonth(time.fetch_month);
        dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
    }
    //////////// REMOVE ///////////////
    printf("  dayDif = %i\n", dayDif);
    ///////////////////////////////////

    if (dayDif > 1) {
        return true;
    }

    if (dayDif == 0) {
        hrDif = time.curnt_hour - time.fetch_hour;
    } else {
        hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
    }
    //////////// REMOVE ///////////////
    printf("   hrDif = %i\n", hrDif);
    if (hrDif == 0) {
        minDif = time.curnt_min - time.fetch_min;
    } else {
        minDif = (MINS_IN_HOUR - time.fetch_min) + time.curnt_min;
    }
    printf("  minDif = %i\n", minDif);
    ///////////////////////////////////

    if (hrDif > limit) {
        return true;
    } else if (hrDif < limit) {
        return false;
    }

    if (time.curnt_min >= time.fetch_min) {
        return true;
    }
    return false;
}

bool checkTimeMin(int limit, Tester* fetchConfig) {

    int days_in_month = 0;
    int yearDif = 0;
    int monthDif = 0;
    int dayDif = 0;
    int hrDif = 0;
    int minDif = 0;

    IntTimesnDates time;
    charTimeToInt(&time, fetchConfig->curnt_date, fetchConfig->curnt_time,
                  fetchConfig->fetch_date, fetchConfig->fetch_time);

    yearDif = time.curnt_year - time.fetch_year;
    //////////// REMOVE ///////////////
    printf(" yearDif = %i\n", yearDif);
    ///////////////////////////////////

    if (yearDif > 1) {
        return true;
    }

    if (yearDif == 0) {
        monthDif = time.curnt_month - time.fetch_month;
    } else {
        monthDif = (MONTHS_IN_YR - time.fetch_month) + time.curnt_month;
    }
    //////////// REMOVE ///////////////
    printf("monthDif = %i\n", monthDif);
    ///////////////////////////////////

    if (monthDif > 1) {
        return true;
    }

    if (monthDif == 0) {
        dayDif = time.curnt_day - time.fetch_day;
    } else {
        days_in_month = getDaysInMonth(time.fetch_month);
        dayDif = (days_in_month - time.fetch_day) + time.curnt_day;
    }
    //////////// REMOVE ///////////////
    printf("  dayDif = %i\n", dayDif);
    ///////////////////////////////////

    if (dayDif > 1) {
        return true;
    }

    if (dayDif == 0) {
        hrDif = time.curnt_hour - time.fetch_hour;
    } else {
        hrDif = (HOURS_IN_DAY - time.fetch_hour) + time.curnt_hour;
    }
    //////////// REMOVE ///////////////
    printf("   hrDif = %i\n", hrDif);
    ///////////////////////////////////

    if (hrDif > 1) {
        return true;
    }

    if (hrDif == 0) {
        minDif = time.curnt_min - time.fetch_min;
    } else {
        minDif = (MINS_IN_HOUR - time.fetch_min) + time.curnt_min;
    }
    //////////// REMOVE ///////////////
    printf("  minDif = %i\n", minDif);
    ///////////////////////////////////

    if (minDif >= limit) {
        return true;
    }
    return false;
}
void charTimeToInt(IntTimesnDates* dateData, char curnt_date[],
                   char curnt_time[], char fetch_date[], char fetch_time[]) {

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

int getDaysInMonth(int month) {

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month != 2) {
        return 31;
    } else {
        return 28;
    }
}
// Be sure to update TEST_SIZE after adding or removing tests.
void generateTests(Tester* times) {
    int in = 0;
    ///////// Day Tests ///////////
    times[in].settings.modifier = Day;
    times[in].settings.limit = 5;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-08-05");
    strcpy(times[in].fetch_date, "2025-02-15");
    strcpy(times[in].curnt_time, "00:01:43");
    strcpy(times[in].fetch_time, "13:59:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 1;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-01-20");
    strcpy(times[in].fetch_date, "2025-01-19");
    strcpy(times[in].curnt_time, "03:30:43");
    strcpy(times[in].fetch_time, "13:30:43");
    in++;

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

    times[in].settings.modifier = Day;
    times[in].settings.limit = 31;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2026-01-31");
    strcpy(times[in].fetch_date, "2025-12-31");
    strcpy(times[in].curnt_time, "00:01:43");
    strcpy(times[in].fetch_time, "23:59:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 31;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2026-02-01");
    strcpy(times[in].fetch_date, "2025-12-31");
    strcpy(times[in].curnt_time, "00:00:43");
    strcpy(times[in].fetch_time, "23:59:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-13");
    strcpy(times[in].fetch_date, "2025-02-26");
    strcpy(times[in].curnt_time, "23:30:43");
    strcpy(times[in].fetch_time, "11:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 15;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-13");
    strcpy(times[in].fetch_date, "2025-02-26");
    strcpy(times[in].curnt_time, "11:50:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 3;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2026-04-04");
    strcpy(times[in].fetch_date, "2025-04-01");
    strcpy(times[in].curnt_time, "23:58:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Day;
    times[in].settings.limit = 1;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-04-01");
    strcpy(times[in].curnt_time, "23:51:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    ///////// Hour Tests ///////////
    times[in].settings.modifier = Hour;
    times[in].settings.limit = 1;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-04-01");
    strcpy(times[in].curnt_time, "12:51:43");
    strcpy(times[in].fetch_time, "12:50:43");
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

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 12;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "02:55:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 12;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-01");
    strcpy(times[in].fetch_date, "2025-02-28");
    strcpy(times[in].curnt_time, "11:55:43");
    strcpy(times[in].fetch_time, "23:50:43");
    in++;

    times[in].settings.modifier = Hour;
    times[in].settings.limit = 12;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-04-28");
    strcpy(times[in].fetch_date, "2025-04-28");
    strcpy(times[in].curnt_time, "15:49:43");
    strcpy(times[in].fetch_time, "03:50:43");
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
    strcpy(times[in].curnt_date, "2025-04-01");
    strcpy(times[in].fetch_date, "2025-04-01");
    strcpy(times[in].curnt_time, "10:25:43");
    strcpy(times[in].fetch_time, "10:24:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 1;
    times[in].expected = false;
    strcpy(times[in].curnt_date, "2025-03-31");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "23:58:43");
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

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2026-01-31");
    strcpy(times[in].fetch_date, "2025-12-31");
    strcpy(times[in].curnt_time, "00:01:43");
    strcpy(times[in].fetch_time, "23:59:43");
    in++;

    times[in].settings.modifier = Minute;
    times[in].settings.limit = 15;
    times[in].expected = true;
    strcpy(times[in].curnt_date, "2025-03-31");
    strcpy(times[in].fetch_date, "2025-03-31");
    strcpy(times[in].curnt_time, "10:37:43");
    strcpy(times[in].fetch_time, "10:22:43");
    in++;
}
