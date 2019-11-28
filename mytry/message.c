//  message.c

#include "message.h"
#include "dos.h"

#include <errno.h>

bool hide_warnings;
bool hide_errors;

//接受任意多个参数，第一个参数必须为char *类型，后面有多少个参数都可以，没有也可以
void info(const char* format, ...)
{
    PRINT_FORMATTED(INFO)
}
void error(const char* format, ...)
{
    if(!hide_errors){
        PRINT_FORMATTED(ERROR)
    }
}
void warning(const char* format, ...)
{
    if (!hide_warnings) {
        PRINT_FORMATTED(WARN)
    }
}
void success(const char* format, ...)
{
    PRINT_FORMATTED(SUCCESS)
}
void success_n(const char* format, ...)
{
    PRINT_FORMATTED_NO_NEWLINE(SUCCESS)
}
void die(const char* format, ...)
{
    PRINT_FORMATTED(ERROR)
    error("Fatal error");
    error("Quiting...");
    exit(-1);
}
void dperror(const char* format,...){
    PRINT_FORMATTED_NO_NEWLINE(ERROR)
    error(":%s(%d)",strerror(errno),errno);
}
