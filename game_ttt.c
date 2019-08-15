#include <stdlib.h>
#include "game.h"
#include "ttt.h"

/* define */

const int triplets[8][3] = {
	// check rows
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	// check columns
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	// check diagonals
	{0, 4, 8},
	{2, 4, 6}
};

/* private */

int _game_CheckTriplet (struct ttt *g, int i)
{
	return (
		g->board[triplets[i][0]] == g->board[triplets[i][1]] &&
		g->board[triplets[i][1]] == g->board[triplets[i][2]]
	);
}

enum ttt_tile _game_Winner (struct ttt *g)
{
	for (int i = 0; i < 8; i++) {
		if (_game_CheckTriplet(g, i))
			return g->board[triplets[i][0]];
	}
	// check draw
	for (int i = 0; i < BOARDLEN; i++) {
		if (g->board[i] == ttt_NOTILE)
			return ttt_NOTILE;
	}
	return ttt_DRAW;
}

enum ttt_tile _game_Now (struct ttt *g)
{
	return (enum ttt_tile)(g->turn % 2 + 1);
}

/* public */

int game_Join (void *game)
{
	struct ttt *g = (struct ttt *)game;
	if (g->player_count >= MAXPLAYERS)
		return -1;
	return (int)(g->player_count++ ? ttt_O : ttt_X);
}

int game_Turn (void *game)
{
	struct ttt *g = (struct ttt *)game;
	return g->turn;
}

int game_Update (void *game, int *n)
{
	struct ttt *g = (struct ttt *)game;

	if (n == NULL) {
		*n = (int)ttt_BADINPUT;
		return GAMEERROR;
	}

	int index = *n - 1;

	if (index < 0 || index >= BOARDLEN) {
		*n = (int)ttt_BADINPUT;
		return GAMEERROR;
	}

	if (g->board[index] != ttt_NOTILE) {
		*n = (int)ttt_PLAYED;
		return GAMEERROR;
	}

	g->board[index] = _game_Now(g);
	g->turn++;
	return GAMEOK;
}

int game_Result (void *game)
{
	struct ttt *g = (struct ttt *)game;
	return (int)_game_Winner(g);
}

void *game_New ()
{
	struct ttt *g = malloc(sizeof(struct ttt));
	for (int i = 0; i < BOARDLEN; i++) g->board[i] = ttt_NOTILE;
	g->turn = 0;
	g->player_count = 0;
	return g;
}

void game_Destroy (void *game)
{
	free(game);
}
