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
#include "esvg_private_svg.h"
#include "esvg_a.h"
#include "esvg_event.h"

/* TODO
 * whenever a child is added, if it is of type renderable, then
 * register the click event, whenever the click event occurs, trigger
 * a function on the svg that will inform that the document should
 * go to another location
 * the 'a' can have any child that its parent may container except an 'a'
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_LOG_DEFAULT esvg_log_a

static Ender_Property *ESVG_A_HREF;

typedef struct _Esvg_A
{
	/* properties */
	char *href;
	/* interface */
	/* private */
	char *real_href;
} Esvg_A;

static Esvg_A * _esvg_a_get(Edom_Tag *t)
{
	Esvg_A *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_A)
		return NULL;
	thiz = esvg_element_data_get(t);

	return thiz;
}

static void _esvg_a_renderable_click(Ender_Element *e,
		const char *event_name, void *event_data, void *data)
{
	Esvg_A *thiz = data;
	Esvg_Event_Mouse *ev = event_data;
	Ender_Element *svg;

	DBG("Clicking on '%s'", thiz->real_href);
	svg = esvg_element_topmost_get(e);
	esvg_svg_go_to(svg, thiz->real_href);
}
/*----------------------------------------------------------------------------*
 *                         The Esvg Element interface                         *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_a_child_add(Edom_Tag *t, Edom_Tag *child)
{
	Esvg_A *thiz;
	Esvg_Type type;
	Ender_Element *e;

	thiz = _esvg_a_get(t);
	type = esvg_element_internal_type_get(child);
	if (!esvg_type_is_renderable(type))
		return EINA_TRUE;

	e = esvg_element_ender_get(child);
	/* add the mouse click event */
	ender_event_listener_add(e, "click", _esvg_a_renderable_click, thiz);
	return EINA_TRUE;
}

static Eina_Bool _esvg_a_child_remove(Edom_Tag *t, Edom_Tag *child)
{
	Esvg_A *thiz;
	Esvg_Type type;
	Ender_Element *e;

	thiz = _esvg_a_get(t);
	type = esvg_element_internal_type_get(child);
	if (!esvg_type_is_renderable(type))
		return EINA_TRUE;

	e = esvg_element_ender_get(child);
	/* remove the mouse click event */
	ender_event_listener_remove_full(e, "click", _esvg_a_renderable_click, thiz);
	return EINA_TRUE;
}

static void _esvg_a_free(Edom_Tag *t)
{
	Esvg_A *thiz;

	thiz = _esvg_a_get(t);
	free(thiz);
}

static Eina_Bool _esvg_a_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (strcmp(key, "xlink:href") == 0)
	{
		esvg_a_href_set(e, value);
	}
	else
	{
		return EINA_FALSE;
	}
	/*
	 * a color and a opacity are part of the presentation attributes
	 * and already parsed on the element
	 */
	return EINA_TRUE;
}

static Eina_Bool _esvg_a_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Esvg_Element_Setup_Return _esvg_a_setup(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *parent_context,
		Esvg_Element_Context *context,
		Esvg_Attribute_Presentation *attr,
		Enesim_Error **error)
{
	Esvg_A *thiz;
	Ender_Element *topmost;
	char *real;

	thiz = _esvg_a_get(t);
	esvg_element_internal_topmost_get(t, &topmost);

	/* set the resolved uri */
	if (thiz->real_href)
	{
		free(thiz->href);
		thiz->href = NULL;
	}
	thiz->real_href = esvg_svg_uri_resolve(topmost, thiz->href);
	return EINA_TRUE;
}

static Esvg_Element_Descriptor _descriptor = {
	/* .child_add		= */ _esvg_a_child_add,
	/* .child_remove	= */ _esvg_a_child_remove,
	/* .attribute_get 	= */ _esvg_a_attribute_get,
	/* .cdata_set 		= */ NULL,
	/* .text_set 		= */ NULL,
	/* .text_get 		= */ NULL,
	/* .free 		= */ _esvg_a_free,
	/* .attribute_set 	= */ _esvg_a_attribute_set,
	/* .attribute_animated_fetch = */ NULL,
	/* .initialize 		= */ NULL,
	/* .setup		= */ _esvg_a_setup,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static Edom_Tag * _esvg_a_new(void)
{
	Esvg_A *thiz;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_A));
	if (!thiz) return NULL;

	/* default values */

	t = esvg_element_new(&_descriptor, ESVG_A, thiz);
	return t;
}

static void _esvg_a_href_set(Edom_Tag *t, const char *href)
{
	Esvg_A *thiz;

	thiz = _esvg_a_get(t);
	if (thiz->href)
	{
		free(thiz->href);
		thiz->href = NULL;
	}
	if (href)
		thiz->href = strdup(href);
}

static void _esvg_a_href_get(Edom_Tag *t, const char **href)
{
	Esvg_A *thiz;

	thiz = _esvg_a_get(t);
	if (!href)
		return;
	*href = thiz->href;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool esvg_is_a_internal(Edom_Tag *t)
{
	if (esvg_element_internal_type_get(t) != ESVG_A)
		return EINA_FALSE;
	return EINA_TRUE;
}

/* The ender wrapper */
#define _esvg_a_href_is_set NULL
#define _esvg_a_delete NULL
#include "generated/esvg_generated_a.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ender_Element * esvg_a_new(void)
{
	return ESVG_ELEMENT_NEW("a");
}

EAPI Eina_Bool esvg_is_a(Ender_Element *e)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	return esvg_is_a_internal(t);
}

EAPI void esvg_a_href_set(Ender_Element *e, const char *href)
{
	ender_element_property_value_set(e, ESVG_A_HREF, href, NULL);
}
