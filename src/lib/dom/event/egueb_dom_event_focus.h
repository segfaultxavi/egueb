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

#ifndef _EGUEB_DOM_EVENT_FOCUS_H_
#define _EGUEB_DOM_EVENT_FOCUS_H_

#include "egueb_dom_event.h"

/**
 * @file
 * @ender_group{Egueb_Dom_Event_Focus}
 */

/**
 * @defgroup Egueb_Dom_Event_Focus Focus Event
 * TODO make it inherit from ui event
 * @brief Focus Event @ender_inherits{Egueb_Dom_Event}
 * @ingroup Egueb_Dom_Event_UI_Group
 * @{
 */

EAPI extern Egueb_Dom_String *EGUEB_DOM_EVENT_FOCUS_IN;
EAPI extern Egueb_Dom_String *EGUEB_DOM_EVENT_FOCUS_OUT;

EAPI Egueb_Dom_Event * egueb_dom_event_focus_in_new(void);
EAPI Egueb_Dom_Event * egueb_dom_event_focus_out_new(void);

/**
 * @}
 */

#endif

