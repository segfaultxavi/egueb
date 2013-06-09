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

#ifndef _EGUEB_SMIL_TIMING_H_
#define _EGUEB_SMIL_TIMING_H_

typedef enum _Egueb_Smil_Timing_Type
{
	EGUEB_SMIL_TIMING_TYPE_OFFSET,
	EGUEB_SMIL_TIMING_TYPE_SYNCBASE,
	EGUEB_SMIL_TIMING_TYPE_EVENT,
	EGUEB_SMIL_TIMING_TYPE_REPEAT,
	EGUEB_SMIL_TIMING_TYPE_ACCESS_KEY,
	EGUEB_SMIL_TIMING_TYPE_WALLCLOCK,
	EGUEB_SMIL_TIMING_TYPE_INDEFINITE,
	EGUEB_SMIL_TIMING_TYPE_TYPES,
} Egueb_Smil_Timing_Type;

typedef struct _Egueb_Smil_Timing
{
	Egueb_Smil_Timing_Type type;
	char *id;
	char *event;
	int repeat;
	int key;
	int64_t offset;
} Egueb_Smil_Timing;

EAPI const Egueb_Dom_Value_Descriptor * egueb_smil_timing_descriptor_get(void);
EAPI Eina_Bool egueb_smil_timing_string_from(Egueb_Smil_Timing *thiz, const char *s);
EAPI char * egueb_smil_timing_string_to(Egueb_Smil_Timing *thiz);
EAPI void egueb_smil_timing_copy(const Egueb_Smil_Timing *thiz, Egueb_Smil_Timing *copy);
EAPI void egueb_smil_timing_reset(Egueb_Smil_Timing *thiz);

#endif
