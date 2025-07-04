
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "src/lib/tokyobash.h"
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

    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    char cur_date[11];
    char cur_time[9];

    if (!strftime(cur_date, sizeof(cur_date), "%Y-%m-%d", time_struct))
    {
        return false;
    }
    if (!strftime(cur_time, sizeof(cur_time), "%X", time_struct))
    {
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

    if (fetch_date[2] != cur_date[2] || fetch_date[3] != cur_date[3]) return true; // Year
    if (fetch_date[5] != cur_date[5] || fetch_date[6] != cur_date[6]) return true; // Month

    if (fetchSettings->state == Day) {

        if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) {

            char fbuf[3];
            char cbuf[3];

            for (int i = 0; i < 3; i++) {
                if (i == 2) {
                    fbuf[i] = '\0';
                    cbuf[i] = '\0';
                    break;
                }
                fbuf[i] = fetch_date[8+i];
                cbuf[i] = cur_date[8+i];
            }

            int i_fbuf = atoi(fbuf);
            int i_cbuf = atoi(cbuf);
            int diff;
            if (i_cbuf > i_fbuf) {
                diff = i_cbuf - i_fbuf;
            } else {
                diff = i_fbuf - i_cbuf;
            }

            if (diff > fetchSettings->amount) {
                return true;
            }
        }
        return false;
    }

    if (fetch_date[8] != cur_date[8] || fetch_date[9] != cur_date[9]) return true; // Day

    if (fetchSettings->state == Hour) {

        if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) {

            char fbuf[3];
            char cbuf[3];

            for (int i = 0; i < 3; i++) {
                if (i == 2) {
                    fbuf[i] = '\0';
                    cbuf[i] = '\0';
                    break;
                }
                fbuf[i] = fetch_time[0+i];
                cbuf[i] = cur_time[0+i];
            }

            int i_fbuf = atoi(fbuf);
            int i_cbuf = atoi(cbuf);
            int diff;
            if (i_cbuf > i_fbuf) {
                diff = i_cbuf - i_fbuf;
            } else {
                diff = i_fbuf - i_cbuf;
            }

            if (diff > fetchSettings->amount) {
                return true;
            }

        }
        return false;
    }

    if (fetch_time[0] != cur_time[0] || fetch_time[1] != cur_time[1]) return true; // Hour

    if (fetchSettings->state == Minute) {

        if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) {

            char fbuf[3];
            char cbuf[3];

            for (int i = 0; i < 3; i++) {
                if (i == 2) {
                    fbuf[i] = '\0';
                    cbuf[i] = '\0';
                    break;
                }
                fbuf[i] = fetch_time[3+i];
                cbuf[i] = cur_time[3+i];
            }

            int i_fbuf = atoi(fbuf);
            int i_cbuf = atoi(cbuf);
            int diff;
            if (i_cbuf > i_fbuf) {
                diff = i_cbuf - i_fbuf;
            } else {
                diff = i_fbuf - i_cbuf;
            }

            if (diff > fetchSettings->amount) {
                return true;
            }

        }
        return false;
    }
    if (fetch_time[3] != cur_time[3] || fetch_time[4] != cur_time[4]) return true; // Minute

    return false;
}
// Checks when last time the repo was updated,
// calls fetch if longer than user specified. Then,
// returns number of commits if any from remote.
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
