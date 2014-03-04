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

#ifndef _EGUEB_SMIL_EVENT_H_
#define _EGUEB_SMIL_EVENT_H_

extern Egueb_Dom_String *EGUEB_SMIL_EVENT_BEGIN;
extern Egueb_Dom_String *EGUEB_SMIL_EVENT_END;
extern Egueb_Dom_String *EGUEB_SMIL_EVENT_REPEAT;
extern Egueb_Dom_String *EGUEB_SMIL_EVENT_ETCH;

EAPI Egueb_Dom_Event * egueb_smil_event_new(void);
EAPI void egueb_smil_event_init(Egueb_Dom_Event *e, Egueb_Dom_String *type, int detail);

EAPI Egueb_Dom_Event * egueb_smil_event_etch_new(void);
EAPI void egueb_smil_event_etch_set(Egueb_Dom_Event *e, Etch *etch);
EAPI Etch * egueb_smil_event_etch_get(Egueb_Dom_Event *e);

#endif



