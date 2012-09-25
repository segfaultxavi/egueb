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
#include "esvg_private_referenceable.h"
#include "esvg_private_types.h"
#include "esvg_private_renderable_container.h"
#include "esvg_private_input.h"

#include "esvg_event.h"
/* The renderable container abstracts the input handling of events
 * to propagate the input events to its children. We need to keep
 * track in a private list of every renderable child and whenever
 * a mousemove happens, also feed events into its own input object
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Esvg_Renderable_Container
{
	Ender_Element *e;
	Esvg_Input *input;
	Eina_List *renderables;
};

static void _esvg_renderable_container_mouse_move(Ender_Element *e,
		const char *event_name, void *event_data, void *data)
{
	Esvg_Renderable_Container *thiz = data;
	Esvg_Event_Mouse *ev = event_data;

	esvg_input_feed_mouse_move(thiz->input, ev->screen_x, ev->screen_y);
}

static Ender_Element * _esvg_renderable_container_element_at(void *data, int x, int y)
{
	Esvg_Renderable_Container *thiz = data;
	Edom_Tag *t;
	Eina_List *l;
	Eina_Rectangle in;

	eina_rectangle_coords_from(&in, x, y, 1, 1);
	EINA_LIST_REVERSE_FOREACH(thiz->renderables, l, t)
	{
		Ender_Element *e;
		Enesim_Renderer *r;
		Eina_Rectangle bounds;

		esvg_renderable_internal_renderer_get(t, &r);
		enesim_renderer_destination_boundings(r, &bounds, 0, 0);
		if (!eina_rectangles_intersect(&bounds, &in))
			continue;

		e = esvg_element_ender_get(t);
		return e;
	}
	return NULL;
}

static Esvg_Input_Descriptor _renderable_container_input_descriptor = {
	/* .element_at 	= */ _esvg_renderable_container_element_at,
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Esvg_Renderable_Container * esvg_renderable_container_new(Ender_Element *e)
{
	Esvg_Renderable_Container *thiz;

	thiz = calloc(1, sizeof(Esvg_Renderable_Container));
	ender_event_listener_add(e, "mousemove", _esvg_renderable_container_mouse_move, thiz);
	/* TODO handle mouse down and mouse up */
	thiz->e = e;
	thiz->input = esvg_input_new(&_renderable_container_input_descriptor, thiz);

	return thiz;
}

void esvg_renderable_container_free(Esvg_Renderable_Container *thiz)
{
	esvg_renderable_container_clear(thiz);
	ender_event_listener_remove(thiz->e, "mousemove", _esvg_renderable_container_mouse_move);
}

void esvg_renderable_container_clear(Esvg_Renderable_Container *thiz)
{
	eina_list_free(thiz->renderables);
}


void esvg_renderable_container_renderable_add(Esvg_Renderable_Container *thiz,
		Edom_Tag *t)
{
	printf("adding element %s\n", edom_tag_name_get(t));
	thiz->renderables = eina_list_append(thiz->renderables, t);
}

void esvg_renderable_container_renderable_remove(Esvg_Renderable_Container *thiz,
		Edom_Tag *t)
{
	thiz->renderables = eina_list_remove(thiz->renderables, t);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

