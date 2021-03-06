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

#ifndef _EGUEB_DOM_EVENT_KEYBOARD_PRIVATE_H_
#define _EGUEB_DOM_EVENT_KEYBOARD_PRIVATE_H_

Egueb_Dom_Event * egueb_dom_event_key_down_new(Egueb_Dom_String *key,
		Egueb_Dom_Key_Location location,
		Eina_Bool alt_key, Eina_Bool ctrl_key, Eina_Bool shift_key,
		Eina_Bool meta_key);
Egueb_Dom_Event * egueb_dom_event_key_up_new(Egueb_Dom_String *key,
		Egueb_Dom_Key_Location location,
		Eina_Bool alt_key, Eina_Bool ctrl_key, Eina_Bool shift_key,
		Eina_Bool meta_key);

#endif
