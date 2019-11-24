//
//  socket.h
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#ifndef socket_h
#define socket_h

#include "message.h"
#include "packet.h"

#include <arpa/inet.h>
#include <errno.h> //For errno - the error number
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

extern bool socket_wait;

bool dos_tcp_send_noalloc(int sock, char* data, char* buf, size_t bufsize);
int dos_tcp_sock(char* host, int port);
int hostname2ip(const char* hostname, char* ip);
#endif /* socket_h */
