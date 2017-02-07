#include "NBSTimer.h"

char *finalS = NULL;

char* getReadableTime()
{
    // Time
    time_t rt;
    struct tm *timeInfo;

    time(&rt);
    timeInfo = localtime(&rt);

    return asctime(timeInfo);
}

int initTimer()
{
    finalS = (char*)malloc(9 * sizeof(char));
    return finalS != NULL;
}

char* getLogTime()
{
    int i = 0;
    char h[3], m[3], s[3];
    char *str = getReadableTime();

    // Extract hour, minute and second.
    sprintf(h, "%c%c", str[11], str[12]);
    sprintf(m, "%c%c", str[14], str[15]);
    sprintf(s, "%c%c", str[17], str[18]);

    sprintf(finalS, "[%s.%s.%s]: ", h, m, s);

    return finalS;
}

double getNanoTime()
{
    LARGE_INTEGER Time, Frequency;

    QueryPerformanceCounter(&Time);
    QueryPerformanceFrequency(&Frequency);

    return (double)(Time.QuadPart)/(double)(Frequency.QuadPart);
}

/* wait for n (nanosecs) */
void nanoSleep(float n)
{
	clock_t until;
	until = clock()+(n/((float)CLOCKS_PER_SEC));
	while(clock() < until){ /* wait */ }
}

void closeTimer()
{
    GlobalFree((HANDLE)finalS);
    printf("Timer Clean Up Done.\n");
}
