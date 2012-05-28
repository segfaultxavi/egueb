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
#include "esvg_private_clone.h"

#include "esvg_renderable.h"
#include "esvg_g.h"
#include "esvg_use.h"
#include "esvg_main.h"
/*
 * The 'use' element should be able to create a new instance from another
 * svg tree. Basically we set the 'link' property of the 'use' to the svg
 * tree. Once it is attached on the setup we should clone it if it is not
 * already cloned and render it that new tree as a 'g' element.
 * We need to add a clone function on the element descriptor to be able
 * to clone renderers easily.
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *ESVG_USE_X;
static Ender_Property *ESVG_USE_Y;
static Ender_Property *ESVG_USE_WIDTH;
static Ender_Property *ESVG_USE_HEIGHT;
static Ender_Property *ESVG_USE_LINK;

typedef struct _Esvg_Use
{
	/* properties */
	Esvg_Coord x;
	Esvg_Coord y;
	Esvg_Length width;
	Esvg_Length height;
	char *link;
	/* private */
	Esvg_Clone *clone;
	/* the always present g tag */
	Edom_Tag *g_t;
	Ender_Element *g_e;
	Enesim_Renderer *g_r;
} Esvg_Use;

static Esvg_Use * _esvg_use_get(Edom_Tag *t)
{
	Esvg_Use *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_USE)
		return NULL;
	thiz = esvg_instantiable_data_get(t);

	return thiz;
}

static void _esvg_use_topmost_changed_cb(Ender_Element *e, const char *event_name,
		void *event_data, void *data)
{
	Esvg_Use *thiz = data;
	Esvg_Element_Event_Topmost_Changed *ev = event_data;

	printf("topmost set on the use %p\n", ev->current);
	esvg_element_topmost_set(thiz->g_t, ev->current);
}
/*----------------------------------------------------------------------------*
 *                       The Esvg Renderable interface                        *
 *----------------------------------------------------------------------------*/
static void _esvg_use_initialize(Ender_Element *e)
{
	Esvg_Use *thiz;
	Edom_Tag *t;

	t = ender_element_object_get(e);
	thiz = _esvg_use_get(t);
	/* whenever the topmost is set on the use
	 * we should also set the topmost on the g
	 */
	printf("initializing!!!\n");
	ender_event_listener_add(e, "topmost_changed", _esvg_use_topmost_changed_cb, thiz);
}

static Eina_Bool _esvg_use_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "x") == 0)
	{
		Esvg_Coord x;

		esvg_length_string_from(&x, value);
		esvg_use_x_set(e, &x);
	}
	else if (strcmp(key, "y") == 0)
	{
		Esvg_Coord y;

		esvg_length_string_from(&y, value);
		esvg_use_y_set(e, &y);
	}
	else if (strcmp(key, "width") == 0)
	{
		Esvg_Length width;

		esvg_length_string_from(&width, value);
		esvg_use_width_set(e, &width);
	}
	else if (strcmp(key, "height") == 0)
	{
		Esvg_Length height;

		esvg_length_string_from(&height, value);
		esvg_use_height_set(e, &height);
	}
	else if (strcmp(key, "xlink:href") == 0)
	{
		esvg_use_link_set(e, value);
	}
	else
	{
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static Eina_Bool _esvg_use_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Enesim_Renderer * _esvg_use_renderer_get(Edom_Tag *t)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	return thiz->g_r;
}

static Esvg_Element_Setup_Return _esvg_use_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Esvg_Renderable_Context *rctx,
		Enesim_Error **error)
{
	Esvg_Use *thiz;
	Ender_Element *topmost;
	Ender_Element *link;
	Edom_Tag *clone_t;
	Enesim_Matrix translate;
	double tx, ty;

	thiz = _esvg_use_get(t);
#if 0
	/* we should append a new transformation */
	tx = esvg_length_final_get(&thiz->x, ctx->viewbox.width, ctx->font_size);
	ty = esvg_length_final_get(&thiz->y, ctx->viewbox.height, ctx->font_size);
	enesim_matrix_translate(&translate, tx, ty);
	enesim_matrix_compose(&ctx->transform.base, &translate, &ctx->transform.base);

	/* we take the shortcut here because there's no need to go through
	 * the normal enesim API
	 */
#endif
	printf("calling the setup on the use\n");
	if (!thiz->link)
	{
		printf("nothing to use\n");
		return EINA_TRUE;
	}

	esvg_element_internal_topmost_get(t, &topmost);
	if (!topmost)
	{
		printf("no topmost available\n");
		return EINA_TRUE;
	}

	esvg_svg_element_get(topmost, thiz->link, &link);
	thiz->clone = esvg_clone_new(link);

	if (!thiz->clone)
	{
		printf("impossible to clone\n");
		return EINA_TRUE;
	}

	/* TODO add the clone to the generated g */
	clone_t = ender_element_object_get(thiz->clone->our);
	ender_element_property_value_add(thiz->g_e, EDOM_CHILD, clone_t, NULL);
	/* setup the g */
	printf("doing the setup on the inner g!\n");
	/* set the parent */
	esvg_element_topmost_set(thiz->g_t, topmost);
	return esvg_element_internal_setup(thiz->g_t, c, error);
}

static void _esvg_use_clone(Edom_Tag *t, Edom_Tag *dt)
{
	/* inst this too much? */
}

static void _esvg_use_free(Edom_Tag *t)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	free(thiz);
}

static Esvg_Instantiable_Descriptor _descriptor = {
	/* .child_add		= */ NULL,
	/* .child_remove	= */ NULL,
	/* .attribute_get 	= */ _esvg_use_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .free 		= */ _esvg_use_free,
	/* .initialize 		= */ _esvg_use_initialize,
	/* .attribute_set 	= */ _esvg_use_attribute_set,
	/* .clone		= */ _esvg_use_clone,
	/* .setup		= */ _esvg_use_setup,
	/* .renderer_get	= */ _esvg_use_renderer_get,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_use_new(void)
{
	Esvg_Use *thiz;
	Edom_Tag *t;
	Enesim_Renderer *r;

	thiz = calloc(1, sizeof(Esvg_Use));
	if (!thiz) return NULL;
	thiz->g_e = esvg_g_new();
	thiz->g_r = esvg_renderable_renderer_get(thiz->g_e);
	thiz->g_t = ender_element_object_get(thiz->g_e);

	/* Default values */
	thiz->x = ESVG_COORD_0;
	thiz->y = ESVG_COORD_0;
	thiz->width = ESVG_LENGTH_0;
	thiz->height = ESVG_LENGTH_0;

	t = esvg_instantiable_new(&_descriptor, ESVG_USE, thiz);
	return t;
}

static void _esvg_use_x_set(Edom_Tag *t, const Esvg_Coord *x)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (x) thiz->x = *x;
}

static void _esvg_use_x_get(Edom_Tag *t, Esvg_Coord *x)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (x) *x = thiz->x;
}

static void _esvg_use_y_set(Edom_Tag *t, const Esvg_Coord *y)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (y) thiz->y = *y;
}

static void _esvg_use_y_get(Edom_Tag *t, Esvg_Coord *y)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (y) *y = thiz->y;
}

static void _esvg_use_width_set(Edom_Tag *t, const Esvg_Length *width)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (width) thiz->width = *width;
}

static void _esvg_use_width_get(Edom_Tag *t, Esvg_Length *width)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (width) *width = thiz->width;
}

static void _esvg_use_height_set(Edom_Tag *t, const Esvg_Length *height)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (height) thiz->height = *height;
}

static void _esvg_use_height_get(Edom_Tag *t, Esvg_Length *height)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (height) *height = thiz->height;
}

static void _esvg_use_link_set(Edom_Tag *t, const char *link)
{
	Esvg_Use *thiz;

	thiz = _esvg_use_get(t);
	if (thiz->link)
	{
		free(thiz->link);
		thiz->link = NULL;
	}
	if (link)
	{
		thiz->link = strdup(link);
	}
}

static void _esvg_use_link_get(Edom_Tag *t, const char **link)
{
	Esvg_Use *thiz;

	if (!link) return;
	thiz = _esvg_use_get(t);
	*link = thiz->link;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#define _esvg_use_x_is_set NULL
#define _esvg_use_y_is_set NULL
#define _esvg_use_width_is_set NULL
#define _esvg_use_height_is_set NULL
#define _esvg_use_link_is_set NULL
#include "generated/esvg_generated_use.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_use_new(void)
{
	return ender_element_new_with_namespace("use", "esvg");
}

EAPI Eina_Bool esvg_is_use(Ender_Element *e)
{
	Eina_Bool ret = EINA_TRUE;

	return ret;
}

EAPI void esvg_use_x_set(Ender_Element *e, const Esvg_Coord *x)
{
	ender_element_property_value_set(e, ESVG_USE_X, x, NULL);
}

EAPI void esvg_use_x_get(Ender_Element *e, Esvg_Coord *x)
{
}

EAPI void esvg_use_y_set(Ender_Element *e, const Esvg_Coord *y)
{
	ender_element_property_value_set(e, ESVG_USE_Y, y, NULL);
}

EAPI void esvg_use_y_get(Ender_Element *e, Esvg_Coord *y)
{
}

EAPI void esvg_use_width_set(Ender_Element *e, const Esvg_Length *width)
{
	ender_element_property_value_set(e, ESVG_USE_WIDTH, width, NULL);
}

EAPI void esvg_use_width_get(Ender_Element *e, Esvg_Length *width)
{
}

EAPI void esvg_use_height_set(Ender_Element *e, const Esvg_Length *height)
{
	ender_element_property_value_set(e, ESVG_USE_HEIGHT, height, NULL);
}

EAPI void esvg_use_height_get(Ender_Element *e, Esvg_Length *height)
{
}

EAPI void esvg_use_link_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_USE_LINK, v, NULL);
}

EAPI void esvg_use_link_get(Ender_Element *e, const char **v)
{
}
