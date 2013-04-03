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
#include "esvg_private_attribute_presentation.h"
#include "esvg_private_context.h"
#include "esvg_element_private.h"
#include "esvg_private_referenceable.h"
#include "esvg_paint_server_private.h"
#include "esvg_element_gradient_private.h"
#include "esvg_element_stop_private.h"
#include "esvg_private_types.h"

#include "esvg_element_radial_gradient.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_LOG_DEFAULT _esvg_element_radial_gradient_log

static int _esvg_element_radial_gradient_log = -1;

static Ender_Property *ESVG_ELEMENT_RADIAL_GRADIENT_CX;
static Ender_Property *ESVG_ELEMENT_RADIAL_GRADIENT_CY;
static Ender_Property *ESVG_ELEMENT_RADIAL_GRADIENT_FX;
static Ender_Property *ESVG_ELEMENT_RADIAL_GRADIENT_FY;
static Ender_Property *ESVG_ELEMENT_RADIAL_GRADIENT_R;

typedef struct _Esvg_Element_Radial_Gradient
{
	EINA_MAGIC
	/* properties */
	Esvg_Element_Gradient_Units units;
	Esvg_Attribute_Animated_Length cx;
	Esvg_Attribute_Animated_Length cy;
	Esvg_Attribute_Animated_Length fx;
	Esvg_Attribute_Animated_Length fy;
	Esvg_Attribute_Animated_Length rad;
	/* private */
} Esvg_Element_Radial_Gradient;

static Esvg_Element_Radial_Gradient * _esvg_element_radial_gradient_get(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_TYPE_RADIALGRADIENT)
		return NULL;
	thiz = esvg_element_gradient_data_get(t);

	return thiz;
}

static Eina_Bool _esvg_element_radial_gradient_deep_fy_get(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Length *fy)
{
	Esvg_Attribute_Length l;

	esvg_attribute_animated_length_merge(&thiz->fy, &l);
	if (!l.is_set)
	{
		Edom_Tag *href;

		href = esvg_element_gradient_href_tag_get(t);
 		if (href)
		{
			Esvg_Element_Radial_Gradient *other;

			other = _esvg_element_radial_gradient_get(href);
			if (!other) return EINA_FALSE;

			return _esvg_element_radial_gradient_deep_fy_get(other, href, fy);
		}
		return EINA_FALSE;
	}
	else
	{
		*fy = l.v;
		return EINA_TRUE;
	}
}

static Eina_Bool _esvg_element_radial_gradient_deep_fx_get(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Length *fx)
{
	Esvg_Attribute_Length l;

	esvg_attribute_animated_length_merge(&thiz->fx, &l);
	if (!l.is_set)
	{
		Edom_Tag *href;

		href = esvg_element_gradient_href_tag_get(t);
		if (href)
		{
			Esvg_Element_Radial_Gradient *other;

			other = _esvg_element_radial_gradient_get(href);
			if (!other) return EINA_FALSE;

			return _esvg_element_radial_gradient_deep_fx_get(other, href, fx);
		}
		return EINA_FALSE;
	}
	else
	{
		*fx = l.v;
		return EINA_TRUE;
	}
}

static void _esvg_element_radial_gradient_deep_cx_get(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Length *cx)
{
	Esvg_Attribute_Length l;
	Edom_Tag *href;

	href = esvg_element_gradient_href_tag_get(t);
	esvg_attribute_animated_length_merge(&thiz->cx, &l);
	if (!l.is_set && href)
	{
		Esvg_Element_Radial_Gradient *other;

		other = _esvg_element_radial_gradient_get(href);
		_esvg_element_radial_gradient_deep_cx_get(other, href, cx);
	}
	else
		*cx = l.v;
}

static void _esvg_element_radial_gradient_deep_cy_get(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Coord *cy)
{
	Esvg_Attribute_Length l;
	Edom_Tag *href;

	href = esvg_element_gradient_href_tag_get(t);
	esvg_attribute_animated_length_merge(&thiz->cy, &l);
	if (!l.is_set && href)
	{
		Esvg_Element_Radial_Gradient *other;

		other = _esvg_element_radial_gradient_get(href);
		_esvg_element_radial_gradient_deep_cy_get(other, href, cy);
	}
	else
		*cy = l.v;
}

static void _esvg_element_radial_gradient_deep_rad_get(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Length *rad)
{
	Esvg_Attribute_Length l;
	Edom_Tag *href;

	href = esvg_element_gradient_href_tag_get(t);
	esvg_attribute_animated_length_merge(&thiz->rad, &l);
	if (!l.is_set && href)
	{
		Esvg_Element_Radial_Gradient *other;

		other = _esvg_element_radial_gradient_get(href);
		_esvg_element_radial_gradient_deep_rad_get(other, href, rad);
	}
	else
		*rad = l.v;
}

static void _esvg_element_radial_gradient_merge(Esvg_Element_Radial_Gradient *thiz,
		Edom_Tag *t,
		Esvg_Coord *cx, Esvg_Coord *cy,
		Esvg_Coord *fx, Esvg_Coord *fy,
		Esvg_Length *rad)
{
	_esvg_element_radial_gradient_deep_cx_get(thiz, t, cx);
	_esvg_element_radial_gradient_deep_cy_get(thiz, t, cy);
	/* for fx and fy, if those are not set or inherited, then use
	 * cx or cy accordingly
	 */
	if (!_esvg_element_radial_gradient_deep_fx_get(thiz, t, fx))
		*fx = *cx;
	if (!_esvg_element_radial_gradient_deep_fy_get(thiz, t, fy))
		*fy = *cy;
	_esvg_element_radial_gradient_deep_rad_get(thiz, t, rad);
}
/*----------------------------------------------------------------------------*
 *                       Esvg Paint Server interface                          *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_element_radial_gradient_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "cx") == 0)
	{
		Esvg_Coord cx;

		esvg_length_string_from(&cx, value);
		esvg_element_radial_gradient_cx_set(e, &cx);
	}
	else if (strcmp(key, "cy") == 0)
	{
		Esvg_Coord cy;

		esvg_length_string_from(&cy, value);
		esvg_element_radial_gradient_cy_set(e, &cy);
	}
	else if (strcmp(key, "r") == 0)
	{
		Esvg_Length rad;

		esvg_length_string_from(&rad, value);
		esvg_element_radial_gradient_r_set(e, &rad);
	}
	else if (strcmp(key, "fx") == 0)
	{
		Esvg_Length fx;

		esvg_length_string_from(&fx, value);
		esvg_element_radial_gradient_fx_set(e, &fx);
	}
	else if (strcmp(key, "fy") == 0)
	{
		Esvg_Length fy;

		esvg_length_string_from(&fy, value);
		esvg_element_radial_gradient_fy_set(e, &fy);
	}
	else
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static int * _esvg_element_radial_gradient_attribute_animated_fetch(Edom_Tag *t, const char *attr)
{
	Esvg_Element_Radial_Gradient *thiz;
	int *animated = NULL;

	thiz = _esvg_element_radial_gradient_get(t);
	if (!strcmp(attr, "cx"))
	{
		animated = &thiz->cx.animated;	
	}
	else if (!strcmp(attr, "cy"))
	{
		animated = &thiz->cy.animated;	
	}
	else if (!strcmp(attr, "r"))
	{
		animated = &thiz->rad.animated;	
	}
	else if (!strcmp(attr, "fx"))
	{
		animated = &thiz->fx.animated;	
	}
	else if (!strcmp(attr, "fy"))
	{
		animated = &thiz->fy.animated;	
	}
	return animated;
}


static Eina_Bool _esvg_element_radial_gradient_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Enesim_Renderer * _esvg_element_radial_gradient_renderer_new(Edom_Tag *t)
{
	Enesim_Renderer *r;

	r = enesim_renderer_gradient_radial_new();
	enesim_renderer_gradient_mode_set(r, ENESIM_PAD);
	return r;
}

static Eina_Bool _esvg_element_radial_gradient_propagate(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *ctx,
		const Esvg_Attribute_Presentation *attr,
		Esvg_Element_Gradient_Context *gctx,
		Enesim_Renderer *r,
		Enesim_Log **error)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Element_Gradient_Units gu;
	Enesim_Repeat_Mode mode;
	Enesim_Matrix m;
	Esvg_Coord lcx;
	Esvg_Coord lcy;
	Esvg_Coord lfx;
	Esvg_Coord lfy;
	Esvg_Coord lrad;
	double cx;
	double cy;
	double fx;
	double fy;
	double rad;

	thiz = _esvg_element_radial_gradient_get(t);

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

		default:
		return EINA_FALSE;
	}
	enesim_renderer_gradient_mode_set(r, mode);

	/* the coordinates can come from the href
	 * we need to found which one isnt set and use that
	 * for the calculus
	 */
	_esvg_element_radial_gradient_merge(thiz, t, &lcx, &lcy, &lfx, &lfy, &lrad);

	if (gu == ESVG_OBJECT_BOUNDING_BOX)
	{
		DBG("Using object bounding box %g %g %g %g", ctx->bounds.x, ctx->bounds.y, ctx->bounds.w, ctx->bounds.h);
		/* check that the coordinates shold be set with (0,0) -> (1, 1) */
		cx = esvg_coord_final_get(&lcx, 1, 1);
		cy = esvg_coord_final_get(&lcy, 1, 1);
		fx = esvg_coord_final_get(&lfx, 1, 1);
		fy = esvg_coord_final_get(&lfy, 1, 1);
		/* the enesim renderer radius is not scaled by the transformation? */
		rad = esvg_length_final_get(&lrad, ctx->bounds.w, ctx->bounds.h, 1);
		enesim_matrix_values_set(&m, ctx->bounds.w, 0, ctx->bounds.x, 0, ctx->bounds.h, ctx->bounds.y, 0, 0, 1);
		/* transform the bounds using the context matrix */
		enesim_matrix_compose(&ctx->transform, &m, &m);
	}
	else
	{
		double w;
		double h;

		/* use the user space coordiantes */
		w = ctx->viewbox.w;
		h = ctx->viewbox.h;

		cx = esvg_coord_final_get(&lcx, w, ctx->font_size);
		cy = esvg_coord_final_get(&lcy, h, ctx->font_size);
		fx = esvg_coord_final_get(&lfx, w, ctx->font_size);
		fy = esvg_coord_final_get(&lfy, h, ctx->font_size);
		rad = esvg_length_final_get(&lrad, w, h, ctx->font_size);
		m = ctx->transform;
	}
	if (enesim_matrix_type_get(&gctx->transform) != ENESIM_MATRIX_IDENTITY)
	{
		enesim_matrix_compose(&m, &gctx->transform, &m);
	}
	enesim_renderer_transformation_set(r, &m);
	enesim_renderer_gradient_radial_center_x_set(r, cx);
	enesim_renderer_gradient_radial_center_y_set(r, cy);
	enesim_renderer_gradient_radial_focus_x_set(r, fx);
	enesim_renderer_gradient_radial_focus_y_set(r, fy);
	enesim_renderer_gradient_radial_radius_set(r, rad);

	DBG("Setup transform %" ENESIM_MATRIX_FORMAT " cx %g cy %g fx %g fy %g"
			" rad %g", ENESIM_MATRIX_ARGS (&m),
			cx, cy, fx, fy, rad);

	return ESVG_SETUP_OK;
}

static void _esvg_element_radial_gradient_free(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	free(thiz);
}

static Esvg_Element_Gradient_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get 	= */ _esvg_element_radial_gradient_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .free 		= */ _esvg_element_radial_gradient_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ _esvg_element_radial_gradient_attribute_set,
	/* .attribute_animated_fetch = */ _esvg_element_radial_gradient_attribute_animated_fetch,
	/* .setup		= */ NULL,
	/* .renderer_new	= */ _esvg_element_radial_gradient_renderer_new,
	/* .propagate		= */ _esvg_element_radial_gradient_propagate,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_element_radial_gradient_new(void)
{
	Esvg_Element_Radial_Gradient *thiz;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_Element_Radial_Gradient));

	/* default values */

	t = esvg_element_gradient_new(&_descriptor, ESVG_TYPE_RADIALGRADIENT, thiz);
	return t;
}

static void _esvg_element_radial_gradient_cx_set(Edom_Tag *t, const Esvg_Length_Animated *cx)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Length def = { ESVG_UNIT_LENGTH_PX, 0 };
	Eina_Bool animating;

	thiz = _esvg_element_radial_gradient_get(t);
	animating = esvg_element_attribute_animate_get(t);
	esvg_attribute_animated_length_set(&thiz->cx,
		cx, &def, animating);
}

static void _esvg_element_radial_gradient_cx_get(Edom_Tag *t, Esvg_Length_Animated *cx)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	esvg_attribute_animated_length_get(&thiz->cx,
		cx);
}

static Eina_Bool _esvg_element_radial_gradient_cx_is_set(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	return esvg_attribute_animated_length_is_set(&thiz->cx);
}

static void _esvg_element_radial_gradient_cy_set(Edom_Tag *t, const Esvg_Length_Animated *cy)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Length def = { ESVG_UNIT_LENGTH_PX, 0 };
	Eina_Bool animating;

	thiz = _esvg_element_radial_gradient_get(t);
	animating = esvg_element_attribute_animate_get(t);
	esvg_attribute_animated_length_set(&thiz->cy,
		cy, &def, animating);
}

static void _esvg_element_radial_gradient_cy_get(Edom_Tag *t, Esvg_Length_Animated *cy)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	esvg_attribute_animated_length_get(&thiz->cy,
		cy);
}

static Eina_Bool _esvg_element_radial_gradient_cy_is_set(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	return esvg_attribute_animated_length_is_set(&thiz->cy);
}

static void _esvg_element_radial_gradient_fx_set(Edom_Tag *t, const Esvg_Length_Animated *fx)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Length def = { ESVG_UNIT_LENGTH_PX, 0 };
	Eina_Bool animating;

	thiz = _esvg_element_radial_gradient_get(t);
	animating = esvg_element_attribute_animate_get(t);
	esvg_attribute_animated_length_set(&thiz->fx,
		fx, &def, animating);
}

static void _esvg_element_radial_gradient_fx_get(Edom_Tag *t, Esvg_Length_Animated *fx)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	esvg_attribute_animated_length_get(&thiz->fx,
		fx);
}

static Eina_Bool _esvg_element_radial_gradient_fx_is_set(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	return esvg_attribute_animated_length_is_set(&thiz->fx);
}

static void _esvg_element_radial_gradient_fy_set(Edom_Tag *t, const Esvg_Length_Animated *fy)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Length def = { ESVG_UNIT_LENGTH_PX, 0 };
	Eina_Bool animating;

	thiz = _esvg_element_radial_gradient_get(t);
	animating = esvg_element_attribute_animate_get(t);
	esvg_attribute_animated_length_set(&thiz->fy,
		fy, &def, animating);
}

static void _esvg_element_radial_gradient_fy_get(Edom_Tag *t, Esvg_Length_Animated *fy)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	esvg_attribute_animated_length_get(&thiz->fy,
		fy);
}

static Eina_Bool _esvg_element_radial_gradient_fy_is_set(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	return esvg_attribute_animated_length_is_set(&thiz->fy);
}

static void _esvg_element_radial_gradient_r_set(Edom_Tag *t, const Esvg_Length_Animated *r)
{
	Esvg_Element_Radial_Gradient *thiz;
	Esvg_Length def = { ESVG_UNIT_LENGTH_PX, 0 };
	Eina_Bool animating;

	thiz = _esvg_element_radial_gradient_get(t);
	animating = esvg_element_attribute_animate_get(t);
	esvg_attribute_animated_length_set(&thiz->rad,
		r, &def, animating);
}

static void _esvg_element_radial_gradient_r_get(Edom_Tag *t, Esvg_Length_Animated *r)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	esvg_attribute_animated_length_get(&thiz->rad,
		r);
}

static Eina_Bool _esvg_element_radial_gradient_r_is_set(Edom_Tag *t)
{
	Esvg_Element_Radial_Gradient *thiz;

	thiz = _esvg_element_radial_gradient_get(t);
	return esvg_attribute_animated_length_is_set(&thiz->rad);
}

/* The ender wrapper */
#define _esvg_element_radial_gradient_delete NULL
#include "esvg_generated_element_radial_gradient.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void esvg_element_radial_gradient_init(void)
{
	_esvg_element_radial_gradient_log = eina_log_domain_register("esvg_element_radial_gradient", ESVG_LOG_COLOR_DEFAULT);
	if (_esvg_element_radial_gradient_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_esvg_element_radial_gradient_init();
}

void esvg_element_radial_gradient_shutdown(void)
{
	if (_esvg_element_radial_gradient_log < 0)
		return;
	_esvg_element_radial_gradient_shutdown();
	eina_log_domain_unregister(_esvg_element_radial_gradient_log);
	_esvg_element_radial_gradient_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_element_radial_gradient_new(void)
{
	return ESVG_ELEMENT_NEW("SVGRadialGradientElement");
}

EAPI Eina_Bool esvg_is_radial_gradient(Ender_Element *e)
{
	Edom_Tag *t;
	Esvg_Type type;

	t = (Edom_Tag *)ender_element_object_get(e);
	type = esvg_element_internal_type_get(t);
	return (type == ESVG_TYPE_RADIALGRADIENT) ? EINA_TRUE : EINA_FALSE;
}

EAPI void esvg_element_radial_gradient_cx_set(Ender_Element *e, const Esvg_Coord *cx)
{
	ender_element_property_value_set(e, ESVG_ELEMENT_RADIAL_GRADIENT_CX, cx, NULL);
}

EAPI void esvg_element_radial_gradient_cx_get(Ender_Element *e, Esvg_Coord *cx)
{
	Esvg_Animated_Coord a;
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	_esvg_element_radial_gradient_cx_get(t, &a);
	*cx = a.base;
}

EAPI Eina_Bool esvg_element_radial_gradient_cx_is_set(Ender_Element *e)
{
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	return _esvg_element_radial_gradient_cx_is_set(t);
}

EAPI void esvg_element_radial_gradient_cy_set(Ender_Element *e, const Esvg_Coord *cy)
{
	ender_element_property_value_set(e, ESVG_ELEMENT_RADIAL_GRADIENT_CY, cy, NULL);
}

EAPI void esvg_element_radial_gradient_cy_get(Ender_Element *e, Esvg_Coord *cy)
{
	Esvg_Animated_Coord a;
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	_esvg_element_radial_gradient_cy_get(t, &a);
	*cy = a.base;
}

EAPI Eina_Bool esvg_element_radial_gradient_cy_is_set(Ender_Element *e)
{
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	return _esvg_element_radial_gradient_cy_is_set(t);
}

EAPI void esvg_element_radial_gradient_fx_set(Ender_Element *e, const Esvg_Coord *fx)
{
	ender_element_property_value_set(e, ESVG_ELEMENT_RADIAL_GRADIENT_FX, fx, NULL);
}

EAPI void esvg_element_radial_gradient_fx_get(Ender_Element *e, Esvg_Coord *fx)
{
	Esvg_Animated_Coord a;
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	_esvg_element_radial_gradient_fx_get(t, &a);
	*fx = a.base;
}

EAPI Eina_Bool esvg_element_radial_gradient_fx_is_set(Ender_Element *e)
{
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	return _esvg_element_radial_gradient_fx_is_set(t);
}

EAPI void esvg_element_radial_gradient_fy_set(Ender_Element *e, const Esvg_Coord *fy)
{
	ender_element_property_value_set(e, ESVG_ELEMENT_RADIAL_GRADIENT_FY, fy, NULL);
}

EAPI void esvg_element_radial_gradient_fy_get(Ender_Element *e, Esvg_Coord *fy)
{
	Esvg_Animated_Coord a;
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	_esvg_element_radial_gradient_fy_get(t, &a);
	*fy = a.base;
}

EAPI Eina_Bool esvg_element_radial_gradient_fy_is_set(Ender_Element *e)
{
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	return _esvg_element_radial_gradient_fy_is_set(t);
}

EAPI void esvg_element_radial_gradient_r_set(Ender_Element *e, const Esvg_Length *rad)
{
	ender_element_property_value_set(e, ESVG_ELEMENT_RADIAL_GRADIENT_R, rad, NULL);
}

EAPI void esvg_element_radial_gradient_r_get(Ender_Element *e, Esvg_Length *rad)
{
	Esvg_Animated_Coord a;
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	_esvg_element_radial_gradient_r_get(t, &a);
	*rad = a.base;
}

EAPI Eina_Bool esvg_element_radial_gradient_r_is_set(Ender_Element *e)
{
	Edom_Tag *t;

	t = (Edom_Tag *)ender_element_object_get(e);
	return _esvg_element_radial_gradient_r_is_set(t);
}
