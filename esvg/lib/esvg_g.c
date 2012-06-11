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
#include "esvg_g.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Esvg_G
{
	/* properties */
	/* private */
	/* keep track if the renderable tree has changed, includeing the <a> tag */
	Eina_Bool renderable_tree_changed : 1;
	/* our renderer */
	Enesim_Renderer *r;
} Esvg_G;

static Esvg_G * _esvg_g_get(Edom_Tag *t)
{
	Esvg_G *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_G)
		return NULL;
	thiz = esvg_renderable_data_get(t);

	return thiz;
}

static Eina_Bool _esvg_g_setup_interceptor(Edom_Tag *t,
		Edom_Tag *child,
		Esvg_Context *c,
		Enesim_Error **error,
		void *data)
{
	Esvg_Type type;
	Esvg_G *thiz = data;

	type = esvg_element_internal_type_get(child);
	if (esvg_type_is_renderable(type) && thiz->renderable_tree_changed)
	{
		/* if renderable, add the renderer into the compound */
		Enesim_Renderer *r = NULL;

		esvg_renderable_internal_renderer_get(child, &r);
		enesim_renderer_compound_layer_add(thiz->r, r);
	}
 	else if (type == ESVG_A)
	{
		return esvg_element_internal_child_setup(child, c, error, _esvg_g_setup_interceptor, thiz);
	}
	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                          The Container interface                           *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_g_child_add(Edom_Tag *t, Edom_Tag *child)
{
	Esvg_G *thiz;
	Esvg_Type type;

	thiz = _esvg_g_get(t);
	if (!esvg_is_element_internal(child))
		return EINA_FALSE;

	type = esvg_element_internal_type_get(child);
	if (esvg_type_is_renderable(type) || type == ESVG_A)
	{
		thiz->renderable_tree_changed = EINA_TRUE;
		enesim_renderer_compound_layer_clear(thiz->r);
	}

	return EINA_TRUE;
}

static Eina_Bool _esvg_g_child_remove(Edom_Tag *t, Edom_Tag *child)
{
	Esvg_G *thiz;
	Esvg_Type type;

	thiz = _esvg_g_get(t);
	if (!esvg_is_element_internal(child))
		return EINA_FALSE;

	type = esvg_element_internal_type_get(child);
	if (esvg_type_is_renderable(type) || type == ESVG_A)
	{
		thiz->renderable_tree_changed = EINA_TRUE;
		enesim_renderer_compound_layer_clear(thiz->r);
	}

	return EINA_TRUE;
}

Enesim_Renderer * _esvg_g_renderer_get(Edom_Tag *t)
{
	Esvg_G *thiz;

	thiz = _esvg_g_get(t);
	return thiz->r;
}

static Esvg_Element_Setup_Return _esvg_g_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Enesim_Error **error)
{
	Esvg_G *thiz;
	Esvg_Element_Setup_Return ret;

	thiz = _esvg_g_get(t);
	ret = esvg_element_internal_child_setup(t, c, error, _esvg_g_setup_interceptor, thiz);
	thiz->renderable_tree_changed = EINA_FALSE;
	return ret;
}

static void _esvg_g_free(Edom_Tag *t)
{
	Esvg_G *thiz;

	thiz = _esvg_g_get(t);
	free(thiz);
}

static Eina_Bool _esvg_g_renderer_propagate(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *ctx,
		const Esvg_Attribute_Presentation *attr,
		Esvg_Renderable_Context *rctx,
		Enesim_Error **error)
{
	Esvg_G *thiz;

	thiz = _esvg_g_get(t);
	enesim_renderer_color_set(thiz->r, rctx->color);
	return EINA_TRUE;
}

static Esvg_Renderable_Descriptor _descriptor = {
	/* .child_add		= */ _esvg_g_child_add,
	/* .child_remove	= */ _esvg_g_child_remove,
	/* .attribute_get 	= */ NULL,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .free 		= */ _esvg_g_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ NULL,
	/* .setup		= */ _esvg_g_setup,
	/* .renderer_get	= */ _esvg_g_renderer_get,
	/* .renderer_propagate	= */ _esvg_g_renderer_propagate,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_g_new(void)
{
	Esvg_G *thiz;
	Edom_Tag *t;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_G));
	if (!thiz) return NULL;

	r = enesim_renderer_compound_new();
	thiz->r = r;
	enesim_renderer_rop_set(r, ENESIM_BLEND);

	t = esvg_renderable_new(&_descriptor, ESVG_G, thiz);
	return t;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#include "generated/esvg_generated_g.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_g_new(void)
{
	return ender_element_new_with_namespace("g", "esvg");
}

EAPI Eina_Bool esvg_is_g(Ender_Element *e)
{
	Edom_Tag *t;
	Esvg_Type type;

	t = (Edom_Tag *)ender_element_object_get(e);
	type = esvg_element_internal_type_get(t);
	return (type == ESVG_G) ? EINA_TRUE : EINA_FALSE;
}
