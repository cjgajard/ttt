#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "listen.h"
#include "game.h"

#define ROOMSIZE 2
int client[ROOMSIZE];

bool _client_Missing ()
{
	for (int i = 0; i < ROOMSIZE; i++)
		if (client[i] < 0)
			return true;
	return false;
}

int main (int argc, char const *argv[])
{
	void *game = game_New();

	int err = listen_Init();
	if (err) exit(err);
	printf("Server listening...\n");

	for (int i = 0; i < ROOMSIZE; i++) {
		client[i] = -1;
		printf("Waiting player %d...\n", i + 1);
		client[i] = listen_Wait();
		int playerId = game_Join(game);
		listen_SendMsg(client + i, MSGJOIN);
		listen_SendInt(client + i, playerId);
	}

	for (int i = 0; i < ROOMSIZE; i++)
		listen_SendMsg(client + i, MSGSTART);

	while (!_client_Missing()) {
		int now = game_Turn(game) % ROOMSIZE;

		for (int i = 0; i < ROOMSIZE; i++)
			if (i != now)
				listen_SendMsg(client + i, MSGWAIT);
#if DEBUG
		printf("Waiting message from player %d...\n", now + 1);
#endif
		listen_SendMsg(client + now, MSGINPUT);
		int input = listen_Read(client + now);

		int err = game_Update(game, &input);

		if (err) {
			listen_SendMsg(client + now, MSGERROR);
			listen_SendInt(client + now, input);
		}

		int event = game_Result(game);

		for (int i = 0; i < ROOMSIZE; i++) {
			listen_SendMsg(client + i, MSGUPDATE);
			listen_SendInt(client + i, input);

			if (!event)
				continue;
			listen_SendMsg(client + i, MSGEVENT);
			listen_SendInt(client + i, event);
		}
	}

	return 0;
}
