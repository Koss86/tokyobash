#include <stdio.h>
#include <time.h>

int main() {

    char buffer[255];
    time_t now = time(0);
    struct tm *mytime = localtime(&now); 
    if ( strftime(buffer, sizeof buffer, "%Y-%m-%d %X", mytime) )
    {
        printf("time1 = %s\n", buffer);
    }
    return 0;
}
