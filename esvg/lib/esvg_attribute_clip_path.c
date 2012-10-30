/* Esvg - SVG
 * Copyright (C) 2011 Jorge Luis Zapata, Vincent Torri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "esvg_private_main.h"
#include "esvg_types.h"
#include "esvg_private_attribute.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                            Animated Descriptor                             *
 *----------------------------------------------------------------------------*/
static void * _esvg_animate_clip_path_value_new(void)
{
	Esvg_Clip_Path *v;

	v = calloc(1, sizeof(Esvg_Clip_Path));
	return v;
}

static Eina_Bool _esvg_animate_clip_path_value_get(const char *attr, void **value)
{
	Esvg_Clip_Path *v = *value;

	esvg_clip_path_string_from(v, attr);
	return EINA_TRUE;
}

static void _esvg_animate_clip_path_value_free(void *value)
{
	Esvg_Clip_Path *v = value;

	if (v->type == ESVG_CLIP_PATH_IRI)
		free(v->value.iri);
	free(v);
}

static void * _esvg_animate_clip_path_destination_new(void)
{
	Esvg_Animated_Clip_Path *v;
	v = calloc(1, sizeof(Esvg_Animated_Clip_Path));
	return v;
}

static void _esvg_animate_clip_path_destination_free(void *destination, Eina_Bool deep)
{
	Esvg_Animated_Clip_Path *d = destination;
	free(d);
}

static void _esvg_animate_clip_path_destination_value_to(void *destination, void **value)
{
	Esvg_Animated_Clip_Path *d = destination;
	Esvg_Clip_Path *v = *value;

	*v = d->base;
}

static void _esvg_animate_clip_path_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	Esvg_Clip_Path *va = a;
	Esvg_Clip_Path *vb = b;
	Esvg_Clip_Path *vacc = acc;
	Esvg_Animated_Clip_Path *vadd = add;
	Esvg_Animated_Clip_Path *r = res;

	if (va->type != vb->type)
	{
		r->base.type = ESVG_CLIP_PATH_NONE;
		return;
	}
	r->base.type = va->type;
	switch (va->type)
	{
		case ESVG_CLIP_PATH_IRI:{
		/* TODO */
		}
		break;

		printf("NOT SUPPORTED\n");
		return;
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The different animated desriptors */
Esvg_Attribute_Animated_Descriptor esvg_attribute_animated_clip_path_descriptor = {
	/* .value_new 			= */ _esvg_animate_clip_path_value_new,
	/* .value_get 			= */ _esvg_animate_clip_path_value_get,
	/* .value_free 			= */ _esvg_animate_clip_path_value_free,
	/* .destination_new 		= */ _esvg_animate_clip_path_destination_new,
	/* .destination_free 		= */ _esvg_animate_clip_path_destination_free,
	/* .destination_keep 		= */ NULL,
	/* .destination_value_from 	= */ NULL,
	/* .destination_value_to 	= */ _esvg_animate_clip_path_destination_value_to,
	/* .interpolate 		= */ _esvg_animate_clip_path_interpolate,
};
/*----------------------------------------------------------------------------*
 *                                   Basic                                    *
 *----------------------------------------------------------------------------*/
void esvg_attribute_clip_path_merge_rel(const Esvg_Attribute_Clip_Path *rel,
		const Esvg_Attribute_Clip_Path *v,
		Esvg_Attribute_Clip_Path *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_clip_path_unset(Esvg_Attribute_Clip_Path *a, const Esvg_Clip_Path *def)
{
	a->v = *def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_clip_path_set(Esvg_Attribute_Clip_Path *a, const Esvg_Clip_Path *v,
		const Esvg_Clip_Path *def)
{
	if (!v)
	{
		esvg_attribute_clip_path_unset(a, def);
	}
	else
	{
		a->v = *v;
		if (v->type == ESVG_CLIP_PATH_IRI)
			a->v.value.iri = strdup(v->value.iri);
		a->is_set = EINA_TRUE;
	}
}
/*----------------------------------------------------------------------------*
 *                                  Animated                                  *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_clip_path_merge_rel(const Esvg_Attribute_Animated_Clip_Path *rel,
		const Esvg_Attribute_Animated_Clip_Path *v,
		Esvg_Attribute_Clip_Path *d)
{
	const Esvg_Attribute_Clip_Path *rr = NULL;
	const Esvg_Attribute_Clip_Path *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_clip_path_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_clip_path_merge(const Esvg_Attribute_Animated_Clip_Path *v,
		Esvg_Attribute_Clip_Path *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_animated_clip_path_set(Esvg_Attribute_Animated_Clip_Path *aa,
	const Esvg_Animated_Clip_Path *v,
	const Esvg_Clip_Path *def,
	Eina_Bool animate)
{
	Esvg_Attribute_Clip_Path *a;

	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_clip_path_set(a, &v->base, def);
	else
		esvg_attribute_clip_path_unset(a, def);
}

void esvg_attribute_animated_clip_path_extended_set(Esvg_Attribute_Animated_Clip_Path *aa,
	const Esvg_Animated_Clip_Path *v,
	const Esvg_Clip_Path *def,
	Eina_Bool animate,
	int *set)
{
	Eina_Bool was_set;
	Eina_Bool is_set;

	was_set = aa->anim.is_set || aa->base.is_set;
	esvg_attribute_animated_clip_path_set(aa, v, def, animate);
	is_set = aa->anim.is_set || aa->base.is_set;
	if (was_set && !is_set)
		(*set)--;
	else if (!was_set && is_set)
		(*set)++;
}

void esvg_attribute_animated_clip_path_get(Esvg_Attribute_Animated_Clip_Path *aa,
	Esvg_Animated_Clip_Path *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

