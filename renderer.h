#ifndef RENDERER_H
#define RENDERER_H
int renderer_Input ();
void renderer_OnError (void *g, int error);
void renderer_OnMsg (char *msg);
void renderer_OnResult (void *g, int id, int result);
void renderer_Render (void *g);
#endif
