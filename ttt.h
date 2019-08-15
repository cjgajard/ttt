#ifndef TTT_H
#define TTT_H
#define BOARDLEN 9
#define MAXPLAYERS 2

enum ttt_error {
	ttt_OK,
	ttt_BADINPUT,
	ttt_PLAYED
};

enum ttt_tile {
	ttt_NOTILE,
	ttt_X,
	ttt_O,
	ttt_DRAW
};

struct ttt {
	int turn;
	int player_count;
	enum ttt_tile board[BOARDLEN];
};
#endif
