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
#include "egueb_svg_element_tspan.h"
#include "egueb_svg_document.h"
#include "egueb_svg_shape_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EGUEB_SVG_ELEMENT_TSPAN_DESCRIPTOR egueb_svg_element_tspan_descriptor_get()
#define EGUEB_SVG_ELEMENT_TSPAN_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k, 		\
		Egueb_Svg_Element_Tspan_Class, EGUEB_SVG_ELEMENT_TSPAN_DESCRIPTOR)
#define EGUEB_SVG_ELEMENT_TSPAN(o) ENESIM_OBJECT_INSTANCE_CHECK(o, 		\
		Egueb_Svg_Element_Tspan, EGUEB_SVG_ELEMENT_TSPAN_DESCRIPTOR)

typedef struct _Egueb_Svg_Element_Tspan
{
	Egueb_Svg_Shape base;
	/* properties */
	Egueb_Dom_Node *x;
	Egueb_Dom_Node *y;
	/* private */
	double gx;
	double gy;
	Enesim_Renderer *r;
} Egueb_Svg_Element_Tspan;

typedef struct _Egueb_Svg_Element_Tspan_Class
{
	Egueb_Svg_Shape_Class base;
} Egueb_Svg_Element_Tspan_Class;

/* from a node inserted or node removed event, get the tspan node in case
 * the node inserted/removed is of that type and the parent is ourselves
 */
static Eina_Bool _egueb_svg_element_tspan_mutation_get_tspan(Egueb_Dom_Event *e,
		Egueb_Dom_Node *us, Egueb_Dom_Node **tspan)
{
	Egueb_Dom_Node *target = NULL;
	Egueb_Dom_Node *related = NULL;
	Egueb_Dom_Node_Type type;
	Eina_Bool ret = EINA_FALSE;

	egueb_dom_event_mutation_related_get(e, &related);
	if (related != us)
	{
		goto not_us;
	}

	egueb_dom_event_target_get(e, &target);
	egueb_dom_node_type_get(target, &type);
	if (type != EGUEB_DOM_NODE_TYPE_TSPAN_NODE)
	{
		goto not_tspan;
	}

	*tspan = egueb_dom_node_ref(target);
	ret = EINA_TRUE;
not_tspan:
	egueb_dom_node_unref(target);
not_us:
	egueb_dom_node_unref(related);
	return ret;
}

static void _egueb_svg_element_tspan_node_inserted_cb(Egueb_Dom_Event *e,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *tspan = NULL;

	if (_egueb_svg_element_tspan_mutation_get_tspan(e, n, &tspan))
	{
		Egueb_Svg_Element_Tspan *thiz;
		Enesim_Tspan_Buffer *nb = NULL;

		thiz = EGUEB_SVG_ELEMENT_TSPAN(n);
		/* set the internal buffer of the tspan span to be the one
		 * on the tspan node */
		egueb_dom_character_data_buffer_get(tspan, &nb);
		enesim_renderer_tspan_span_real_buffer_set(thiz->r, nb);
		/* now make the tspan node to use our own buffer from now on */
		enesim_renderer_tspan_span_buffer_get(thiz->r, &nb);
		egueb_dom_character_data_buffer_set(tspan, nb);
		egueb_dom_node_unref(tspan);
	}
}

static void _egueb_svg_element_tspan_node_removed_cb(Egueb_Dom_Event *e,
		void *data)
{
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *tspan = NULL;

	if (_egueb_svg_element_tspan_mutation_get_tspan(e, n, &tspan))
	{
		Egueb_Svg_Element_Tspan *thiz;
		Enesim_Tspan_Buffer *nb;

		thiz = EGUEB_SVG_ELEMENT_TSPAN(n);
		enesim_renderer_tspan_span_real_buffer_get(thiz->r, &nb);
		egueb_dom_character_data_buffer_set(tspan, nb);
		egueb_dom_node_unref(tspan);
	}
}

#if 0
static void _egueb_svg_element_tspan_tspan_set(Egueb_Dom_Tag *t, const char *tspan, unsigned int length)
{
	Egueb_Svg_Element_Tspan *thiz;
	char *modified;
	char past;

	thiz = _egueb_svg_element_tspan_get(t);
	if (!tspan) return;

	past = tspan[length];
	modified = (char *)tspan;
	modified[length] = '\0';

	DBG("setting tspan %s", modified);
	enesim_renderer_tspan_span_tspan_set(thiz->r, modified);
	modified[length] = past;
	thiz->changed = EINA_TRUE;
}

static void _egueb_svg_element_tspan_tspan_get(Egueb_Dom_Tag *t, const char **tspan, unsigned int *length)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = _egueb_svg_element_tspan_get(t);

	DBG("getting tspan");
	enesim_renderer_tspan_span_tspan_get(thiz->r, tspan);
	*length = 0;
}

static Enesim_Renderer * _egueb_svg_element_tspan_renderer_get(Egueb_Dom_Tag *t)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = _egueb_svg_element_tspan_get(t);
	return thiz->r;
}

static Egueb_Svg_Element_Setup_Return _egueb_svg_element_tspan_setup(Egueb_Dom_Tag *t,
		Egueb_Svg_Contspan *c,
		Egueb_Svg_Element_Contspan *ctx,
		Egueb_Svg_Attribute_Presentation *attr,
		Enesim_Log **error)
{
	Egueb_Svg_Element_Tspan *thiz;
	Egueb_Svg_Length lx, ly;

	thiz = _egueb_svg_element_tspan_get(t);

	/* position */
	egueb_svg_attribute_animated_length_final_get(&thiz->x, &lx);
	egueb_svg_attribute_animated_length_final_get(&thiz->y, &ly);
	thiz->gx = egueb_svg_coord_final_get(&lx, ctx->viewbox.w, ctx->font_size);
	thiz->gy = egueb_svg_coord_final_get(&ly, ctx->viewbox.h, ctx->font_size);

	DBG("calling the setup on the tspan (%g %g)", thiz->gx, thiz->gy);

	return ESVG_SETUP_OK;
}

static Egueb_Svg_Renderable_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get	= */ _egueb_svg_element_tspan_attribute_get,
	/* .cdata_set		= */ NULL,
	/* .tspan_set		= */ _egueb_svg_element_tspan_tspan_set,
	/* .tspan_get		     = */ _egueb_svg_element_tspan_tspan_get,
	/* .free		= */ _egueb_svg_element_tspan_free,
	/* .initialize		= */ NULL,
	/* .attribute_set	= */ _egueb_svg_element_tspan_attribute_set,
	/* .attribute_animated_fetch = */ NULL,
	/* .setup		= */ _egueb_svg_element_tspan_setup,
	/* .renderer_get	= */ _egueb_svg_element_tspan_renderer_get,
	/* .renderer_propagate	= */ _egueb_svg_element_tspan_renderer_propagate,
};
#endif
/*----------------------------------------------------------------------------*
 *                               Shape interface                              *
 *----------------------------------------------------------------------------*/
static Eina_Bool _egueb_svg_element_tspan_generate_geometry(Egueb_Svg_Shape *s)
{
	Egueb_Svg_Element_Tspan *thiz;
	Egueb_Svg_Element *e, *e_parent;
	Egueb_Svg_Length x, y;
	Egueb_Svg_Font_Size font_size;
	Egueb_Dom_Node *relative, *doc;
	Enesim_Matrix inv;
	int max;
	double doc_font_size, gfont;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(s);
	egueb_dom_attr_final_get(thiz->x, &x);
	egueb_dom_attr_final_get(thiz->y, &y);

	/* calculate the real size */
	egueb_svg_element_geometry_relative_get(EGUEB_DOM_NODE(s), &relative);
	if (!relative)
	{
		WARN("No relative available");
		return EINA_FALSE;
	}
	egueb_dom_node_document_get(EGUEB_DOM_NODE(s), &doc);
	if (!doc)
	{
		WARN("No document set");
		egueb_dom_node_unref(relative);
		return EINA_FALSE;
	}

	e_parent = EGUEB_SVG_ELEMENT(relative);
	egueb_svg_document_font_size_get(doc, &doc_font_size);

	thiz->gx = egueb_svg_coord_final_get(&x, e_parent->viewbox.w, doc_font_size);
	thiz->gy = egueb_svg_coord_final_get(&y, e_parent->viewbox.h, doc_font_size);

	egueb_dom_node_unref(relative);
	egueb_dom_node_unref(doc);

	/* set the position */
	e = EGUEB_SVG_ELEMENT(s);
	/* FIXME the font size accumulates from its parent, so we need to do
	 * similar to the transformation
	 */
	egueb_dom_attr_final_get(e->font_size, &font_size);
	gfont = egueb_svg_font_size_final_get(&font_size, e_parent->viewbox.w, e_parent->viewbox.h, doc_font_size, doc_font_size);

	enesim_renderer_tspan_span_size_set(thiz->r, ceil(gfont));
	enesim_renderer_tspan_span_max_ascent_get(thiz->r, &max);
	enesim_renderer_origin_set(thiz->r, thiz->gx, thiz->gy - max);

	DBG("matrix %" ENESIM_MATRIX_FORMAT, ENESIM_MATRIX_ARGS (&e->transform));
	enesim_matrix_inverse(&e->transform, &inv);
	enesim_renderer_transformation_set(thiz->r, &inv);

	DBG("x: %g, y: %g, font-size: %g", thiz->gx, thiz->gy, ceil(gfont));

	return EINA_TRUE;
}

static void _egueb_svg_element_tspan_renderer_propagate(Egueb_Svg_Shape *s,
		Egueb_Svg_Painter *painter)
{
	Egueb_Svg_Element_Tspan *thiz;
	Enesim_Color color;
	Enesim_Color fill_color;
	Eina_Bool visibility;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(s);

	egueb_svg_painter_visibility_get(painter, &visibility);
	egueb_svg_painter_color_get(painter, &color);
	egueb_svg_painter_fill_color_get(painter, &fill_color);

	enesim_renderer_color_set(thiz->r, fill_color);
	enesim_renderer_visibility_set(thiz->r, visibility);
}
/*----------------------------------------------------------------------------*
 *                            Renderable interface                            *
 *----------------------------------------------------------------------------*/
static Enesim_Renderer * _egueb_svg_element_tspan_renderer_get(
		Egueb_Svg_Renderable *r)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(r);
	return enesim_renderer_ref(thiz->r);
}

static void _egueb_svg_element_tspan_bounds_get(Egueb_Svg_Renderable *r,
		Enesim_Rectangle *bounds)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(r);
#if 0
	enesim_rectangle_coords_from(bounds, thiz->gx, thiz->gy,
			thiz->gw, thiz->gh);
#endif
}
/*----------------------------------------------------------------------------*
 *                              Element interface                             *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_String * _egueb_svg_element_tspan_tag_name_get(
		Egueb_Dom_Element *e)
{
	return egueb_dom_string_ref(EGUEB_SVG_NAME_TSPAN);
}
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
EGUEB_DOM_ATTR_FETCH_DEFINE(egueb_svg_element_tspan, Egueb_Svg_Element_Tspan, x);
EGUEB_DOM_ATTR_FETCH_DEFINE(egueb_svg_element_tspan, Egueb_Svg_Element_Tspan, y);

ENESIM_OBJECT_INSTANCE_BOILERPLATE(EGUEB_SVG_SHAPE_DESCRIPTOR,
		Egueb_Svg_Element_Tspan, Egueb_Svg_Element_Tspan_Class,
		egueb_svg_element_tspan);

static void _egueb_svg_element_tspan_class_init(void *k)
{
	Egueb_Svg_Shape_Class *klass;
	Egueb_Svg_Renderable_Class *r_klass;
	Egueb_Dom_Element_Class *e_klass;

	klass = EGUEB_SVG_SHAPE_CLASS(k);
	klass->generate_geometry = _egueb_svg_element_tspan_generate_geometry;
	klass->renderer_propagate = _egueb_svg_element_tspan_renderer_propagate;

	r_klass = EGUEB_SVG_RENDERABLE_CLASS(k);
	r_klass->bounds_get = _egueb_svg_element_tspan_bounds_get;
	r_klass->renderer_get = _egueb_svg_element_tspan_renderer_get;

	e_klass= EGUEB_DOM_ELEMENT_CLASS(k);
	e_klass->tag_name_get = _egueb_svg_element_tspan_tag_name_get;
}

static void _egueb_svg_element_tspan_class_deinit(void *k)
{
}

static void _egueb_svg_element_tspan_instance_init(void *o)
{
	Egueb_Svg_Element_Tspan *thiz;
	Enesim_Renderer *r;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(o);
	thiz->r = enesim_renderer_tspan_span_new();
	enesim_renderer_tspan_span_font_name_set(thiz->r, "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
	enesim_renderer_color_set(thiz->r, 0xff000000);
	enesim_renderer_tspan_span_tspan_set(thiz->r, "Hello");

	/* Default values */
	enesim_renderer_rop_set(thiz->r, ENESIM_BLEND);

	/* create the properties */
	thiz->x = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_X),
			&EGUEB_SVG_LENGTH_0, EINA_TRUE,
			EINA_FALSE, EINA_FALSE);
	thiz->y = egueb_svg_attr_length_new(
			egueb_dom_string_ref(EGUEB_SVG_Y),
			&EGUEB_SVG_LENGTH_0, EINA_TRUE,
			EINA_FALSE, EINA_FALSE);

	EGUEB_DOM_ELEMENT_CLASS_PROPERTY_ADD(thiz, egueb_svg_element_tspan, x);
	EGUEB_DOM_ELEMENT_CLASS_PROPERTY_ADD(thiz, egueb_svg_element_tspan, y);

	/* add the events */
	egueb_dom_node_event_listener_add(EGUEB_DOM_NODE(o),
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_egueb_svg_element_tspan_node_inserted_cb,
			EINA_FALSE, thiz);
	egueb_dom_node_event_listener_add(EGUEB_DOM_NODE(o),
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_egueb_svg_element_tspan_node_removed_cb,
			EINA_FALSE, thiz);
}

static void _egueb_svg_element_tspan_instance_deinit(void *o)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(o);
	enesim_renderer_unref(thiz->r);
	/* destroy the properties */
	egueb_dom_node_unref(thiz->x);
	egueb_dom_node_unref(thiz->y);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * egueb_svg_element_tspan_new(void)
{
	Egueb_Dom_Node *n;

	n = ENESIM_OBJECT_INSTANCE_NEW(egueb_svg_element_tspan);
	return n;
}

EAPI Eina_Error egueb_svg_element_tspan_x_set(Egueb_Dom_Node *n,
		const Egueb_Svg_Length *x)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(n);
	return egueb_dom_attr_set(thiz->x, EGUEB_DOM_ATTR_TYPE_BASE, x);
}

EAPI Eina_Error egueb_svg_element_tspan_y_set(Egueb_Dom_Node *n,
		const Egueb_Svg_Length *y)
{
	Egueb_Svg_Element_Tspan *thiz;

	thiz = EGUEB_SVG_ELEMENT_TSPAN(n);
	return egueb_dom_attr_set(thiz->y, EGUEB_DOM_ATTR_TYPE_BASE, y);
}

