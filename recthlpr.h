#ifndef __ENG_RECTHLPR_H
#define __ENG_RECTHLPR_H

#include <SDL2/SDL.h>
#include "object.h"

/* check if rectangle is outside of window */
int chkoob(struct object o);

/* check if rectangle is outside of window, horizontally */
int chkhoob(struct object o);

/* check if rectangle is outside of window, vertically */
int chkvoob(struct object o);

#endif
