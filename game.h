#ifndef GAME_H
#define GAME_H
#define GAMEOK 0
#define GAMEERROR -1
#define GAMEEVENT -2

int game_Join (void *g);
int game_Result (void *g);
int game_Turn (void *g);
int game_Update (void *g, int *n);
void *game_New ();
void game_Destroy (void *g);
#endif
