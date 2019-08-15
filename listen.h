#ifndef LISTEN_H
#define LISTEN_H
#include "msg.h"
#define PORT 8080
#define BUFFERSIZE 512

int listen_Init ();
int listen_Read (int *fd);
int listen_SendMsg (int *fd, char *str);
int listen_SendInt (int *fd, int n);
int listen_Wait ();
#endif
