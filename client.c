#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connect.h"
#include "game.h"
#include "renderer.h"

#define HOST "127.0.0.1"
#define PORT 8080
#define GAMELENGTH 9
#define INPUT_QUIT -1
#define INPUT_INVALID -2

#define USAGE ("./client.out [HOST] [PORT]\n")

int parseOpts (int argc, char const *argv[], char *hostPtr, int *portPtr)
{
	if (argc > 1)
		sscanf(argv[1], "%s", hostPtr);
	if (argc > 2)
		sscanf(argv[2], "%d", portPtr);
	return 0;
}

int main (int argc, char const *argv[])
{
	struct connect conn;
	char buffer[BUFFERSIZE];
	int port = PORT;
	char host[BUFFERSIZE] = HOST;
	int playerId = -1;
	void *g = game_New();

	parseOpts(argc, argv, host, &port);
	connect_Init(&conn, host, port);

	while (1) {
		if (!connect_RecvMsg(&conn, buffer)) {
			fprintf(stderr, "\tnothing received. \n");
			break;
		}

		if (!strcmp(buffer, MSGINPUT)) {
			int input = renderer_Input();
			connect_Send(&conn, input);
			continue;
		}

		if (!strcmp(buffer, MSGJOIN)) {
			renderer_OnMsg(MSGJOIN);
			connect_RecvInt(&conn, &playerId);
			if (playerId < 0) {
				renderer_OnMsg(MSGJOINERROR);
				break;
			}
#if DEBUG
			fprintf(stderr, "\tjoined with id=%d.\n", playerId);
#endif
			continue;
		}

		if (!strcmp(buffer, MSGERROR)) {
			int n = 0;
			connect_RecvInt(&conn, &n);
			renderer_OnError(g, n);
		}

		if (!strcmp(buffer, MSGEVENT)) {
			int n = 0;
			connect_RecvInt(&conn, &n);
			renderer_OnResult(g, playerId, n);
			break;
		}

		if (!strcmp(buffer, MSGUPDATE)) {
			int n = 0;
			connect_RecvInt(&conn, &n);
			game_Update(g, &n);
			renderer_Render(g);
			continue;
		}

		if (!strcmp(buffer, MSGSTART)) {
			renderer_OnMsg(MSGSTART);
			renderer_Render(g);
			continue;
		}

		if (!strcmp(buffer, MSGWAIT)) {
			renderer_OnMsg(MSGWAIT);
			continue;
		}
	}

	game_Destroy(g);
	return 0;
}
