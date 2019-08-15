#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "connect.h"

int connect_Init (struct connect *c, char *h, int p) {
	int fd = 0;
	struct sockaddr_in serv_addr;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Socket creation error.\n");
		exit(EXIT_FAILURE);
	}
	c->fd = fd;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(p);

	if (inet_pton(AF_INET, h, &serv_addr.sin_addr) <= 0) {
		fprintf(stderr, "Invalid address | Address not supported.\n");
		exit(EXIT_FAILURE);
	}

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Connection Failed.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int connect_Send (struct connect *c, int n)
{
#if DEBUG
	printf("\tsending (%d)\n", n);
#endif
	int i = send(c->fd, &n, sizeof(int), 0);
#if DEBUG
	if (i == 0)
		fprintf(stderr, "\tnothing sent\n");
#endif
	return i;
}

int connect_RecvMsg (struct connect *c, char *buf) {
	memset(buf, 0, MSGSIZE);
	int read = recv(c->fd, buf, MSGSIZE, 0);
#if DEBUG
	printf("\treceived msg '%s'.\n", buf);
#endif
	return read;
}

int connect_RecvInt (struct connect *c, int *n) {
	int read = recv(c->fd, n, sizeof(int), 0);
#if DEBUG
	printf("\treceived int (%d).\n", *n);
#endif
	return read;
}
