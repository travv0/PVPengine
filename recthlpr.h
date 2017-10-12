#ifndef __ENG_RECTHLPR_H
#define __ENG_RECTHLPR_H

#include <SDL2/SDL.h>

/* check if rectangle is outside of window */
int chkoob(SDL_Rect r);

/* check if rectangle is outside of window, horizontally */
int chkhoob(SDL_Rect r);

/* check if rectangle is outside of window, vertically */
int chkvoob(SDL_Rect r);

#endif
