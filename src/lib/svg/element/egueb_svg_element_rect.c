/* Egueb
 * Copyright (C) 2011 - 2013 Jorge Luis Zapata
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
#include "egueb_svg_main.h"
#include "egueb_svg_length.h"
#include "egueb_svg_element_rect.h"
#include "egueb_svg_document.h"
#include "egueb_svg_shape_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EGUEB_SVG_ELEMENT_RECT_DESCRIPTOR egueb_svg_element_rect_descriptor_get()
#define EGUEB_SVG_ELEMENT_RECT_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k, 		\
		Egueb_Svg_Element_Rect_Class, EGUEB_SVG_ELEMENT_RECT_DESCRIPTOR)
#define EGUEB_SVG_ELEMENT_RECT(o) ENESIM_OBJECT_INSTANCE_CHECK(o, 		\
		Egueb_Svg_Element_Rect, EGUEB_SVG_ELEMENT_RECT_DESCRIPTOR)

typedef struct _Egueb_Svg_Element_Rect
{
	Egueb_Svg_Shape base;
	/* properties */
	Egueb_Dom_Node *x;
	Egueb_Dom_Node *y;
	Egueb_Dom_Node *width;
	Egueb_Dom_Node *height;
	Egueb_Dom_Node *rx;
	Egueb_Dom_Node *ry;
	/* private */
	Enesim_Renderer *r;
	/* needed for the bounds */
	double gx, gy, gw, gh;
} Egueb_Svg_Element_Rect;

typedef struct _Egueb_Svg_Element_Rect_Class
{
	Egueb_Svg_Shape_Class base;
} Egueb_Svg_Element_Rect_Class;
/*----------------------------------------------------------------------------*
 *                               Shape interface                              *
 *----------------------------------------------------------------------------*/
static Eina_Bool _egueb_svg_element_rect_generate_geometry(Egueb_Svg_Shape *s,
		Egueb_Svg_Element *relative, Egueb_Dom_Node *doc)
{
	Egueb_Svg_Element_Rect *thiz;
	Egueb_Svg_Element *e;
	Egueb_Svg_Length x, y, rx, ry, w, h;
	Eina_Bool rx_set, ry_set;
	double grx, gry;
	double font_size;

	thiz = EGUEB_SVG_ELEMENT_RECT(s);
	egueb_dom_attr_final_get(thiz->x, &x);
	egueb_dom_attr_final_get(thiz->y, &y);
	rx_set = egueb_dom_attr_final_get(thiz->rx, &rx);
	ry_set = egueb_dom_attr_final_get(thiz->ry, &ry);
	if (!rx_set && !ry_set)
	{
		rx = ry = EGUEB_SVG_LENGTH_0;
	}
	else
	{
		if (!rx_set)
			rx = ry;
		else if (!ry_set)
			ry = rx;
	}
	egueb_dom_attr_final_get(thiz->width, &w);
	egueb_dom_attr_final_get(thiz->height, &h);

	font_size = egueb_svg_document_font_size_get(doc);
	thiz->gx = egueb_svg_coord_final_get(&x, relative->viewbox.w, font_size);
	thiz->gy = egueb_svg_coord_final_get(&y, relative->viewbox.h, font_size);
	thiz->gw = egueb_svg_coord_final_get(&w, relative->viewbox.w, font_size);
	thiz->gh = egueb_svg_coord_final_get(&h, relative->viewbox.h, font_size);
	grx = egueb_svg_coord_final_get(&rx, relative->viewbox.w, font_size);
	gry = egueb_svg_coord_final_get(&ry, relative->viewbox.h, font_size);

	/* set the position */
	enesim_renderer_rectangle_position_set(thiz->r, thiz->gx, thiz->gy);
	/* set the size */
	enesim_renderer_rectangle_size_set(thiz->r, thiz->gw, thiz->gh);
	/* set the corners */
	enesim_renderer_rectangle_corner_radii_set(thiz->r, grx, gry);
	enesim_renderer_rectangle_corners_set(thiz->r,
			EINA_TRUE, EINA_TRUE, EINA_TRUE, EINA_TRUE);

	e = EGUEB_SVG_ELEMENT(s);
	/* set the transformation */
	enesim_renderer_transformation_set(thiz->r, &e->transform);

	DBG("x: %g, y: %g, w: %g, h: %g, rx: %g, ry: %g", thiz->gx, thiz->gy, thiz->gw, thiz->gh,
			grx, gry);

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                            Renderable interface                            *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _egueb_svg_element_rect_renderer_get(
		Egueb_Svg_Renderable *r)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(r);
	return enesim_renderer_ref(thiz->r);
}

static void _egueb_svg_element_rect_bounds_get(Egueb_Svg_Renderable *r,
		Enesim_Rectangle *bounds)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(r);
	enesim_rectangle_coords_from(bounds, thiz->gx, thiz->gy,
			thiz->gw, thiz->gh);
}
/*----------------------------------------------------------------------------*
 *                              Element interface                             *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _egueb_svg_element_rect_tag_name_get(
		Egueb_Dom_Element *e)
{
	return egueb_dom_string_ref(EGUEB_SVG_NAME_RECT);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EGUEB_SVG_SHAPE_DESCRIPTOR,
		Egueb_Svg_Element_Rect, Egueb_Svg_Element_Rect_Class,
		egueb_svg_element_rect);

static void _egueb_svg_element_rect_class_init(void *k)
{
	Egueb_Svg_Shape_Class *klass;
	Egueb_Svg_Renderable_Class *r_klass;
	Egueb_Dom_Element_Class *e_klass;

	klass = EGUEB_SVG_SHAPE_CLASS(k);
	klass->generate_geometry = _egueb_svg_element_rect_generate_geometry;

	r_klass = EGUEB_SVG_RENDERABLE_CLASS(k);
	r_klass->bounds_get = _egueb_svg_element_rect_bounds_get;
	r_klass->renderer_get = _egueb_svg_element_rect_renderer_get;

	e_klass= EGUEB_DOM_ELEMENT_CLASS(k);
	e_klass->tag_name_get = _egueb_svg_element_rect_tag_name_get;
}

static void _egueb_svg_element_rect_instance_init(void *o)
{
	Egueb_Svg_Element_Rect *thiz;
	Egueb_Dom_Node *n;
	Enesim_Renderer *r;

	thiz = EGUEB_SVG_ELEMENT_RECT(o);
	r = enesim_renderer_rectangle_new();
	thiz->r = r;

	/* Default values */

	/* create the properties */
	thiz->x = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_X),
			&EGUEB_SVG_LENGTH_0, EINA_TRUE,
			EINA_FALSE, EINA_FALSE);
	thiz->y = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_Y),
			&EGUEB_SVG_LENGTH_0, EINA_TRUE,
			EINA_FALSE, EINA_FALSE);
	thiz->width = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_WIDTH),
			&EGUEB_SVG_LENGTH_0,
			EINA_TRUE, EINA_FALSE, EINA_FALSE);
	thiz->height = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_HEIGHT),
			&EGUEB_SVG_LENGTH_0,
			EINA_TRUE, EINA_FALSE, EINA_FALSE);
	thiz->rx = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_RX), NULL,
			EINA_TRUE, EINA_FALSE, EINA_FALSE);
	thiz->ry = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_NAME_RY), NULL,
			EINA_TRUE, EINA_FALSE, EINA_FALSE);

	n = EGUEB_DOM_NODE(o);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->x), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->y), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->width), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->height), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->rx), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->ry), NULL);
}

static void _egueb_svg_element_rect_instance_deinit(void *o)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(o);
	enesim_renderer_unref(thiz->r);
	/* destroy the properties */
	egueb_dom_node_unref(thiz->x);
	egueb_dom_node_unref(thiz->y);
	egueb_dom_node_unref(thiz->width);
	egueb_dom_node_unref(thiz->height);
	egueb_dom_node_unref(thiz->rx);
	egueb_dom_node_unref(thiz->ry);
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new rect element
 * @return The newly created rect element @ender_transfer{full}
 */
EAPI Egueb_Dom_Node * egueb_svg_element_rect_new(void)
{
	Egueb_Dom_Node *n;

	n = ENESIM_OBJECT_INSTANCE_NEW(egueb_svg_element_rect);
	return n;
}

/**
 * Sets the x coordinate of a rect element
 * @param[in] n The rect element to set the x coordinate @ender_transfer{none}
 * @param[in] x The x coordinate to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_x_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Coord *x)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->x, EGUEB_DOM_ATTR_TYPE_BASE, x);
}

/**
 * Gets the x coordinate of a rect element
 * @ender_prop{x}
 * @param[in] n The rect element to get the x coordinate @ender_transfer{none}
 * @param[out] x The pointer to store the x coordinate @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_x_get(Egueb_Dom_Node *n, Egueb_Svg_Coord_Animated *x)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->x, x);
}

/**
 * Sets the y coordinate of a rect element
 * @param[in] n The rect element to set the y coordinate @ender_transfer{none}
 * @param[in] y The y coordinate to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_y_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Coord *y)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->y, EGUEB_DOM_ATTR_TYPE_BASE, y);
}

/**
 * Gets the y coordinate of a rect element
 * @ender_prop{y}
 * @param[in] n The rect element to get the y coordinate @ender_transfer{none}
 * @param[out] y The pointer to store the y coordinate @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_y_get(Egueb_Dom_Node *n, Egueb_Svg_Coord_Animated *y)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->y, y);
}

/**
 * Sets the width of a rect element
 * @param[in] n The rect element to set the width @ender_transfer{none}
 * @param[in] width The width to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_width_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Length *width)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->width, EGUEB_DOM_ATTR_TYPE_BASE, width);
}

/**
 * Gets the width of a rect element
 * @ender_prop{width}
 * @param[in] n The rect element to get the width @ender_transfer{none}
 * @param[out] width The pointer to store the width @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_width_get(Egueb_Dom_Node *n, Egueb_Svg_Length_Animated *width)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->width, width);
}

/**
 * Sets the height of a rect element
 * @param[in] n The rect element to set the height @ender_transfer{none}
 * @param[in] height The height to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_height_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Length *height)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->height, EGUEB_DOM_ATTR_TYPE_BASE, height);
}

/**
 * Gets the height of a rect element
 * @ender_prop{height}
 * @param[in] n The rect element to get the height @ender_transfer{none}
 * @param[out] height The pointer to store the height @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_height_get(Egueb_Dom_Node *n, Egueb_Svg_Length_Animated *height)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->height, height);
}

/**
 * Sets the radius in the x coordinate of a rect element
 * @param[in] n The rect element to set the radius in the x coordinate @ender_transfer{none}
 * @param[in] rx The radius in the x coordinate to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_rx_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Coord *rx)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->rx, EGUEB_DOM_ATTR_TYPE_BASE, rx);
}

/**
 * Gets the radius in the x coordinate of a rect element
 * @ender_prop{rx}
 * @param[in] n The rect element to get the radius in the x coordinate @ender_transfer{none}
 * @param[out] rx The pointer to store the radius in the x coordinate @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_rx_get(Egueb_Dom_Node *n, Egueb_Svg_Coord_Animated *rx)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->rx, rx);
}

/**
 * Sets the radius in the y coordinate of a rect element
 * @param[in] n The rect element to set the radius in the y coordinate @ender_transfer{none}
 * @param[in] ry The radius in the y coordinate to set @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_ry_set_simple(Egueb_Dom_Node *n, const Egueb_Svg_Coord *ry)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	egueb_dom_attr_set(thiz->ry, EGUEB_DOM_ATTR_TYPE_BASE, ry);
}

/**
 * Gets the radius in the y coordinate of a rect element
 * @ender_prop{ry}
 * @param[in] n The rect element to get the radius in the y coordinate @ender_transfer{none}
 * @param[out] ry The pointer to store the radius in the y coordinate @ender_transfer{content}
 */
EAPI void egueb_svg_element_rect_ry_get(Egueb_Dom_Node *n, Egueb_Svg_Coord_Animated *ry)
{
	Egueb_Svg_Element_Rect *thiz;

	thiz = EGUEB_SVG_ELEMENT_RECT(n);
	EGUEB_SVG_ELEMENT_ATTR_ANIMATED_GET(thiz->ry, ry);
}
