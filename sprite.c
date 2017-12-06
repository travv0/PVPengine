#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <math.h>
#include "objmanager.h"

#include "log.h"
#include "error.h"
#include "strings.h"
#include "object.h"
#include "str.h"
#include "flags.h"
#include "../data.h"

/* load a sprite */
void _sprload(struct sprite *spr, char *fname);

/* initialize sprite */
void _sprinit(struct sprite *spr);

void anistart(struct sprite *spr, int loop)
{
	spr->flags |= S_ANIMATING;
	if (loop)
		spr->flags |= S_LOOPING;
	else
		spr->flags &= ~S_LOOPING;
}

void anispeed(struct sprite *spr, float speed)
{
	spr->speed = speed;
}

void anireverse(struct sprite *spr, int reverse)
{
	if (reverse)
		spr->flags |= S_REVERSE;
	else
		spr->flags &= ~S_REVERSE;
}

void anipause(struct sprite *spr)
{
	spr->flags &= ~S_ANIMATING;
}

void anistop(struct sprite *spr)
{
	spr->flags &= ~S_ANIMATING;

	if (spr->flags & S_REVERSE)
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
	if (spr->flags & S_ANIMATING) {
		if (spr->flags & S_PINGPONG) {
			if (spr->flags & S_REVERSE && spr->curr_frame <= 0) {
				spr->flags &= ~S_REVERSE;
				spr->curr_frame += 1;
			}
			else if (spr->flags & ~S_REVERSE &&
					spr->curr_frame >= spr->frames - spr->speed) {
				spr->flags |= S_REVERSE;
				spr->curr_frame -= 1;
			}
		}
		/* stop animating if not set to looping and animation is done */
		if (!(spr->flags & S_LOOPING) &&
				(
				 /* if sprite isn't reversing, stop at last frame */
				 (!(spr->flags & S_REVERSE) && spr->curr_frame == spr->frames - 1) ||
				 /* otherwise, stop at first */
				 ((spr->flags & S_REVERSE) && spr->curr_frame <= 0)
				)
		   )
			anistop(spr);
		else if (spr->flags & S_REVERSE)
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
	for (i = 0; i < NUMOFSPRITES; ++i) {
		if (SPRITES[i].fname)
			_sprload(&SPRITES[i], SPRITES[i].fname);
	}
}

void _sprload(struct sprite *spr, char *fname)
{
	SDL_Surface *surface = NULL;

	surface = IMG_Load(fname);

	if (surface == NULL) {
		throw_err(SDL_BMP_ERR);
	}

	if ((spr->texture = SDL_CreateTextureFromSurface(RENDERER,
					surface)) == NULL)
		throw_err(SDL_TEXTURE_ERR);

	SDL_FreeSurface(surface);
}

void unloadsprites(void)
{
	unsigned int i;
	for (i = 0; i < objmcnt(OBJ_MGR); ++i)
		SDL_DestroyTexture(objmget(OBJ_MGR, i)->spr.texture);
}

void drawall(void) {
	unsigned int i;

	objmsort(OBJ_MGR);

	for (i = 0; i < objmcnt(OBJ_MGR); ++i) {
		if (objmget(OBJ_MGR, i)->spr.texture != NULL)
			animate(&objmget(OBJ_MGR, i)->spr);
	}

	SDL_RenderPresent(RENDERER);
}

void sprchange(struct object *obj, struct sprite spr)
{
	obj->spr = spr;
}
