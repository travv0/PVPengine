#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <math.h>

#include "log.h"
#include "error.h"
#include "strings.h"
#include "object.h"
#include "str.h"

/* load a sprite */
void _sprload(struct sprite *spr, char *fname);

/* initialize sprite */
void _sprinit(struct sprite *spr);

void anistart(struct sprite *spr, int loop)
{
	spr->animating = TRUE;
	spr->looping = loop;
}

void anispeed(struct sprite *spr, float speed)
{
	spr->speed = speed;
}

void anireverse(struct sprite *spr, int reverse)
{
	spr->reverse = reverse;
}

void anipause(struct sprite *spr)
{
	spr->animating = FALSE;
}

void anistop(struct sprite *spr)
{
	spr->animating = FALSE;

	if (spr->reverse == FALSE)
		spr->curr_frame = 0;
	else
		spr->curr_frame = spr->frames - 1;
}

void aniset(struct sprite *spr, int frame)
{
	spr->curr_frame = frame;
}

void animate(struct sprite *spr)
{
	SDL_Rect draw_rect;
	if (spr->animating == TRUE)
		{
			/* stop animating if not set to looping and animation is done */
			if (spr->looping == FALSE &&
			    (
			     /* if sprite isn't reversing, stop at last frame */
			     (!spr->reverse && spr->curr_frame == spr->frames - 1) ||
			     /* otherwise, stop at first */
			     (spr->reverse && spr->curr_frame <= 0)
			     )
			    )
				anistop(spr);
			else if (spr->reverse)
				spr->curr_frame = (spr->curr_frame <= 0 ?
						   spr->frames - spr->speed :
						   spr->curr_frame - spr->speed);
			else
				spr->curr_frame = fmod(spr->curr_frame + spr->speed,
						       (float)spr->frames);
		}

	spr->source_rect.x = spr->base_rect.x + spr->source_rect.w *
		(int)spr->curr_frame;

	/* draw sprite from center */
	draw_rect.x = spr->dest_rect.x - spr->dest_rect.w / 2.0;
	draw_rect.y = spr->dest_rect.y - spr->dest_rect.h / 2.0;
	draw_rect.w = spr->dest_rect.w;
	draw_rect.h = spr->dest_rect.h;

	if (SDL_RenderCopy(RENDERER, spr->texture,
			   &spr->source_rect, &draw_rect) != 0) {
		throw_err(SDL_REND_COPY_ERR);
	}

	/* draw hitboxes */
	if (DEBUG) {
		static SDL_Surface *hitboxsurf;
		static SDL_Texture *hitboxtex;
		if (!hitboxsurf)
			 hitboxsurf = IMG_Load(HITBOX_PATH);
		if (!hitboxtex && hitboxsurf)
			hitboxtex = SDL_CreateTextureFromSurface(RENDERER, hitboxsurf);
		if (SDL_RenderCopy(RENDERER, hitboxtex,
			   		NULL, &spr->hb_rect) != 0) {
			throw_err(SDL_REND_COPY_ERR);
		}
	}
}

void initsprites(void)
{
	unsigned int i;
	for (i = 0; i < objmcnt(OBJ_MGR); ++i) {
		_sprload(&objmget(OBJ_MGR, i)->spr, objmget(OBJ_MGR, i)->spr.fname);
	}
}

void _sprload(struct sprite *spr, char *fname)
{
	SDL_Surface *surface = NULL;

	if (spr->load) {
		surface = IMG_Load(fname);

		if (surface == NULL) {
			throw_err(SDL_BMP_ERR);
		}

		if ((spr->texture = SDL_CreateTextureFromSurface(RENDERER,
								 surface)) == NULL)
			throw_err(SDL_TEXTURE_ERR);

		SDL_FreeSurface(surface);
	}
}

void unloadsprites(void)
{
	unsigned int i;
	for (i = 0; i < objmcnt(OBJ_MGR); ++i)
		SDL_DestroyTexture(objmget(OBJ_MGR, i)->spr.texture);
}

void drawall(void) {
	unsigned int i;
	for (i = 0; i < objmcnt(OBJ_MGR); ++i) {
		if (objmget(OBJ_MGR, i)->spr.texture != NULL)
			animate(&objmget(OBJ_MGR, i)->spr);
	}

	SDL_RenderPresent(RENDERER);
}

/* FIXME: figure out how to not call initsprites() every time */
void sprchange(struct object *obj, struct sprite spr)
{
	obj->spr = spr;
	initsprites();
}