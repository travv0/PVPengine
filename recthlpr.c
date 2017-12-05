#include "recthlpr.h"
#include "../basic.h"	/* needed to get user-set windows size */

int chkoob(struct object o)
{
	if (chkhoob(o) || chkvoob(o))
		return 1;
	return 0;
}

int chkhoob(struct object o)
{
	if (o.spr.hb_rect.x < 0 || o.spr.hb_rect.x + o.spr.hb_rect.w > WIN_WIDTH)
		return 1;
	return 0;
}

int chkvoob(struct object o)
{
	if (o.spr.hb_rect.y + o.z < 0 || o.spr.hb_rect.y - o.z + o.spr.hb_rect.h > WIN_HEIGHT)
		return 1;
	return 0;
}
