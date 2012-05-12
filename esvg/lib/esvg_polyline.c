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
#include "esvg_private_renderable.h"
#include "esvg_private_instantiable.h"
#include "esvg_polyline.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *ESVG_POLYLINE_POINT;

typedef struct _Esvg_Polyline
{
	/* properties */
	Eina_List *points;
	/* private */
	Enesim_Renderer *proxy;
	Enesim_Renderer *line;
	Enesim_Renderer *figure;
} Esvg_Polyline;

static Esvg_Polyline * _esvg_polyline_get(Edom_Tag *t)
{
	Esvg_Polyline *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_POLYLINE)
		return NULL;
	thiz = esvg_instantiable_data_get(t);

	return thiz;
}

static void _esvg_polyline_points_cb(Esvg_Point *p, void *data)
{
	Ender_Element *e = data;

	esvg_polyline_point_add(e, p);
}
/*----------------------------------------------------------------------------*
 *                       The Esvg Renderable interface                        *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_polyline_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "points") == 0)
	{
		esvg_points_string_from(value, _esvg_polyline_points_cb, e);
	}

	return EINA_TRUE;
}

static Eina_Bool _esvg_polyline_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Enesim_Renderer * _esvg_polyline_renderer_get(Edom_Tag *t)
{
	Esvg_Polyline *thiz;
	int nvert;

	thiz = _esvg_polyline_get(t);
	return thiz->proxy;
}

static Esvg_Element_Setup_Return _esvg_polyline_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Enesim_Error **error)
{
	return ESVG_SETUP_OK;
}

static Eina_Bool _esvg_polyline_renderer_propagate(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *ctx,
		const Esvg_Attribute_Presentation *attr,
		Esvg_Renderable_Context *rctx,
		Enesim_Error **error)
{
	Esvg_Polyline *thiz;
	Enesim_Renderer *r;
	int nvert;

	thiz = _esvg_polyline_get(t);

	/* FIXME gets the parents size or the topmost? */
	/* FIXME we should keep the old fill renderer */

	nvert = eina_list_count(thiz->points);
	if (nvert > 2)
		r = thiz->figure;
	else
		r = thiz->line;

	/* shape properties */
	enesim_renderer_shape_fill_color_set(r, rctx->fill_color);
	enesim_renderer_shape_fill_rule_set(r, rctx->fill_rule);
	enesim_renderer_shape_fill_renderer_set(r, rctx->fill_renderer);
	enesim_renderer_shape_stroke_color_set(r, rctx->stroke_color);
	enesim_renderer_shape_stroke_weight_set(r, rctx->stroke_weight);
	enesim_renderer_shape_draw_mode_set(r, rctx->draw_mode);
	enesim_renderer_shape_stroke_location_set(r, ENESIM_SHAPE_STROKE_CENTER);
	enesim_renderer_shape_stroke_cap_set(r, rctx->stroke_cap);
	enesim_renderer_shape_stroke_join_set(r, rctx->stroke_join);

	/* base properties */
	enesim_renderer_geometry_transformation_set(r, &ctx->transform.base);
	enesim_renderer_color_set(r, rctx->color);
	printf("calling the setup on the polyline ");
	/* setup the points */
	if (nvert > 2)
	{
		Esvg_Point *p;
		Eina_List *l;

		enesim_renderer_figure_clear(r);
		enesim_renderer_figure_polygon_add(r);
		EINA_LIST_FOREACH(thiz->points, l, p)
		{
			printf("(%f, %f) ", p->x, p->y);
			enesim_renderer_figure_polygon_vertex_add(r, p->x, p->y);
		}
	}
	else
	{
		Esvg_Point *p;
		Esvg_Point pts[2] = { { 0, 0 }, { 0, 0 } };
		Esvg_Point *pt = pts;
		Eina_List *l;

		EINA_LIST_FOREACH(thiz->points, l, p)
		{
			*pt = *p;
			pt++;
		}
		enesim_renderer_line_x0_set(r, pts[0].x);
		enesim_renderer_line_y0_set(r, pts[0].y);
		enesim_renderer_line_x1_set(r, pts[1].x);
		enesim_renderer_line_y1_set(r, pts[1].y);
		printf("using the line (%f, %f) (%f, %f)", pts[0].x, pts[0].y, pts[1].x, pts[1].y);
	}
	printf("\n");
	enesim_renderer_proxy_proxied_set(thiz->proxy, r);

	return EINA_TRUE;
}

static void _esvg_polyline_clone(Edom_Tag *t, Edom_Tag *dt)
{
#if 0
	Esvg_Polyline *thiz;
	Eina_List *l;
	Esvg_Point *point;

	thiz = _esvg_polyline_get(t);
	/* FIXME we could remove our own list of polylines and use a _get from the enesim_renderer_figure */
	EINA_LIST_FOREACH(thiz->points, l, point)
	{
		esvg_polyline_point_add(dr, point);
	}
#endif
}

static void _esvg_polyline_free(Edom_Tag *t)
{
	Esvg_Polyline *thiz;

	thiz = _esvg_polyline_get(t);
	free(thiz);
}

static Esvg_Instantiable_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get 	= */ _esvg_polyline_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .free 		= */ _esvg_polyline_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ _esvg_polyline_attribute_set,
	/* .clone		= */ _esvg_polyline_clone,
	/* .setup		= */ _esvg_polyline_setup,
	/* .renderer_get	= */ _esvg_polyline_renderer_get,
	/* .renderer_propagate	= */ _esvg_polyline_renderer_propagate,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_polyline_new(void)
{
	Esvg_Polyline *thiz;
	Edom_Tag *t;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_Polyline));
	if (!thiz) return NULL;

	r = enesim_renderer_proxy_new();
	thiz->proxy = r;

	r = enesim_renderer_figure_new();
	/* FIXME for now */
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->figure = r;

	r = enesim_renderer_line_new();
	/* FIXME for now */
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	thiz->line = r;


	/* default values */
	t = esvg_instantiable_new(&_descriptor, ESVG_POLYLINE, thiz);
	return t;
}

static void _esvg_polyline_point_add(Edom_Tag *t, Esvg_Point *p)
{
	Esvg_Polyline *thiz;
	Esvg_Point *new_point;

	if (!p) return;

	thiz = _esvg_polyline_get(t);
	new_point = calloc(1, sizeof(Esvg_Point));
	*new_point = *p;
	thiz->points = eina_list_append(thiz->points, new_point);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#define _esvg_polyline_point_is_set NULL
#define _esvg_polyline_point_set NULL
#define _esvg_polyline_point_get NULL
#define _esvg_polyline_point_clear NULL
#define _esvg_polyline_point_remove NULL
#include "generated/esvg_generated_polyline.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_polyline_new(void)
{
	return ender_element_new_with_namespace("polyline", "esvg");
}

EAPI void esvg_polyline_point_add(Ender_Element *e, Esvg_Point *p)
{
	ender_element_property_value_add(e, ESVG_POLYLINE_POINT, p, NULL);
}
