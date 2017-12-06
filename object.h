/* this file is where game object code goes.
 * a game object is a collection of a sprite, pos, etc., and also
 * a pointer to its step() function which will run every frame (at least) */
#ifndef __ENG_OBJECT_H
#define __ENG_OBJECT_H

#include "sprite.h"

struct object {
	int		type;
	struct sprite	spr;
	int		upspr;
	int		rightspr;
	int		downspr;
	int		leftspr;
	float		x;
	float		y;
	float		z;	/* for drawing objects after other objects */
	float		hvel;
	float		vvel;
	int		flags;
	int		(*step)(struct object *obj);
	float		ext[256];
};

/* loop through every object and run its step function */
void updateall(void);

#endif
