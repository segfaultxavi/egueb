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
#include "esvg_private_element.h"
#include "esvg_private_renderable.h"
#include "esvg_element_polygon.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_LOG_DEFAULT _esvg_element_polygon_log

static int _esvg_element_polygon_log = -1;

static Ender_Property *ESVG_ELEMENT_POLYGON_POINT;

typedef struct _Esvg_Element_Polygon
{
	/* properties */
	Eina_List *points;
	/* private */
	Enesim_Renderer *r;
	Eina_Bool changed : 1;
} Esvg_Element_Polygon;

static Esvg_Element_Polygon * _esvg_element_polygon_get(Edom_Tag *t)
{
	Esvg_Element_Polygon *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_TYPE_POLYGON)
		return NULL;
	thiz = esvg_renderable_data_get(t);

	return thiz;
}

static void _esvg_element_polygon_points_cb(Esvg_Point *p, void *data)
{
	Ender_Element *e = data;

	esvg_element_polygon_point_add(e, p);
}
/*----------------------------------------------------------------------------*
 *                       The Esvg Renderable interface                        *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_element_polygon_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "points") == 0)
	{
		esvg_points_string_from(value, _esvg_element_polygon_points_cb, e);
	}
	else
	{
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static Eina_Bool _esvg_element_polygon_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Enesim_Renderer * _esvg_element_polygon_renderer_get(Edom_Tag *t)
{
	Esvg_Element_Polygon *thiz;

	thiz = _esvg_element_polygon_get(t);
	return thiz->r;
}

static Esvg_Element_Setup_Return _esvg_element_polygon_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Enesim_Error **error)
{
	return ESVG_SETUP_OK;
}

static Eina_Bool _esvg_element_polygon_renderer_propagate(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *ctx,
		const Esvg_Attribute_Presentation *attr,
		Esvg_Renderable_Context *rctx,
		Enesim_Error **error)
{
	Esvg_Element_Polygon *thiz;

	thiz = _esvg_element_polygon_get(t);

	DBG("polygon setup");
	/* the shape must always be closed */
	if (thiz->changed)
		enesim_renderer_figure_polygon_close(thiz->r, EINA_TRUE);

	/* shape properties */
	if (!rctx->fill_renderer)
		enesim_renderer_shape_fill_color_set(thiz->r, rctx->fill_color);
	else
		enesim_renderer_shape_fill_renderer_set(thiz->r, rctx->fill_renderer);
	if (!rctx->stroke_renderer)
		enesim_renderer_shape_stroke_color_set(thiz->r, rctx->stroke_color);
	else
		enesim_renderer_shape_stroke_renderer_set(thiz->r, rctx->stroke_renderer);
	enesim_renderer_shape_fill_rule_set(thiz->r, rctx->fill_rule);
	enesim_renderer_shape_stroke_weight_set(thiz->r, rctx->stroke_weight);
	enesim_renderer_shape_stroke_location_set(thiz->r, ENESIM_SHAPE_STROKE_CENTER);
	enesim_renderer_shape_stroke_cap_set(thiz->r, rctx->stroke_cap);
	enesim_renderer_shape_stroke_join_set(thiz->r, rctx->stroke_join);
	enesim_renderer_shape_draw_mode_set(thiz->r, rctx->draw_mode);
	/* base properties */
	enesim_renderer_transformation_set(thiz->r, &ctx->transform);
	enesim_renderer_color_set(thiz->r, rctx->color);

	return EINA_TRUE;
}

static void _esvg_element_polygon_free(Edom_Tag *t)
{
	Esvg_Element_Polygon *thiz;
	Esvg_Point *p;

	thiz = _esvg_element_polygon_get(t);
	EINA_LIST_FREE(thiz->points, p)
	{
		free(p);
	}
	enesim_renderer_unref(thiz->r);
	free(thiz);
}

static Esvg_Renderable_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get 	= */ _esvg_element_polygon_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .text_get		     = */ NULL,
	/* .free 		= */ _esvg_element_polygon_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ _esvg_element_polygon_attribute_set,
	/* .attribute_animated_fetch = */ NULL,
	/* .setup		= */ _esvg_element_polygon_setup,
	/* .renderer_get	= */ _esvg_element_polygon_renderer_get,
	/* .renderer_propagate	= */ _esvg_element_polygon_renderer_propagate,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_element_polygon_new(void)
{
	Esvg_Element_Polygon *thiz;
	Edom_Tag *t;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_Element_Polygon));
	if (!thiz) return NULL;

	r = enesim_renderer_figure_new();
	/* FIXME for now */
	enesim_renderer_figure_polygon_add(r);
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	thiz->r = r;
	/* default values */

	t = esvg_renderable_new(&_descriptor, ESVG_TYPE_POLYGON, thiz);
	return t;
}

static void _esvg_element_polygon_point_add(Edom_Tag *t, Esvg_Point *p)
{
	Esvg_Element_Polygon *thiz;
	Esvg_Point *new_point;

	if (!p) return;

	thiz = _esvg_element_polygon_get(t);
	new_point = calloc(1, sizeof(Esvg_Point));
	*new_point = *p;
	thiz->points = eina_list_append(thiz->points, new_point);
	enesim_renderer_figure_polygon_vertex_add(thiz->r, new_point->x, new_point->y);
	thiz->changed = EINA_TRUE;
}

static void _esvg_element_polygon_point_set(Edom_Tag *t, const Eina_List *pts)
{
	Esvg_Element_Polygon *thiz;
	Esvg_Point *pt;
	const Eina_List *l;

	thiz = _esvg_element_polygon_get(t);
	/* FIXME remove what we had */
	EINA_LIST_FOREACH (pts, l, pt)
	{
		_esvg_element_polygon_point_add(t, pt);
	}
}

static void _esvg_element_polygon_point_get(Edom_Tag *t, const Eina_List **pts)
{
	Esvg_Element_Polygon *thiz;

	thiz = _esvg_element_polygon_get(t);
	*pts = thiz->points;
}

/* The ender wrapper */
#define _esvg_element_polygon_delete NULL
#define _esvg_element_polygon_point_is_set NULL
#define _esvg_element_polygon_point_clear NULL
#define _esvg_element_polygon_point_remove NULL
#include "esvg_generated_element_polygon.c"
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void esvg_element_polygon_init(void)
{
	_esvg_element_polygon_log = eina_log_domain_register("esvg_element_polygon", ESVG_LOG_COLOR_DEFAULT);
	if (_esvg_element_polygon_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_esvg_element_polygon_init();
}

void esvg_element_polygon_shutdown(void)
{
	if (_esvg_element_polygon_log < 0)
		return;
	_esvg_element_polygon_shutdown();
	eina_log_domain_unregister(_esvg_element_polygon_log);
	_esvg_element_polygon_log = -1;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_element_polygon_new(void)
{
	return ESVG_ELEMENT_NEW("SVGPolygonElement");
}

EAPI Eina_Bool esvg_is_polygon(Ender_Element *e)
{
	Edom_Tag *t;
	Esvg_Type type;

	t = (Edom_Tag *)ender_element_object_get(e);
	type = esvg_element_internal_type_get(t);
	return (type == ESVG_TYPE_POLYGON) ? EINA_TRUE : EINA_FALSE;
}

EAPI void esvg_element_polygon_point_add(Ender_Element *e, Esvg_Point *p)
{
	ender_element_property_value_add(e, ESVG_ELEMENT_POLYGON_POINT, p, NULL);
}
