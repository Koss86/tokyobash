#include <stdio.h>
#include <stdlib.h>
#include "../lib/tokyobash.h"

bool shouldFetch(FetchOpts *fetchSettings);

int main() {

    FetchOpts settings;
    settings.state = Minute;
    settings.amount = 14;
    if (shouldFetch(&settings)) {
        printf("True\n");
    }else{
        printf("False\n");
    }
}

bool shouldFetch(FetchOpts *fetchSettings) {

    char cur_date[] = "2025-03-16";
    char fetch_date[] = "2025-03-15";

    char cur_time[] = "04:55:43";
    char fetch_time[] = "23:55:22";

    char fbuf[3];
    char cbuf[3];
    int i_fbuf;
    int i_cbuf;
    int dayDif;
    int hrDif;
    int minDif;

    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) return true; // Year
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) return true; // Month


    if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) {  // Day


        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_date[8+i];
            cbuf[i] = cur_date[8+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        dayDif = i_cbuf - i_fbuf;

        if (fetchSettings->state == Day) {

            if (dayDif >= fetchSettings->amount) {
                // If dif is same as what user set, we need
                // to check for false positives.
                if (dayDif == fetchSettings->amount) {

                    for (int i = 0; i < 3; i++) {
                        if (i == 2) {
                            fbuf[i] = '\0';
                            cbuf[i] = '\0';
                            break;
                        }
                        fbuf[i] = fetch_time[0+i];
                        cbuf[i] = cur_time[0+i];
                    }
                    i_fbuf = atoi(fbuf);
                    i_cbuf = atoi(cbuf);

                    hrDif = (24 - i_fbuf) + i_cbuf;

                    if (hrDif >= 24) {

                        for (int i = 0; i < 3; i++) {
                            if (i == 2) {
                                fbuf[i] = '\0';
                                cbuf[i] = '\0';
                                break;
                            }
                            fbuf[i] = fetch_time[3+i];
                            cbuf[i] = cur_time[3+i];
                        }
                        i_fbuf = atoi(fbuf);
                        i_cbuf = atoi(cbuf); 

                        minDif = (60 - i_fbuf) + i_cbuf;

                        if (minDif >= 60) {
                            return true;
                        } else { return false; }

                    } else { return false; }

                } else { return true; }

            } else { return false; }

        } else if (dayDif > 1) { return true; }
    }


    if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) { // Hour

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[0+i];
            cbuf[i] = cur_time[0+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        hrDif = (24 - i_fbuf) + i_cbuf;

        if (fetchSettings->state == Hour) {

            if (hrDif >= fetchSettings->amount) {

                if (hrDif == fetchSettings->amount) {
                    for (int i = 0; i < 3; i++) {
                        if (i == 2) {
                            fbuf[i] = '\0';
                            cbuf[i] = '\0';
                            break;
                        }
                        fbuf[i] = fetch_time[3+i];
                        cbuf[i] = cur_time[3+i];
                    }
                    i_fbuf = atoi(fbuf);
                    i_cbuf = atoi(cbuf); 

                    minDif = (60 - i_fbuf) + i_cbuf;
                    if (minDif >= 60) {
                        return true;
                    } else { return false; }

                } else { return true; }

            } else { return false; }

        } else if (hrDif > 1) { return true; }
    }

    if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) {  // Minute

        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = fetch_time[3+i];
            cbuf[i] = cur_time[3+i];
        }
        i_fbuf = atoi(fbuf);
        i_cbuf = atoi(cbuf);

        if (((60 - i_fbuf) + i_cbuf) >= fetchSettings->amount) {
            return true;
        }
    }

    return false;
}



