#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include "listen.h"

int serverfd;
struct sockaddr_in addr;
socklen_t addrlen;
char buffer[BUFFERSIZE];

int error (char * msg) {
	int err = errno;
	perror(msg);
	return err;
}

int listen_Init ()
{
	int err;
	int opt = 1;
	int flags = SO_REUSEADDR;
	int backlog =  3;

	addrlen = sizeof(addr);
	memset(buffer, 0, BUFFERSIZE);

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0)
		return error("socket failed");

	err = setsockopt(serverfd, SOL_SOCKET, flags, &opt, sizeof(opt));
	if (err)
		return error("setsockopt failed");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	err = bind(serverfd, (struct sockaddr *)&addr, sizeof(addr));
	if (err)
		return error("bind failed");

	err = listen(serverfd, backlog);
	if (err)
		return error("listen failed");

	return 0;
}

int listen_Wait ()
{
	int c = accept(serverfd, (struct sockaddr *)&addr, &addrlen);
	if (c < 0)
		return error("accept failed");
	return c;
}

int listen_Read (int *fd)
{
	if (*fd < 0)
		return 0;
	int n = 0;
	int read = recv(*fd, &n, sizeof(int), 0);
#if DEBUG
	printf("\tread (%d) from client=%d\n", n, *fd);
#endif
	if (read <= 0) {
		*fd = -1;
		return 0;
	}
	return n;
}

int listen_SendMsg (int *fd, char *str)
{
	if (*fd < 0)
		return -1;
#if DEBUG
	printf("\tsending '%s' to client=%d\n", str, *fd);
#endif
	int n = send(*fd, str, MSGSIZE, MSG_NOSIGNAL);
	if (n <= 0)
		*fd = -1;
	return n;
}

int listen_SendInt (int *fd, int n)
{
	if (*fd < 0)
		return -1;
#if DEBUG
	printf("\tsending (%d) to client=%d\n", n, *fd);
#endif
	int sent = send(*fd, &n, sizeof(int), MSG_NOSIGNAL);
	if (sent <= 0)
		*fd = 1;
	return sent;
}
