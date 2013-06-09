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
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                            Animated Descriptor                             *
 *----------------------------------------------------------------------------*/
void * egueb_svg_attribute_enum_value_new(void)
{
	int *v;

	v = calloc(1, sizeof(int));
	return v;
}

void * egueb_svg_attribute_enum_destination_new(void)
{
	Egueb_Svg_Animated_Enum *v;
	v = calloc(1, sizeof(Egueb_Svg_Animated_Enum));
	return v;
}

void egueb_svg_attribute_enum_destination_free(void *destination, Eina_Bool deep)
{
	Egueb_Svg_Animated_Enum *d = destination;
	free(d);
}

void egueb_svg_attribute_enum_destination_value_to(void *destination, void **value)
{
	Egueb_Svg_Animated_Enum *d = destination;
	int *v = *value;

	*v = d->base;
}

void egueb_svg_attribute_enum_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	int *va = a;
	int *vb = b;
	Egueb_Svg_Animated_Enum *r = res;

	/* enums can not accumulate or add */
	if (m < 1) r->base = *va;
	else r->base = *vb;
}
/*----------------------------------------------------------------------------*
 *                                   Basic                                    *
 *----------------------------------------------------------------------------*/
void egueb_svg_attribute_enum_unset(Egueb_Svg_Attribute_Enum *a, int def)
{
	a->v = def;
	a->is_set = EINA_FALSE;
}

void egueb_svg_attribute_enum_set(Egueb_Svg_Attribute_Enum *a, int v)
{
	a->v = v;
	a->is_set = EINA_TRUE;
}

void egueb_svg_attribute_enum_merge_rel(const Egueb_Svg_Attribute_Enum *rel,
		const Egueb_Svg_Attribute_Enum *v,
		Egueb_Svg_Attribute_Enum *d)
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
/*----------------------------------------------------------------------------*
 *                                  Animated                                  *
 *----------------------------------------------------------------------------*/
void egueb_svg_attribute_animated_enum_merge_rel(const Egueb_Svg_Attribute_Animated_Enum *rel,
		const Egueb_Svg_Attribute_Animated_Enum *v,
		Egueb_Svg_Attribute_Enum *d)
{
	const Egueb_Svg_Attribute_Enum *rr = NULL;
	const Egueb_Svg_Attribute_Enum *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	egueb_svg_attribute_enum_merge_rel(rr, vv, d);
}

void egueb_svg_attribute_animated_enum_merge(const Egueb_Svg_Attribute_Animated_Enum *v,
		Egueb_Svg_Attribute_Enum *d)
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

void egueb_svg_attribute_animated_enum_set(Egueb_Svg_Attribute_Animated_Enum *aa,
	const Egueb_Svg_Animated_Enum *v,
	int def,
	Eina_Bool animate)
{
	Egueb_Svg_Attribute_Enum *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		egueb_svg_attribute_enum_set(a, v->base);
	else
		egueb_svg_attribute_enum_unset(a, def);
}


void egueb_svg_attribute_animated_enum_extended_set(Egueb_Svg_Attribute_Animated_Enum *aa,
	const Egueb_Svg_Animated_Enum *v,
	int def,
	Eina_Bool animate,
	int *set)
{
	Eina_Bool was_set;
	Eina_Bool is_set;

	was_set = aa->anim.is_set || aa->base.is_set;
	egueb_svg_attribute_animated_enum_set(aa, v, def, animate);
	is_set = aa->anim.is_set || aa->base.is_set;
	if (was_set && !is_set)
		(*set)--;
	else if (!was_set && is_set)
		(*set)++;
}

void egueb_svg_attribute_animated_enum_get(Egueb_Svg_Attribute_Animated_Enum *aa,
	Egueb_Svg_Animated_Enum *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
