#include <stdio.h>
#include <string.h>
#include <time.h>

void get_timestamp(char* string){
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(string,100,"%y-%m-%d %H:%M:%S",timeinfo);
}