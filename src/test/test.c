#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    char cdate[11];
    char ctime[9];
    time_t now = time(0);
    struct tm *time_struct = localtime(&now); 

    if (!strftime(cdate, sizeof(cdate), "%Y-%m-%d", time_struct))
    {
        printf("\\nError. shouldFetch()\\n");
    }

    if (!strftime(ctime, sizeof(ctime), "%X", time_struct))
    {
        printf("\\nError. shouldFetch()\\n");
    }
    FILE *fetch_status = popen("stat .git/FETCH_HEAD 2>/dev/null", "r");
    if (fetch_status == NULL) {
        printf("\\nError. shouldFetch()\\n");
    }

    /*
    char fbuf[800];
    if (fgets(fbuf, sizeof(fbuf), fetch_status) == NULL) {
        pclose(fetch_status);
        printf("\\nError. shouldFetch()\\n");
    }
    */

    char c;
    int indx = 0;
    int nl = 0;
    int space = 0;
    int inDate = 0;
    int inTime = 0;
    char fdate[11];
    char ftime[9];

    while ((c = fgetc(fetch_status)) != EOF) {

        if (c == '\n') {
            nl++;
            continue;
        }

        if (nl == 6 && c == ' ') {
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
            fdate[indx++] = c;
            if (indx == 10) {
                fdate[indx] = '\0';
                indx = 0;
            }
        }

        if (inTime) {
            ftime[indx++] = c;
            if (indx == 8) {
                ftime[indx] = '\0';
                break;
            }
        }
    }
 // Check how long since fetch was called
    if (fdate[2] != cdate[2] || fdate[3] != cdate[3]) return true; // Year
    if (fdate[5] != cdate[5] || fdate[6] != cdate[6]) return true; // Month
    if (fdate[8] != cdate[8] || fdate[9] != cdate[9]) return true; // Day

    //if (ftime[0] != ctime[0] || ftime[1] != ctime[1]) return true; // Hour
    if (ftime[3] != ctime[3] || ftime[4] != ctime[4]) {            // Minute

        char fbuf[3];
        char cbuf[3];
        for (int i = 0; i < 3; i++) {
            if (i == 2) {
                fbuf[i] = '\0';
                cbuf[i] = '\0';
                break;
            }
            fbuf[i] = ftime[3+i];
            cbuf[i] = ctime[3+i];
        }
        int i_fbuf = atoi(fbuf);
        int i_cbuf = atoi(cbuf);
        printf("%i %i\n", i_cbuf, i_fbuf);
    }

    printf("Last Fetch:%s %s\nCurrent Date:%s %s", fdate, ftime, cdate, ctime);
    return 0;
}
