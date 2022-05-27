#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void get_timestamp(char* string){
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(string,100,"%y-%m-%d %H:%M:%S",timeinfo);
}

void rotate_log_check(void){
    FILE * fp = fopen("/tmp/my_services_log","r");
    fseek(fp,0L,SEEK_END);
    long int size = ftell(fp);
    fclose(fp);
    if(size>1000000) system("/tmp/rotate_log.sh");
}

int usuario_duplicado(char* name,int n_names,char** names){
    for(int i=0;i<n_names;i++){
        if(strcmp(name,names[i])==0) return 1;
    }
    return 0;
}
