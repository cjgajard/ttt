#ifndef CONNECT_H
#define CONNECT_H
#include "msg.h"

#define BUFFERSIZE 512
struct connect {
    int fd;
};

int connect_Init(struct connect *c, char *h, int p);
int connect_Send(struct connect *c, int n);
int connect_RecvMsg(struct connect *c, char *msg);
int connect_RecvInt(struct connect *c, int *n);
#endif
