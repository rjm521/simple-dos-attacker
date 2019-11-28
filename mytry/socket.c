//  socket.c

#include "socket.h"
#include "util.h"

/* 将域名转换为IP
*  参数：hostname:域名 ip:转换后的IP地址（点十进制）
*  返回 0 表示成功找到IP  
*  返回 1 表示失败
*/
int hostname2ip(const char* hostname, char* ip)   
{
    struct hostent* he;
    struct in_addr** addr_list;
    int i;

    if ((he = gethostbyname(hostname)) == NULL) {
        // get the host info
        error("Failed to find host %s", hostname);
        return 1;
    }

    addr_list = (struct in_addr**)he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip, inet_ntoa(*addr_list[i]));
        return 0;
    }

    return 1;
}


/* 创建套接字并建立TCP链接
*  参数：host:IP地址 port: 端口
*  返回 非负整数 成功建立TCP连接  
*  返回 -1 表示 创建套接字失败
*/

int dos_tcp_sock(char* host, int port)
{
    int sock;

    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        error("Could not create socket.Is everything ok with your system?");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        error("Could not connect to target.");
        shutdown(sock,2);
        return -2;
    }
    return sock;
}


/* 向攻击目标发送数据
*  参数：sock:创建好的网络链接的标识 data:发送的字符串数据流 bufsize:接受数据缓冲区大小
*  返回 true 表示已经发送
*  返回 false 表示没有接受到目标主机的应答数据
*/
bool dos_tcp_send_noalloc(int sock, char* data, char* buf, size_t bufsize)
{
    send(sock, data, strlen(data), 0);

    if (socket_wait) {
        if (recv(sock, buf, bufsize, 0) < 0) {
            warning("Failed to recieve data from tcp host");
            return false;
        }
    }
    return true;
}
