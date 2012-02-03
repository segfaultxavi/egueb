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
#include "Esvg.h"
#include "esvg_private.h"
#include "esvg_values.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_LINEAR_GRADIENT_MAGIC_CHECK(d) \
	do {\
		if (!EINA_MAGIC_CHECK(d, ESVG_LINEAR_GRADIENT_MAGIC))\
			EINA_MAGIC_FAIL(d, ESVG_LINEAR_GRADIENT_MAGIC);\
	} while(0)

typedef struct _Esvg_Linear_Gradient
{
	EINA_MAGIC
	/* properties */
	Esvg_Coord x1;
	Esvg_Coord y1;
	Esvg_Coord x2;
	Esvg_Coord y2;
	/* private */
	Enesim_Renderer *r;
	Eina_Bool x1_set : 1;
	Eina_Bool y1_set : 1;
	Eina_Bool x2_set : 1;
	Eina_Bool y2_set : 1;
} Esvg_Linear_Gradient;

static Esvg_Linear_Gradient * _esvg_linear_gradient_get(Enesim_Renderer *r)
{
	Esvg_Linear_Gradient *thiz;

	thiz = esvg_gradient_data_get(r);
	ESVG_LINEAR_GRADIENT_MAGIC_CHECK(thiz);

	return thiz;
}
/*----------------------------------------------------------------------------*
 *                       Esvg Paint Server interface                          *
 *----------------------------------------------------------------------------*/
static const char * _esvg_linear_gradient_name_get(Enesim_Renderer *r)
{
	return "linear_gradient";
}

static Enesim_Renderer * _linear_gradient_renderer_get(Enesim_Renderer *r,
		const Esvg_Element_State *state,
		const Esvg_Attribute_Presentation *attr)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	return thiz->r;
}

static void _linear_gradient_clone(Enesim_Renderer *r, Enesim_Renderer *rr)
{

}

static Eina_Bool _linear_gradient_setup(Enesim_Renderer *r, Enesim_Renderer *rel,
		Eina_List *stops)
{
	Esvg_Linear_Gradient *thiz;
	Esvg_Gradient_Stop *stop;
	Esvg_Gradient_Units gu;
	Enesim_Rectangle bbox;
	Eina_List *l;
	double x1;
	double y1;
	double x2;
	double y2;

	thiz = _esvg_linear_gradient_get(r);

	esvg_gradient_units_get(r, &gu);
	if (gu == ESVG_OBJECT_BOUNDING_BOX)
	{
		enesim_renderer_boundings(rel, &bbox);
	}
	else
	{
		/* use the user space coordiantes */
		printf("TODO\n");
	}

	x1 = esvg_length_final_get(&thiz->x1, bbox.w);
	y1 = esvg_length_final_get(&thiz->y1, bbox.h);
	x2 = esvg_length_final_get(&thiz->x2, bbox.w);
	y2 = esvg_length_final_get(&thiz->y2, bbox.h);

	printf("%g %g %g %g\n", x1, y1, x2, y2);
	/* FIXME for now */
	enesim_renderer_gradient_linear_x0_set(thiz->r, x1);
	enesim_renderer_gradient_linear_y0_set(thiz->r, y1);
	enesim_renderer_gradient_linear_x1_set(thiz->r, x2);
	enesim_renderer_gradient_linear_y1_set(thiz->r, y2);

	/* FIXME for now */
	EINA_LIST_FOREACH(stops, l, stop)
	{
		Enesim_Renderer_Gradient_Stop s;

		enesim_argb_components_from(&s.argb, lrint(stop->stop_opacity * 255),
				stop->stop_color.r, stop->stop_color.g, stop->stop_color.b);

		if (stop->offset.unit == ESVG_UNIT_LENGTH_PERCENT)
			s.pos = stop->offset.value / 100.0;
		else
			s.pos = stop->offset.value;

		if (s.pos > 1)
			s.pos = 1;
		else if (s.pos < 0)
			s.pos = 0;
		printf("pos = %g\n", s.pos);
		enesim_renderer_gradient_stop_add(thiz->r, &s);
	}

	return EINA_TRUE;
}

static Esvg_Gradient_Descriptor _descriptor = {
	/* .setup =		*/ _linear_gradient_setup,
	/* .name_get =		*/ _esvg_linear_gradient_name_get,
	/* .renderer_get =	*/ _linear_gradient_renderer_get,
	/* .clone =		*/ _linear_gradient_clone,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Enesim_Renderer * esvg_linear_gradient_new(void)
{
	Esvg_Linear_Gradient *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_Linear_Gradient));

	EINA_MAGIC_SET(thiz, ESVG_LINEAR_GRADIENT_MAGIC);

	r = enesim_renderer_gradient_linear_new();
	enesim_renderer_gradient_mode_set(r, ENESIM_PAD);
	thiz->r = r;
	/* default values */
	thiz->x1 = ESVG_LENGTH_0;
	thiz->y1 = ESVG_LENGTH_0;
	thiz->x2 = ESVG_LENGTH_100_PERCENT;
	thiz->y2 = ESVG_LENGTH_0;


	r = esvg_gradient_new(&_descriptor, thiz);

	return r;
}

EAPI void esvg_linear_gradient_x1_set(Enesim_Renderer *r, const Esvg_Coord *x1)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	if (!x1)
	{
		thiz->x1_set = EINA_FALSE;
	}
	else
	{
		thiz->x1 = *x1;
		thiz->x1_set = EINA_TRUE;
	}
}

EAPI void esvg_linear_gradient_x1_get(Enesim_Renderer *r, Esvg_Coord *x1)
{
	Esvg_Linear_Gradient *thiz;

	if (!x1) return;
	thiz = _esvg_linear_gradient_get(r);
	*x1 = thiz->x1;
}

EAPI Eina_Bool esvg_linear_gradient_x1_is_set(Enesim_Renderer *r)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	return thiz->x1_set;
}

EAPI void esvg_linear_gradient_y1_set(Enesim_Renderer *r, const Esvg_Coord *y1)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	if (!y1)
	{
		thiz->y1_set = EINA_FALSE;
	}
	else
	{
		thiz->y1 = *y1;
		thiz->y1_set = EINA_TRUE;
	}
}

EAPI void esvg_linear_gradient_y1_get(Enesim_Renderer *r, Esvg_Coord *y1)
{
	Esvg_Linear_Gradient *thiz;

	if (!y1) return;
	thiz = _esvg_linear_gradient_get(r);
	*y1 = thiz->y1;
}

EAPI Eina_Bool esvg_linear_gradient_y1_is_set(Enesim_Renderer *r)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	return thiz->y1_set;
}

EAPI void esvg_linear_gradient_x2_set(Enesim_Renderer *r, const Esvg_Coord *x2)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	if (!x2)
	{
		thiz->x2_set = EINA_FALSE;
	}
	else
	{
		thiz->x2 = *x2;
		thiz->x2_set = EINA_TRUE;
	}
}

EAPI void esvg_linear_gradient_x2_get(Enesim_Renderer *r, Esvg_Coord *x2)
{
	Esvg_Linear_Gradient *thiz;

	if (!x2) return;
	thiz = _esvg_linear_gradient_get(r);
	*x2 = thiz->x2;
}

EAPI Eina_Bool esvg_linear_gradient_x2_is_set(Enesim_Renderer *r)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	return thiz->x2_set;
}

EAPI void esvg_linear_gradient_y2_set(Enesim_Renderer *r, const Esvg_Coord *y2)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	if (!y2)
	{
		thiz->y2_set = EINA_FALSE;
	}
	else
	{
		thiz->y2 = *y2;
		thiz->y2_set = EINA_TRUE;
	}
}

EAPI void esvg_linear_gradient_y2_get(Enesim_Renderer *r, Esvg_Coord *y2)
{
	Esvg_Linear_Gradient *thiz;

	if (!y2) return;
	thiz = _esvg_linear_gradient_get(r);
	*y2 = thiz->y2;
}

EAPI Eina_Bool esvg_linear_gradient_y2_is_set(Enesim_Renderer *r)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(r);
	return thiz->y2_set;
}
