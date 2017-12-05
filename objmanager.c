#include "objmanager.h"
#include "error.h"
#include "log.h"

/* increase object manager's capacity */
void _objmcapup(struct objm *mgr);

void _quicksort(struct objm *mgr, int l, int r);

int _partition(struct objm *mgr, int l, int r);

void objminit(struct objm **mgr)
{
	*mgr = malloc(sizeof(struct objm));
	if (*mgr == NULL)
		throw_err(MALLOC_FAILED_ERR);

	(*mgr)->cap = 0;
	(*mgr)->objcnt = 0;
	(*mgr)->objs = NULL;
	log((sizeof(struct objm)),
			"Object manager: allocated new object manager "
			"of size %llu");
}

void _objmcapup(struct objm *mgr)
{
	mgr->cap = mgr->cap * OBJMANAGER_CAPUP_RATE;
	mgr->objs = realloc(mgr->objs, sizeof(struct object) * mgr->cap);
	log((sizeof(struct object) * mgr->cap),
			"Object manager: reallocated array to size %llu");
}

void objmadd(struct objm *mgr, struct object obj, struct sprite spr,
		float x, float y, float z)
{
	if (mgr == NULL) {
		throw_err(OBJM_NOT_INIT_ERR);
	}

	if (mgr->objcnt == 0) {
		mgr->cap = OBJMANAGER_INITIAL_CAPACITY;
		mgr->objs = malloc(sizeof(struct object) * mgr->cap);

		if (mgr->objs == NULL)
			throw_err(MALLOC_FAILED_ERR);

		log((sizeof(struct object) * mgr->cap),
				"Object manager: allocated new array "
				"of size %llu");
	}

	if (mgr->objcnt >= mgr->cap)
		_objmcapup(mgr);

	obj.x = x;
	obj.y = y;
	obj.z = z;

	mgr->objs[mgr->objcnt] = obj;
	mgr->objs[mgr->objcnt].spr = spr;
	mgr->objcnt++;

	log(mgr->objcnt, "Object manager: contains %d objects");
}

struct object *objmget(struct objm *mgr, unsigned int idx)
{
	if (idx >= mgr->objcnt)
		throw_err(OBJM_IDX_GET_OOR_WARN);

	return &mgr->objs[idx];
}

void objmset(struct objm *mgr, unsigned int idx, struct object obj)
{
	if (idx >= mgr->objcnt)
		throw_err(OBJM_IDX_SET_OOR_WARN);

	mgr->objs[idx] = obj;
}

struct object *objmfind(struct objm *mgr, int type)
{
	int i = 0;
	struct object *ret = NULL;

	while (i < mgr->objcnt)
		if ((ret = objmget(OBJ_MGR, i++))->type == type)
			return ret;

	return NULL;
}

void objmfree(struct objm *mgr)
{
	free(mgr->objs);
	free(mgr);
}

unsigned long objmcnt(struct objm *mgr)
{
	return mgr->objcnt;
}

void objmsort(struct objm *mgr)
{
	_quicksort(mgr, 0, objmcnt(mgr) - 1);
}

void _quicksort(struct objm *mgr, int l, int r)
{
	int j;

	if (l < r)
	{
		j = _partition(mgr, l, r);
		_quicksort(mgr, l, j - 1);
		_quicksort(mgr, j + 1, r);
	}

}

int _partition(struct objm *mgr, int l, int r)
{
	int i, j, pivot;
	struct object t;
	pivot = l;
	i = l;
	j = r;

	while (i < j)
	{
		while (objmget(mgr, i)->y + objmget(mgr, i)->z +
				objmget(mgr, i)->spr.dest_rect.h / 2 <=
				objmget(mgr, pivot)->y + objmget(mgr, pivot)->z +
				objmget(mgr, pivot)->spr.dest_rect.h / 2 &&
				i < r)
			i++;
		while (objmget(mgr, j)->y + objmget(mgr, j)->z +
				objmget(mgr, j)->spr.dest_rect.h / 2 >
				objmget(mgr, pivot)->y + objmget(mgr, pivot)->z +
				objmget(mgr, pivot)->spr.dest_rect.h / 2)
			j--;
		if (i < j) {
			t = *objmget(mgr, i);
			objmset(mgr, i, *objmget(mgr, j));
			objmset(mgr, j, t);
		}
	}

	t = *objmget(mgr, pivot);
	objmset(mgr, pivot, *objmget(mgr, j));
	objmset(mgr, j, t);

	return j;
}

void objmprint(struct objm *mgr)
{
	int i;
	struct object *obj;

	printf("[ ");
	for (i = 0; i < objmcnt(mgr); i++) {
		obj = objmget(mgr, i);
		printf("{%d %f} ", obj->type, obj->y + obj->z);
	}
	printf("]\n");
}
