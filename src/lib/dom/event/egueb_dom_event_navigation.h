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
#ifndef _EGUEB_DOM_EVENT_NAVIGATION_H_
#define _EGUEB_DOM_EVENT_NAVIGATION_H_

/**
 * @defgroup Egueb_Dom_Event_Multimedia_Group Multimedia Event
 * @brief Multimedia Event
 * @ingroup Egueb_Dom_Event_Group
 * @{
 */

EAPI extern Egueb_Dom_String *EGUEB_DOM_EVENT_NAVIGATION_GO_TO;

EAPI Egueb_Dom_Event * egueb_dom_event_navigation_go_to_new(
		Egueb_Dom_Uri *uri);
EAPI void egueb_dom_event_navigation_uri_get(Egueb_Dom_Event *e, Egueb_Dom_Uri *uri);

/**
 * @}
 */

#endif
