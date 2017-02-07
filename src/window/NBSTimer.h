#include<stdlib.h>
#include<stdint.h>
#include<stdarg.h>
#include<windef.h>
#include<winbase.h>
#include<winnt.h>
#include<time.h>

#define NANO_SECOND 1000000000.0

int initTimer();
char* getReadableTime();
char* getLogTime();
double getNanoTime();
void nanoSleep(float);
void closeTimer();
