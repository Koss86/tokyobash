#include <stdio.h>
#include <stdlib.h>

#include "../lib/tokyobash.h"

bool shouldFetch(FetchOpts* fetchSettings);

int main() {
    FetchOpts settings;
    settings.state = Hour;
    settings.amount = 12;
    if (shouldFetch(&settings)) {
        printf("True\n");
    } else {
        printf("False\n");
    }
}

bool shouldFetch(FetchOpts* fetchSettings) {

    char cur_date[] = "2025-03-15";
    char fetch_date[] = "2025-03-14";

    char cur_time[] = "11:55:43";
    char fetch_time[] = "23:55:22";

    char fbuf[3];
    char cbuf[3];
    int i_fbuf;
    int i_cbuf;
    int dayDif;
    int hrDif;
    int minDif;

    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) { // Year
        return true;
    }
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) { // Month
        return true;
    }

    if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) { // Day

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_date[8 + i];
            cbuf[i] = cur_date[8 + i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        dayDif = i_cbuf - i_fbuf;
        printf("dayDif: %i\n", dayDif);

        if (fetchSettings->state == Day && dayDif > fetchSettings->amount ||
            fetchSettings->state != Day && dayDif > 1) {

            return true;

        } else if (fetchSettings->state == Day && dayDif < fetchSettings->amount) {

            return false;

        } else if (fetchSettings->state == Day) {

            for (int i = 0; i < 3; i++) {
                if (i == 2) {
                    fbuf[i] = '\0';
                    cbuf[i] = '\0';
                    break;
                }
                fbuf[i] = fetch_time[0 + i];
                cbuf[i] = cur_time[0 + i];
            }
            i_fbuf = atoi(fbuf);
            i_cbuf = atoi(cbuf);

            hrDif = (24 - i_fbuf) + i_cbuf;
            printf("hrDif: %i\n", hrDif);

            if (hrDif > 24) {
                return true;
            } else if (hrDif < 24) {
                return false;
            } else {

                for (int i = 0; i < 3; i++) {
                    if (i == 2) {
                        fbuf[i] = '\0';
                        cbuf[i] = '\0';
                        break;
                    }
                    fbuf[i] = fetch_time[3 + i];
                    cbuf[i] = cur_time[3 + i];
                }
                i_fbuf = atoi(fbuf);
                i_cbuf = atoi(cbuf);

                minDif = i_cbuf - i_fbuf;
                printf("minDif: %i\n", minDif);

                if (i_cbuf >= i_fbuf) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) { // Hour

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[0 + i];
            cbuf[i] = cur_time[0 + i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        if (i_cbuf > i_fbuf) {
            hrDif = i_cbuf - i_fbuf;
        } else {
            hrDif = (24 - i_fbuf) + i_cbuf;
        }

        printf("hrDif: %i\n", hrDif);

        if (fetchSettings->state == Hour && hrDif > fetchSettings->amount ||
            fetchSettings->state != Hour && hrDif > 1) {

            return true;

        } else if (fetchSettings->state == Hour && hrDif < fetchSettings->amount) {

            return false;

        } else if (fetchSettings->state == Hour) {

            for (int i = 0; i < 3; i++) {
                if (i == 2) {
                    fbuf[i] = '\0';
                    cbuf[i] = '\0';
                    break;
                }
                fbuf[i] = fetch_time[3 + i];
                cbuf[i] = cur_time[3 + i];
            }
            i_fbuf = atoi(fbuf);
            i_cbuf = atoi(cbuf);

            minDif = (60 - i_fbuf) + i_cbuf;
            printf("minDif: %i\n", minDif);

            if (i_cbuf >= i_fbuf) {
                return true;
            } else {
                return false;
            }
        }
    }

    if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) { // Minute

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[3 + i];
            cbuf[i] = cur_time[3 + i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        if (i_cbuf > i_fbuf) {
            minDif = i_cbuf - i_fbuf;
        } else {
            minDif = (60 - i_fbuf) + i_cbuf;
        }

        printf("minDif: %i\n", minDif);

        if (fetchSettings->state == Minute) {
            if (minDif >= fetchSettings->amount) {
                return true;
            }
        }
    }
    return false;
}
