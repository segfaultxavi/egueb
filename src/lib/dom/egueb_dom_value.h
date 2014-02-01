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

#ifndef _EGUEB_DOM_VALUE_H_
#define _EGUEB_DOM_VALUE_H_

#include "egueb_dom_string.h"

typedef struct _Egueb_Dom_Value_Descriptor Egueb_Dom_Value_Descriptor;
typedef struct _Egueb_Dom_Value Egueb_Dom_Value;

typedef enum _Egueb_Dom_Value_Data_Type
{
	EGUEB_DOM_VALUE_DATA_TYPE_INT32,
	EGUEB_DOM_VALUE_DATA_TYPE_INT64,
	EGUEB_DOM_VALUE_DATA_TYPE_DOUBLE,
	EGUEB_DOM_VALUE_DATA_TYPE_PTR,
} Egueb_Dom_Value_Data_Type;

typedef union _Egueb_Dom_Value_Data
{
		int32_t i32;
		int64_t i64;
		double d;
		void *ptr;
} Egueb_Dom_Value_Data;

typedef void (*Egueb_Dom_Value_Data_From)(Egueb_Dom_Value *thiz, Egueb_Dom_Value_Data *data);
typedef void (*Egueb_Dom_Value_Data_To)(Egueb_Dom_Value *thiz, Egueb_Dom_Value_Data *data);

typedef void (*Egueb_Dom_Value_Init)(Egueb_Dom_Value *thiz);
typedef void (*Egueb_Dom_Value_Free)(Egueb_Dom_Value *thiz);
typedef void (*Egueb_Dom_Value_Copy)(const Egueb_Dom_Value *thiz,
		Egueb_Dom_Value *copy, Eina_Bool content);

typedef char * (*Egueb_Dom_Value_String_To)(const Egueb_Dom_Value *thiz);
typedef Eina_Bool (*Egueb_Dom_Value_String_From)(Egueb_Dom_Value *thiz,
		const char *str);

typedef void (*Egueb_Dom_Value_Interpolate)(Egueb_Dom_Value *thiz,
		Egueb_Dom_Value *a, Egueb_Dom_Value *b, double m,
		Egueb_Dom_Value *add, Egueb_Dom_Value *acc, int mul);

struct _Egueb_Dom_Value_Descriptor {
	Egueb_Dom_Value_Data_From data_from;
	Egueb_Dom_Value_Data_Type data_from_type;

	Egueb_Dom_Value_Data_To data_to;
	Egueb_Dom_Value_Data_Type data_to_type;

	Egueb_Dom_Value_Init init;
	Egueb_Dom_Value_Free free;
	Egueb_Dom_Value_Copy copy;

	Egueb_Dom_Value_String_To string_to;
	Egueb_Dom_Value_String_From string_from;

	Egueb_Dom_Value_Interpolate interpolate;
};

struct _Egueb_Dom_Value
{
	const Egueb_Dom_Value_Descriptor *descriptor;
	Eina_Bool owned;
	Egueb_Dom_Value_Data data;
};

#define EGUEB_DOM_VALUE_INIT { 0, EINA_FALSE,  { 0 } }

EAPI void egueb_dom_value_init(Egueb_Dom_Value *thiz,
		const Egueb_Dom_Value_Descriptor *descriptor);
EAPI void egueb_dom_value_reset(Egueb_Dom_Value *thiz);
EAPI void egueb_dom_value_copy(const Egueb_Dom_Value *thiz, Egueb_Dom_Value *copy, Eina_Bool content);
EAPI Eina_Bool egueb_dom_value_string_to(const Egueb_Dom_Value *thiz,
		Egueb_Dom_String **str);
EAPI Eina_Bool egueb_dom_value_string_from(Egueb_Dom_Value *thiz,
		Egueb_Dom_String *str);
EAPI void egueb_dom_value_interpolate(Egueb_Dom_Value *thiz,
		Egueb_Dom_Value *a, Egueb_Dom_Value *b,
		double m, Egueb_Dom_Value *add, Egueb_Dom_Value *acc,
		int mul);

/* primitive (structs) helpers */
EAPI void egueb_dom_value_primitive_data_from(Egueb_Dom_Value *v, Egueb_Dom_Value_Data *data);
EAPI void egueb_dom_value_primitive_data_to(Egueb_Dom_Value *v, Egueb_Dom_Value_Data *data);
/* enum helpers */
EAPI void egueb_dom_value_enum_interpolate(Egueb_Dom_Value *v,
		Egueb_Dom_Value *a, Egueb_Dom_Value *b, double m,
		Egueb_Dom_Value *add, Egueb_Dom_Value *acc, int mul);


#endif
