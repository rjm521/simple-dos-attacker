//  util.c


#include "util.h"
#include "message.h"

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>

static uint32_t Q[4096], c = 362436;

void init_rand(uint32_t x)//cwmc
{
    int i;
    Q[0] = x;
    Q[1] = x + PHI;
    Q[2] = x + PHI + PHI;
    for (i = 3; i < 4096; i++)
    {
        Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
    }
}

int randport()
{
    return rand() % 65534 + 1;
}

char randchar()
{
    return rand() % 126 + 1;
}

char* randstring(size_t sz)
{
    char* s = (char*)malloc(sz * sizeof(char));
    for (int i = 0; i < sz; i++) {
        s[i] = randchar();
    }
    return s;
}

int randrange(int start, int end)
{
    assert(start <= end);
    return rand() % (start + 1 - end) + start;
}

const char* getarg(const char arg[2], const char* argv[], int argc)
{

    for (int i = 0; i < argc; i++) {

        if (!strcmp(arg, argv[i])) {

            if (i < argc-1) {
                return argv[i + 1];
            }else{
                die("Argument required for key:%s",arg);
            }
        }
    }
    return NULL;
}

const char* getlarg(const char* arg, const char* argv[], int argc)
{

    for (int i = 0; i < argc; i++) {

        if (!strcmp(arg, argv[i])) {

            if (i < argc-1) {
                return argv[i + 1];
            }else{
                die("Argument required for key:%s",arg);
            }
        }
    }
    return NULL;
}

bool checkarg(const char arg[2], const char* argv[], int argc)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], arg)) {
            return true;
        }
    }
    return false;
}

bool checklarg(const char* arg, const char* argv[], int argc)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], arg)) {
            return true;
        }
    }
    return false;
}



void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}


char* dtoa(double x){//double to char*
    char *s=(char*)malloc(sizeof(x)+1);
    sprintf(s,"%0.1f",x);
    return s;
}

char* bytes2mb(int64_t bcount){
    return strcat(dtoa(bcount/pow(1024.0,2)), "Mb");
}

char* bytes2any(int64_t bcount,uint8_t type){
    return dtoa(bcount/pow(1024.0,2));
}

char* metrics2str(uint8_t type){
    if(type==SIZE_BYTES){
        return "B";
    }else if(type==SIZE_KB){
        return "Kb";
    }else if(type==SIZE_MB){
        return "Mb";
    }else if(type==SIZE_GB){
        return "Gb";
    }else{
        return "Tb";
    }
}

uint8_t str2metrics(char* metrics){
    if(strcmp(metrics, "b")==0){
        return SIZE_BYTES;
    }else if(strcmp(metrics, "Kb")==0){
        return SIZE_KB;
    }else if(strcmp(metrics, "Mb")==0){
        return SIZE_MB;
    }else if(strcmp(metrics, "Gb")==0){
        return SIZE_GB;
    }else if(strcmp(metrics, "Tb")==0){
        return SIZE_TB;
    }else{
        return -1;
    }
}

const char* sgetarg(const char arg[2], const char* argv[], int argc,const char* _default){
    if(checkarg(arg, argv, argc)){
        return getarg(arg, argv, argc);
    }else{
        return _default;
    }
}

const char* sgetlarg(const char *arg, const char* argv[], int argc,const char* _default){
    if(checklarg(arg, argv, argc)){
        return getlarg(arg, argv, argc);
    }else{
        return _default;
    }
}
