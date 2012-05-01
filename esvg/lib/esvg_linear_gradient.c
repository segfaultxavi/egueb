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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "esvg_private_main.h"
#include "esvg_private_attribute_presentation.h"
#include "esvg_private_context.h"
#include "esvg_private_element.h"
#include "esvg_private_referenceable.h"
#include "esvg_private_paint_server.h"
#include "esvg_private_gradient.h"
#include "esvg_private_stop.h"

#include "esvg_linear_gradient.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *ESVG_LINEAR_GRADIENT_X1;
static Ender_Property *ESVG_LINEAR_GRADIENT_Y1;
static Ender_Property *ESVG_LINEAR_GRADIENT_X2;
static Ender_Property *ESVG_LINEAR_GRADIENT_Y2;

typedef struct _Esvg_Linear_Gradient
{
	/* properties */
	Esvg_Coord x1;
	Esvg_Coord y1;
	Esvg_Coord x2;
	Esvg_Coord y2;
	/* private */
	Eina_Bool x1_set : 1;
	Eina_Bool y1_set : 1;
	Eina_Bool x2_set : 1;
	Eina_Bool y2_set : 1;
} Esvg_Linear_Gradient;

static Esvg_Linear_Gradient * _esvg_linear_gradient_get(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_LINEARGRADIENT)
		return NULL;
	thiz = esvg_gradient_data_get(t);

	return thiz;
}

#if 0
static void _parser_linear_gradient_merge(Enesim_Renderer *r,
			Enesim_Renderer *rel)
{
	if (!esvg_linear_gradient_x1_is_set(r))
	{
		Esvg_Coord c;

		esvg_linear_gradient_x1_get(rel, &c);
		esvg_linear_gradient_x1_set(r, &c);
	}
	if (!esvg_linear_gradient_y1_is_set(r))
	{
		Esvg_Coord c;

		esvg_linear_gradient_y1_get(rel, &c);
		esvg_linear_gradient_y1_set(r, &c);
	}
	if (!esvg_linear_gradient_x2_is_set(r))
	{
		Esvg_Coord c;

		esvg_linear_gradient_x2_get(rel, &c);
		esvg_linear_gradient_x2_set(r, &c);
	}
	if (!esvg_linear_gradient_y2_is_set(r))
	{
		Esvg_Coord c;

		esvg_linear_gradient_y2_get(rel, &c);
		esvg_linear_gradient_y2_set(r, &c);
	}
}
#endif
/*----------------------------------------------------------------------------*
 *                       Esvg Paint Server interface                          *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_linear_gradient_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "x1") == 0)
	{
		Esvg_Coord x;

		esvg_length_string_from(&x, value, ESVG_COORD_0);
		esvg_linear_gradient_x1_set(e, &x);
	}
	else if (strcmp(key, "y1") == 0)
	{
		Esvg_Coord y;

		esvg_length_string_from(&y, value, ESVG_COORD_0);
		esvg_linear_gradient_y1_set(e, &y);
	}
	else if (strcmp(key, "x2") == 0)
	{
		Esvg_Coord x;

		esvg_length_string_from(&x, value, ESVG_COORD_0);
		esvg_linear_gradient_x2_set(e, &x);
	}
	else if (strcmp(key, "y2") == 0)
	{
		Esvg_Coord y;

		esvg_length_string_from(&y, value, ESVG_COORD_0);
		esvg_linear_gradient_y2_set(e, &y);
	}

	return EINA_TRUE;
}

static Eina_Bool _esvg_linear_gradient_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Enesim_Renderer * _esvg_linear_gradient_renderer_new(Edom_Tag *t)
{
	Enesim_Renderer *r;

	r = enesim_renderer_gradient_linear_new();
	enesim_renderer_gradient_mode_set(r, ENESIM_PAD);
	return r;
}

static Esvg_Element_Setup_Return _esvg_linear_gradient_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Enesim_Renderer *r,
		Esvg_Gradient_Context *gctx,
		Enesim_Error **error)
{
	Esvg_Linear_Gradient *thiz;
	Esvg_Gradient_Units gu;
	Enesim_Repeat_Mode mode;
	Enesim_Matrix m;
	double x1;
	double y1;
	double x2;
	double y2;

	thiz = _esvg_linear_gradient_get(t);

	gu = gctx->units;
	switch (gctx->spread_method)
	{
		case ESVG_SPREAD_METHOD_PAD:
		mode = ENESIM_PAD;
		break;

		case ESVG_SPREAD_METHOD_REPEAT:
		mode = ENESIM_REPEAT;
		break;

		case ESVG_SPREAD_METHOD_REFLECT:
		mode = ENESIM_REFLECT;
		break;
	}
	enesim_renderer_gradient_mode_set(r, mode);

	if (gu == ESVG_OBJECT_BOUNDING_BOX)
	{
		/* check that the coordinates shold be set with (0,0) -> (1, 1) */
		x1 = esvg_length_final_get(&thiz->x1, 1);
		y1 = esvg_length_final_get(&thiz->y1, 1);
		x2 = esvg_length_final_get(&thiz->x2, 1);
		y2 = esvg_length_final_get(&thiz->y2, 1);
		enesim_matrix_values_set(&m, ctx->bounds.w, 0, ctx->bounds.x, 0, ctx->bounds.h, ctx->bounds.y, 0, 0, 1);
	}
	else
	{
		double w;
		double h;

		/* use the user space coordiantes */
		w = ctx->viewbox.width;
		h = ctx->viewbox.height;
		x1 = esvg_length_final_get(&thiz->x1, w);
		y1 = esvg_length_final_get(&thiz->y1, h);
		x2 = esvg_length_final_get(&thiz->x2, w);
		y2 = esvg_length_final_get(&thiz->y2, h);

		m = ctx->transform;
	}
	if (enesim_matrix_type_get(&gctx->transform) != ENESIM_MATRIX_IDENTITY)
	{
		enesim_matrix_compose(&m, &gctx->transform, &m);
	}
	enesim_renderer_geometry_transformation_set(r, &m);

	printf("line %g %g %g %g\n", x1, y1, x2, y2);
	enesim_renderer_gradient_linear_x0_set(r, x1);
	enesim_renderer_gradient_linear_y0_set(r, y1);
	enesim_renderer_gradient_linear_x1_set(r, x2);
	enesim_renderer_gradient_linear_y1_set(r, y2);

	return ESVG_SETUP_OK;
}

static void _esvg_linear_gradient_free(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
	free(thiz);
}

static Esvg_Gradient_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get 	= */ _esvg_linear_gradient_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .free 		= */ _esvg_linear_gradient_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ _esvg_linear_gradient_attribute_set,
	/* .clone		= */ NULL,
	/* .setup		= */ _esvg_linear_gradient_setup,
	/* .renderer_new	= */ _esvg_linear_gradient_renderer_new,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_linear_gradient_new(void)
{
	Esvg_Linear_Gradient *thiz;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_Linear_Gradient));

	/* default values */
	thiz->x1 = ESVG_LENGTH_0;
	thiz->y1 = ESVG_LENGTH_0;
	thiz->x2 = ESVG_LENGTH_100_PERCENT;
	thiz->y2 = ESVG_LENGTH_0;

	t = esvg_gradient_new(&_descriptor, ESVG_LINEARGRADIENT, thiz);
	return t;
}

static void _esvg_linear_gradient_x1_set(Edom_Tag *t, const Esvg_Coord *x1)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
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

static void _esvg_linear_gradient_x1_get(Edom_Tag *t, Esvg_Coord *x1)
{
	Esvg_Linear_Gradient *thiz;

	if (!x1) return;
	thiz = _esvg_linear_gradient_get(t);
	*x1 = thiz->x1;
}

static Eina_Bool _esvg_linear_gradient_x1_is_set(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
	return thiz->x1_set;
}

static void _esvg_linear_gradient_y1_set(Edom_Tag *t, const Esvg_Coord *y1)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
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

static void _esvg_linear_gradient_y1_get(Edom_Tag *t, Esvg_Coord *y1)
{
	Esvg_Linear_Gradient *thiz;

	if (!y1) return;
	thiz = _esvg_linear_gradient_get(t);
	*y1 = thiz->y1;
}

static Eina_Bool _esvg_linear_gradient_y1_is_set(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
	return thiz->y1_set;
}

static void _esvg_linear_gradient_x2_set(Edom_Tag *t, const Esvg_Coord *x2)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
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

static void _esvg_linear_gradient_x2_get(Edom_Tag *t, Esvg_Coord *x2)
{
	Esvg_Linear_Gradient *thiz;

	if (!x2) return;
	thiz = _esvg_linear_gradient_get(t);
	*x2 = thiz->x2;
}

static Eina_Bool _esvg_linear_gradient_x2_is_set(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
	return thiz->x2_set;
}

static void _esvg_linear_gradient_y2_set(Edom_Tag *t, const Esvg_Coord *y2)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
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

static void _esvg_linear_gradient_y2_get(Edom_Tag *t, Esvg_Coord *y2)
{
	Esvg_Linear_Gradient *thiz;

	if (!y2) return;
	thiz = _esvg_linear_gradient_get(t);
	*y2 = thiz->y2;
}

static Eina_Bool _esvg_linear_gradient_y2_is_set(Edom_Tag *t)
{
	Esvg_Linear_Gradient *thiz;

	thiz = _esvg_linear_gradient_get(t);
	return thiz->y2_set;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#include "generated/esvg_generated_linear_gradient.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_linear_gradient_new(void)
{
	return ender_element_new_with_namespace("linear_gradient", "esvg");
}

EAPI Eina_Bool esvg_is_linear_gradient(Ender_Element *e)
{
}

EAPI void esvg_linear_gradient_x1_set(Ender_Element *e, const Esvg_Coord *x1)
{
	ender_element_property_value_set(e, ESVG_LINEAR_GRADIENT_X1, x1, NULL);
}

EAPI void esvg_linear_gradient_x1_get(Ender_Element *e, Esvg_Coord *x1)
{
}

EAPI Eina_Bool esvg_linear_gradient_x1_is_set(Ender_Element *e)
{
}

EAPI void esvg_linear_gradient_y1_set(Ender_Element *e, const Esvg_Coord *y1)
{
	ender_element_property_value_set(e, ESVG_LINEAR_GRADIENT_Y1, y1, NULL);
}

EAPI void esvg_linear_gradient_y1_get(Ender_Element *e, Esvg_Coord *y1)
{
}

EAPI Eina_Bool esvg_linear_gradient_y1_is_set(Ender_Element *e)
{
}

EAPI void esvg_linear_gradient_x2_set(Ender_Element *e, const Esvg_Coord *x2)
{
	ender_element_property_value_set(e, ESVG_LINEAR_GRADIENT_X2, x2, NULL);
}

EAPI void esvg_linear_gradient_x2_get(Ender_Element *e, Esvg_Coord *x2)
{
}

EAPI Eina_Bool esvg_linear_gradient_x2_is_set(Ender_Element *e)
{
}

EAPI void esvg_linear_gradient_y2_set(Ender_Element *e, const Esvg_Coord *y2)
{
	ender_element_property_value_set(e, ESVG_LINEAR_GRADIENT_Y2, y2, NULL);
}

EAPI void esvg_linear_gradient_y2_get(Ender_Element *e, Esvg_Coord *y2)
{
}

EAPI Eina_Bool esvg_linear_gradient_y2_is_set(Ender_Element *e)
{
}
