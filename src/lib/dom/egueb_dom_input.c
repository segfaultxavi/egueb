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

#include "egueb_dom_private.h"

#include "egueb_dom_string.h"
#include "egueb_dom_main.h"
#include "egueb_dom_node.h"
#include "egueb_dom_input.h"
#include "egueb_dom_event.h"
#include "egueb_dom_event_mouse.h"
#include "egueb_dom_event_mouse_private.h"

/* TODO
 * Right now on every mouse move we go down through the tree to look for
 * node. We better pass the current over node to let the implementation
 * be more clever by just looking if the same node is still inside.
 * The algorithm then can check if the current node is really inside and if
 * not go iterating up if the parent is inside, and then down again if required
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Egueb_Dom_Input
{
	Egueb_Dom_Input_Descriptor *descriptor;
	int downx;
	int downy;
	int x;
	int y;
	void *data;
	Egueb_Dom_Node *over;
	Egueb_Dom_Node *grabbed;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Input * egueb_dom_input_new(Egueb_Dom_Input_Descriptor *descriptor, void *data)
{
	Egueb_Dom_Input *thiz;

	thiz = calloc(1, sizeof(Egueb_Dom_Input));
	thiz->descriptor = descriptor;
	thiz->data = data;
	return thiz;
}

EAPI void egueb_dom_input_free(Egueb_Dom_Input *thiz)
{
	if (thiz->over)
	{
		egueb_dom_node_unref(thiz->over);
		thiz->over = NULL;
	}
	if (thiz->grabbed)
	{
		egueb_dom_node_unref(thiz->grabbed);
		thiz->over = NULL;
	}

	free(thiz);
}

EAPI void egueb_dom_input_feed_mouse_down(Egueb_Dom_Input *thiz, int button)
{
	Egueb_Dom_Event *ev;

	if (!thiz->over)
		return;
	/* store the coordinates where the mouse buton down was done to
	 * trigger the click later
	 */
	thiz->grabbed = egueb_dom_node_ref(thiz->over);
	thiz->downx = thiz->x;
	thiz->downy = thiz->y;

	ev = egueb_dom_event_mouse_new();
	egueb_dom_event_mouse_down_init(ev, thiz->x, thiz->y, thiz->x, thiz->y,
			EINA_FALSE, EINA_FALSE, EINA_FALSE,
			EINA_FALSE, button, 0);
	egueb_dom_node_event_dispatch(thiz->grabbed, ev, NULL, NULL);
}

EAPI void egueb_dom_input_feed_mouse_up(Egueb_Dom_Input *thiz, int button)
{
	Egueb_Dom_Event *ev;

	/* send the event to the grabbed object */
	if (!thiz->grabbed)
		return;

	ev = egueb_dom_event_mouse_new();
	egueb_dom_event_mouse_up_init(ev, thiz->x, thiz->y, thiz->x, thiz->y,
			EINA_FALSE, EINA_FALSE, EINA_FALSE,
			EINA_FALSE, button, 0);
	egueb_dom_node_event_dispatch(thiz->grabbed, ev, NULL, NULL);
	/* in case the down coordinates are the same as the current coordinates
	 * send a click event
	 */
	/* TODO define this threshold somewhere */
	if ((fabs(thiz->downx - thiz->x) < 2) &&
			(fabs(thiz->downy - thiz->y) < 2))
	{
		ev = egueb_dom_event_mouse_new();
		egueb_dom_event_mouse_click_init(ev, thiz->x, thiz->y, thiz->x, thiz->y,
				EINA_FALSE, EINA_FALSE, EINA_FALSE,
				EINA_FALSE, 0, 0);
		egueb_dom_node_event_dispatch(thiz->grabbed, ev, NULL, NULL);
	}
	egueb_dom_node_unref(thiz->grabbed);
	thiz->grabbed = NULL;
}

EAPI void egueb_dom_input_feed_mouse_move(Egueb_Dom_Input *thiz, int x, int y)
{
	Egueb_Dom_Node *n;

	thiz->x = x;
	thiz->y = y;

#if 0
	/* in case of dragging */
	if (thiz->grabbed)
	{
		Eon_Event_Mouse_Move ev;
		double rel_x, rel_y;


		rel_x = thiz->pointer.rel_x + (px - thiz->pointer.x);
		rel_y = thiz->pointer.rel_y + (py - thiz->pointer.y);
		/* we first send the drag start */
		if (!thiz->pointer.dragging)
		{
			Eon_Event_Mouse_Drag_Start ev_ds;

			ev_ds.input = thiz->input;
			ev_ds.x = x;
			ev_ds.y = y;
			ev_ds.rel_x = rel_x;
			ev_ds.rel_y = rel_y;
			egueb_dom_node_event_dispatch(thiz->grabbed,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_DRAG_START], &ev_ds);
			thiz->pointer.dragging = EINA_TRUE;
		}

		ev.input = thiz->input;
		ev.x = x;
		ev.y = y;
		ev.rel_x = rel_x;
		ev.rel_y = rel_y;
		ev.offset_x = offset_x;
		ev.offset_y = offset_y;
		egueb_dom_node_event_dispatch(thiz->grabbed,
					eon_input_event_names[EON_INPUT_EVENT_MOUSE_MOVE], &ev);

		return;
	}
#endif
	/* get the element at x,y */
	n = thiz->descriptor->element_at(thiz->data, x, y);
	//ev.screen_x = x;
	//ev.screen_y = y;

	if (n == thiz->over)
	{
		/* send move */
		if (n)
		{
			Egueb_Dom_Event *ev;

			ev = egueb_dom_event_mouse_new();
			egueb_dom_event_mouse_move_init(ev, x, y, x, y,
					EINA_FALSE, EINA_FALSE, EINA_FALSE,
					EINA_FALSE, 0, 0);
			egueb_dom_node_event_dispatch(egueb_dom_node_ref(n), ev, NULL, NULL);
		}
	}
	else
	{
		/* send out event on i->r */
		if (thiz->over)
		{

			Egueb_Dom_Event *ev;

			//egueb_dom_node_event_dispatch(thiz->over, "mousemove", &ev);
			ev = egueb_dom_event_mouse_new();
			egueb_dom_event_mouse_out_init(ev, x, y, x, y,
					EINA_FALSE, EINA_FALSE, EINA_FALSE,
					EINA_FALSE, 0, 0, n ? egueb_dom_node_ref(n) : NULL);
			egueb_dom_node_event_dispatch(egueb_dom_node_ref(thiz->over), ev, NULL, NULL);
			DBG("Out");
		}
		/* send in event on r */
		if (n)
		{
			Egueb_Dom_Event *ev;

			ev = egueb_dom_event_mouse_new();
			egueb_dom_event_mouse_over_init(ev, x, y, x, y,
					EINA_FALSE, EINA_FALSE, EINA_FALSE,
					EINA_FALSE, 0, 0, thiz->over ? egueb_dom_node_ref(thiz->over) : NULL);
			egueb_dom_node_event_dispatch(egueb_dom_node_ref(n), ev, NULL, NULL);
			//egueb_dom_node_event_dispatch(n, "mousemove", &ev);
			DBG("Over");
		}
	}

	/* update the current over */
	if (thiz->over)
	{
		egueb_dom_node_unref(thiz->over);
	}
	thiz->over = n;
}

EAPI void egueb_dom_input_feed_mouse_wheel(Egueb_Dom_Input *thiz, int deltax, int deltay, int deltaz)
{
	if (!thiz->over)
		return;
	/* TODO create the wheel event */
	/* TODO dispatch it on the over node */
}

