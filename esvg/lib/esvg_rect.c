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
#define ESVG_RECT_MAGIC_CHECK(d) \
	do {\
		if (!EINA_MAGIC_CHECK(d, ESVG_RECT_MAGIC))\
			EINA_MAGIC_FAIL(d, ESVG_RECT_MAGIC);\
	} while(0)

typedef struct _Esvg_Rect
{
	EINA_MAGIC
	/* properties */
	Esvg_Coord x;
	Esvg_Coord y;
	Esvg_Length width;
	Esvg_Length height;
	Esvg_Length rx;
	Esvg_Length ry;
	/* private */
	Enesim_Renderer *r;
} Esvg_Rect;

static Esvg_Rect * _esvg_rect_get(Enesim_Renderer *r)
{
	Esvg_Rect *thiz;

	thiz = esvg_shape_data_get(r);
	ESVG_RECT_MAGIC_CHECK(thiz);
	return thiz;
}

/*----------------------------------------------------------------------------*
 *                         The ESVG element interface                         *
 *----------------------------------------------------------------------------*/
static const char * _esvg_rect_name_get(Enesim_Renderer *r)
{
	return "rect";
}

static Enesim_Renderer * _esvg_rect_renderer_get(Enesim_Renderer *r)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	return thiz->r;
}

static Eina_Bool _esvg_rect_setup(Enesim_Renderer *r, const Esvg_Element_State *estate,
		const Esvg_Shape_Enesim_State *dstate)
{
	Esvg_Rect *thiz;
	double x, y;
	double rx, ry;
	double width, height;

	thiz = _esvg_rect_get(r);

	/* set the position */
	x = esvg_length_final_get(&thiz->x, estate->viewbox_w);
	y = esvg_length_final_get(&thiz->y, estate->viewbox_h);
	enesim_renderer_rectangle_position_set(thiz->r, x, y);
	/* set the size */
	width = esvg_length_final_get(&thiz->width, estate->viewbox_w);
	height = esvg_length_final_get(&thiz->height, estate->viewbox_h);
	enesim_renderer_rectangle_size_set(thiz->r, width, height);

	/* FIXME enesim does not supports rx *and* ry */
	rx = esvg_length_final_get(&thiz->rx, estate->viewbox_w);
	enesim_renderer_rectangle_corner_radius_set(thiz->r, rx);
	enesim_renderer_rectangle_corners_set(thiz->r,
			EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);

	/* shape properties */
	if (!dstate->fill_renderer)
	{
		DBG("fill color 0x%x", dstate->fill_color);
		enesim_renderer_shape_fill_color_set(thiz->r, dstate->fill_color);
	}
	else
		enesim_renderer_shape_fill_renderer_set(thiz->r, dstate->fill_renderer);
	DBG("stroke color 0x%x", dstate->stroke_color);
	enesim_renderer_shape_stroke_color_set(thiz->r, dstate->stroke_color);
	DBG("stroke width %f", dstate->stroke_weight);
	enesim_renderer_shape_stroke_weight_set(thiz->r, dstate->stroke_weight);
	enesim_renderer_shape_draw_mode_set(thiz->r, dstate->draw_mode);
	enesim_renderer_shape_stroke_location_set(thiz->r, ENESIM_SHAPE_STROKE_CENTER);

	printf("calling the setup on the rect (%g %g %g %g) %g\n", x, y, width, height, dstate->stroke_weight);
	/* base properties */
	//printf("matrix %" ENESIM_MATRIX_FORMAT "\n", ENESIM_MATRIX_ARGS(&(estate->transform)));
	enesim_renderer_geometry_transformation_set(thiz->r, &estate->transform);
	enesim_renderer_color_set(thiz->r, dstate->color);

	return EINA_TRUE;
}

static void _esvg_rect_clone(Enesim_Renderer *r, Enesim_Renderer *dr)
{
	Esvg_Rect *thiz;
	Esvg_Rect *other;

	thiz = _esvg_rect_get(r);
	other = _esvg_rect_get(dr);

	other->x = thiz->x;
	other->y = thiz->y;
	other->width = thiz->width;
	other->height = thiz->height;
	other->rx = thiz->rx;
	other->ry = thiz->ry;
}

static void _esvg_rect_cleanup(Enesim_Renderer *r)
{

}

static Esvg_Shape_Descriptor _descriptor = {
	/* .setup =		*/ _esvg_rect_setup,
	/* .renderer_get =	*/ _esvg_rect_renderer_get,
	/* .name_get =		*/ _esvg_rect_name_get,
	/* .clone =		*/ _esvg_rect_clone,
	/* .cleanup =		*/ _esvg_rect_cleanup,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Enesim_Renderer * esvg_rect_new(void)
{
	Esvg_Rect *thiz;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_Rect));
	if (!thiz) return NULL;
	EINA_MAGIC_SET(thiz, ESVG_RECT_MAGIC);

	r = enesim_renderer_rectangle_new();
	thiz->r = r;

	/* Default values */
	enesim_renderer_rop_set(thiz->r, ENESIM_BLEND);
	thiz->x = ESVG_COORD_0;
	thiz->y = ESVG_COORD_0;
	thiz->width = ESVG_LENGTH_0;
	thiz->height = ESVG_LENGTH_0;

	r = esvg_shape_new(&_descriptor, thiz);
	return r;
}

EAPI Eina_Bool esvg_is_rect(Enesim_Renderer *r)
{
	Esvg_Rect *thiz;
	Eina_Bool ret;

	if (!esvg_is_shape(r))
		return EINA_FALSE;
	thiz = esvg_shape_data_get(r);
	ret = EINA_MAGIC_CHECK(thiz, ESVG_RECT_MAGIC);

	return ret;
}

EAPI void esvg_rect_x_set(Enesim_Renderer *r, const Esvg_Coord *x)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (x) thiz->x = *x;
}

EAPI void esvg_rect_x_get(Enesim_Renderer *r, Esvg_Coord *x)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (x) *x = thiz->x;
}

EAPI void esvg_rect_y_set(Enesim_Renderer *r, const Esvg_Coord *y)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (y) thiz->y = *y;
}

EAPI void esvg_rect_y_get(Enesim_Renderer *r, Esvg_Coord *y)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (y) *y = thiz->y;
}

EAPI void esvg_rect_width_set(Enesim_Renderer *r, const Esvg_Length *width)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (width) thiz->width = *width;
}

EAPI void esvg_rect_width_get(Enesim_Renderer *r, Esvg_Length *width)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (width) *width = thiz->width;
}

EAPI void esvg_rect_height_set(Enesim_Renderer *r, const Esvg_Length *height)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (height) thiz->height = *height;
}

EAPI void esvg_rect_height_get(Enesim_Renderer *r, Esvg_Length *height)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (height) *height = thiz->height;
}

EAPI void esvg_rect_rx_set(Enesim_Renderer *r, const Esvg_Coord *rx)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (rx) thiz->rx = *rx;
}

EAPI void esvg_rect_rx_get(Enesim_Renderer *r, Esvg_Coord *rx)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (rx) *rx = thiz->rx;
}

EAPI void esvg_rect_ry_set(Enesim_Renderer *r, const Esvg_Coord *ry)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (ry) thiz->ry = *ry;
}

EAPI void esvg_rect_ry_get(Enesim_Renderer *r, Esvg_Coord *ry)
{
	Esvg_Rect *thiz;

	thiz = _esvg_rect_get(r);
	if (ry) *ry = thiz->ry;
}
