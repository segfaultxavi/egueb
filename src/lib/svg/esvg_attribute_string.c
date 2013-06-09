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
#include "egueb_svg_main_private.h"
#include "egueb_svg_types.h"
#include "egueb_svg_attribute_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                            Animated Descriptor                             *
 *----------------------------------------------------------------------------*/
static void * _egueb_svg_animate_string_new(void)
{
	return NULL;
}

static Eina_Bool _egueb_svg_animate_string_get(const char *attr, void **value)
{
	char *v;

	v = strdup(attr);
	*value = v;

	return EINA_TRUE;
}

static void * _egueb_svg_animate_string_destination_new(void)
{
	Egueb_Svg_String_Animated *v;
	v = calloc(1, sizeof(Egueb_Svg_String_Animated));
	return v;
}

static void _egueb_svg_animate_string_destination_free(void *destination, Eina_Bool deep)
{
	Egueb_Svg_String_Animated *d = destination;
	if (deep)
	{
		free(d->base);
	}
	free(d);
}

static void _egueb_svg_animate_string_destination_keep(void *destination)
{
	Egueb_Svg_String_Animated *d = destination;
	d->base = strdup(d->base);
}

static void _egueb_svg_animate_string_destination_value_to(void *destination, void **value)
{
	Egueb_Svg_String_Animated *d = destination;
	*value = strdup(d->base);
}

static void _egueb_svg_animate_string_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	char *va = a;
	char *vb = b;
	Egueb_Svg_String_Animated *r = res;

	r->base = va;
	/* TODO
	etch_interpolate_string(va, vb, m, &r->base);
	*/
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Svg_Attribute_Animated_Descriptor egueb_svg_attribute_animated_string_descriptor = {
	/* .value_new 			= */ _egueb_svg_animate_string_new,
	/* .value_get 			= */ _egueb_svg_animate_string_get,
	/* .value_free 			= */ free,
	/* .destination_new 		= */ _egueb_svg_animate_string_destination_new,
	/* .destination_free 		= */ _egueb_svg_animate_string_destination_free,
	/* .destination_keep		= */ _egueb_svg_animate_string_destination_keep,
	/* .destination_value_from 	= */ NULL,
	/* .destination_value_to 	= */ _egueb_svg_animate_string_destination_value_to,
	/* .interpolate 		= */ _egueb_svg_animate_string_interpolate,
};

void egueb_svg_attribute_animated_string_merge_rel(const Egueb_Svg_Attribute_Animated_String *rel,
		const Egueb_Svg_Attribute_Animated_String *v,
		Egueb_Svg_Attribute_String *d)
{
	const Egueb_Svg_Attribute_String *rr = NULL;
	const Egueb_Svg_Attribute_String *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	egueb_svg_attribute_string_merge_rel(rr, vv, d);
}

void egueb_svg_attribute_animated_string_merge(const Egueb_Svg_Attribute_Animated_String *v,
		Egueb_Svg_Attribute_String *d)
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

void egueb_svg_attribute_string_merge_rel(const Egueb_Svg_Attribute_String *rel,
		const Egueb_Svg_Attribute_String *v,
		Egueb_Svg_Attribute_String *d)
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

void egueb_svg_attribute_animated_string_set(Egueb_Svg_Attribute_Animated_String *aa,
	const Egueb_Svg_String_Animated *v,
	Eina_Bool animate)
{
	Egueb_Svg_Attribute_String *a;

	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		egueb_svg_attribute_string_set(a, v->base);
	else
		egueb_svg_attribute_string_unset(a);
}

void egueb_svg_attribute_animated_string_extended_set(Egueb_Svg_Attribute_Animated_String *aa,
	const Egueb_Svg_String_Animated *v,
	Eina_Bool animate,
	int *set)
{
	Eina_Bool was_set;
	Eina_Bool is_set;

	was_set = aa->anim.is_set || aa->base.is_set;
	egueb_svg_attribute_animated_string_set(aa, v, animate);
	is_set = aa->anim.is_set || aa->base.is_set;
	if (was_set && !is_set)
		(*set)--;
	else if (!was_set && is_set)
		(*set)++;
}

void egueb_svg_attribute_animated_string_get(Egueb_Svg_Attribute_Animated_String *aa,
	Egueb_Svg_String_Animated *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void egueb_svg_attribute_string_unset(Egueb_Svg_Attribute_String *a)
{
	if (a->v)
	{
		free(a->v);
		a->v = NULL;
		a->is_set = EINA_FALSE;
	}
}


void egueb_svg_attribute_string_set(Egueb_Svg_Attribute_String *a, const char *v)
{
	if (a->v == v)
		return;

	egueb_svg_attribute_string_unset(a);
	if (v)
	{
		a->v = strdup(v);
		a->is_set = EINA_TRUE;
	}
}

void egueb_svg_attribute_animated_string_final_get(Egueb_Svg_Attribute_Animated_String *aa, char **v)
{
	if (aa->animated && aa->anim.is_set)
		*v = aa->anim.v;
	else
		*v = aa->base.v;
}

