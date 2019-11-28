//
//  socket.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include "socket.h"
#include "util.h"


int hostname2ip(const char* hostname, char* ip)   //返回0 表示成功找到IP  返回1 表示失败
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



int dos_tcp_sock(char* host, int port)
{
    int sock;

    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
#ifdef DEBUG
    info("\n\nWaiting is %d\n\n", socket_wait);
#endif
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


bool dos_tcp_send_noalloc(int sock, char* data, char* buf, size_t bufsize)
{
    send(sock, data, strlen(data), 0);
    //memset(buf, '\0', bufsize);
    if (socket_wait) {
        if (recv(sock, buf, bufsize, 0) < 0) {
            warning("Failed to recieve data from tcp host");
            return false;
        }
    }
    return true;
}
