#ifndef __ENG_PVP_H
#define __ENG_PVP_H

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "error.h"
/* #include "game.h" */
#include "log.h"
#include "strings.h"
#include "basic.h"
#include "objmanager.h"

static int frms;			/* frame counter for fps display */

int pvpinit(int debug);

int pvpgameloop();

int pvpclean();

#endif
