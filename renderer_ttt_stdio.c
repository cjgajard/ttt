#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "renderer.h"
#include "ttt.h"

#define INPUTSIZE 1024

int _readline (char *buf)
{
	memset(buf, 0, INPUTSIZE);
	fgets(buf, INPUTSIZE, stdin);
	buf[strcspn(buf, "\r\n")] = 0;
	return strlen(buf);
}

void _print_Tile (struct ttt *g, int i)
{
	switch (g->board[i]) {
	case ttt_O:
		printf("\033[0;31mO\033[0m");
		break;
	case ttt_X:
		printf("\033[0;34mX\033[0m");
		break;
	default:
		printf("%c", '1' + i);
			break;
	}
}

void _print_Row (struct ttt *g, int start)
{
	printf(" ");
	_print_Tile(g, start);
	printf(" | ");
	_print_Tile(g, start + 1);
	printf(" | ");
	_print_Tile(g, start + 2);
	printf(" \n");
}

int renderer_Input ()
{
	char buf[INPUTSIZE];
	while (1) {
		printf("Make your play [1-9]: ");
		_readline(buf);
		int i = atoi(buf);
		if (i > 0 && i <= BOARDLEN) return i;
		printf("Invalid input. Try again.\n");
	}
}

void renderer_Render (void *game)
{
	struct ttt *g = game;
	printf("\n");
	_print_Row(g, 0);
	printf("---+---+---\n");
	_print_Row(g, 3);
	printf("---+---+---\n");
	_print_Row(g, 6);
}

void renderer_OnError (void *game, int error)
{
	switch (error) {
	case ttt_PLAYED:
		printf("That tile was already played. ");
		break;
	case ttt_BADINPUT:
		printf("That move was not valid. ");
		break;
	default:
		printf("There was an error. ");
		break;
	}
	printf("Try again.\n");
}

void renderer_OnMsg (char *msg)
{
	if (!strcmp(msg, MSGJOIN))
		printf("Connected to server!\n");
	else if (!strcmp(msg, MSGJOINERROR))
		printf("Game unavailable.\n");
	else if (!strcmp(msg, MSGSTART))
		printf("Game start!\n");
	else if (!strcmp(msg, MSGWAIT))
		printf("Waiting for your oponent...\n");
}

void renderer_OnResult (void *game, int id, int result)
{
	if (result == ttt_DRAW) {
		printf("Game ended in a DRAW.\n");
	}
	else if (result == id) {
		printf("You won!\n");
	}
	else {
		printf("You lost.\nGAME OVER\n");
	}
}
