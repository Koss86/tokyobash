
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

    char cur_date[] = "2025-03-15";
    char fetch_date[] = "2025-03-15";

    char cur_time[] = "00:30:43";
    char fetch_time[] = "00:15:22";


    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) return true; // Year
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) return true; // Month
 
    char fbuf[3];
    char cbuf[3];
    int i_fbuf;
    int i_cbuf;
    int diff = 0;

    Fetchtimer state = fetchSettings->state;

    // thinking of wraping these into one statement, and check state if should return and when?
    if (state == Hour) {

        // Check if longer than 1 day since update.
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

            int dayDif;
            dayDif = i_cbuf - i_fbuf;

            // Dont return if dayDif == 1 because
            // if updates are around midnight, it 
            // could think it's been a day when its been 10mins.
            if (dayDif > 1) {
                return true;
            }
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

            // count separation of hours on a 24hr clock.
            for (int i = i_fbuf; i != i_cbuf;) {
                diff++;
                i++;
                if (i == 24) {
                    i = 0;
                }
            }

            if (diff >= fetchSettings->amount) {
                if (diff == 1) { // Check minutes if only an hour apart to make sure its actually an hour apart.
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

                    // This ensures it's been 60mins or longer since last update,
                    // and that the date just didnt change making it look like a day since update.
                    if (((60 - i_fbuf) + i_cbuf) >= 60) {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }

    } else if (fetchSettings->state == Minute) {

        if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) return true; // Day

        if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) return true; // Hour

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
            if (i_cbuf > i_fbuf) {
                diff = i_cbuf - i_fbuf;
            } else {
                diff = i_fbuf - i_cbuf;
            }

            if (diff >= fetchSettings->amount) {
                return true;
            }
        }
    }

    return false;
}



