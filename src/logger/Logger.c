#include "Logger.h"

char *LogStr;

int initLogger()
{
    LogStr = (char*)malloc(MAX_LOG * sizeof(char));
    return LogStr != NULL;
}

void Log(char *str)
{
    sprintf(LogStr, "%s%s", getLogTime(), str);
    printf("%s\n", LogStr);
}
