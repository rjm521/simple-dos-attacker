//  ddos.h


#ifndef dos_h
#define dos_h

#include "message.h"
#include "socket.h"
#include "util.h"

#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MODE_EMPTY 0 //not initiallized mode
#define MODE_TCP 1
#define MODE_MEMCRASHED 2

/*extern是计算机语言中的一个关键字，可置于变量或者函数前，以表示变量或者函数的定义在别的文件中。*/
extern double psent;

//数据包度量单位
extern uint8_t metrics;

//是否展示攻击状态数据
extern bool status;//is status shown?
// 当前线程数
extern int tcount;

//攻击目标IP
extern char* __host;

//目标端口
extern int __port;

//线程互斥信号量
extern pthread_mutex_t mutex;

extern bool __run;

//结构体的作用将参数打包 因为创建多线程要用到

typedef struct {
    char* packet;
    char* host;
    int port;
    uint8_t mode;
} _dos_param;

void dos(char* host, int port, char* packet, int _tcount, int mode);

#endif /* dos_h */
