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
#ifndef _EGUEB_DOM_ATTR_H_
#define _EGUEB_DOM_ATTR_H_

#include "egueb_dom_node.h"
#include "egueb_dom_value.h"

typedef enum _Egueb_Dom_Attr_Flag
{
	EGUEB_DOM_ATTR_FLAG_STYLABLE	= 1 << 0,
	EGUEB_DOM_ATTR_FLAG_ANIMATABLE	= 1 << 1,
	EGUEB_DOM_ATTR_FLAG_INHERITABLE	= 1 << 2,
} Egueb_Dom_Attr_Flag;

static inline int egueb_dom_attr_flag_mask_generate(Eina_Bool animatable,
		Eina_Bool stylable, Eina_Bool inheritable)
{
	return inheritable << 2 | animatable << 1 | stylable << 0;
}

typedef enum _Egueb_Dom_Attr_Type
{
	EGUEB_DOM_ATTR_TYPE_BASE 		= 1 << 0,
	EGUEB_DOM_ATTR_TYPE_DEFAULT		= 1 << 1,
	EGUEB_DOM_ATTR_TYPE_ANIMATED		= 1 << 2,
	EGUEB_DOM_ATTR_TYPE_STYLED 		= 1 << 3,
} Egueb_Dom_Attr_Type;

/* function definition to get a property from an element instance */
typedef void (*Egueb_Dom_Attr_Fetch)(Egueb_Dom_Node *n, Egueb_Dom_Node **p);

EAPI void egueb_dom_attr_init(Egueb_Dom_Node *n, Egueb_Dom_String *name,
		Eina_Bool animatable, Eina_Bool stylable, Eina_Bool inheritable);
EAPI Eina_Error egueb_dom_attr_name_get(Egueb_Dom_Node *n,
		Egueb_Dom_String **name);

EAPI Eina_Bool egueb_dom_attr_is_stylable(Egueb_Dom_Node *n);
EAPI Eina_Bool egueb_dom_attr_is_animatable(Egueb_Dom_Node *n);
EAPI Eina_Bool egueb_dom_attr_is_inheritable(Egueb_Dom_Node *n);
EAPI Eina_Bool egueb_dom_attr_is_set(Egueb_Dom_Node *n);
EAPI Eina_Bool egueb_dom_attr_type_is_set(Egueb_Dom_Node *n,
		Egueb_Dom_Attr_Type type);
EAPI Eina_Bool egueb_dom_attr_has_value(Egueb_Dom_Node *n);
EAPI void egueb_dom_attr_inherit(Egueb_Dom_Node *n,
		Egueb_Dom_Attr_Type type);
EAPI Eina_Bool egueb_dom_attr_value_get(Egueb_Dom_Node *n,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value);
EAPI Eina_Bool egueb_dom_attr_value_set(Egueb_Dom_Node *n,
		Egueb_Dom_Attr_Type type, Egueb_Dom_Value *value);

EAPI Eina_Error egueb_dom_attr_get(Egueb_Dom_Node *n, int prop_mask,
	...);
EAPI Eina_Error egueb_dom_attr_set(Egueb_Dom_Node *n, int prop_mask,
	...);
EAPI void egueb_dom_attr_inheritable_process(Egueb_Dom_Node *n,
			Egueb_Dom_Node *rel);

EAPI Eina_Bool egueb_dom_attr_final_value_get(Egueb_Dom_Node *n,
		Egueb_Dom_Value *value);
EAPI Eina_Bool egueb_dom_attr_final_get_va(Egueb_Dom_Node *n, va_list args);
EAPI Eina_Bool egueb_dom_attr_final_get(Egueb_Dom_Node *n, ...);

EAPI Eina_Bool egueb_dom_attr_string_set(Egueb_Dom_Node *attr,
		Egueb_Dom_Attr_Type type, Egueb_Dom_String *str);
EAPI Eina_Bool egueb_dom_attr_string_get(Egueb_Dom_Node *attr,
		Egueb_Dom_Attr_Type type, Egueb_Dom_String **str);

EAPI void egueb_dom_attr_inherited_get(Egueb_Dom_Node *attr,
		Egueb_Dom_Node **inherited);

#define EGUEB_DOM_ATTR_FETCH_DEFINE(prefix, type, name)				\
	static void _##prefix##_##name##_fetch(Egueb_Dom_Node *n, 		\
			Egueb_Dom_Node **a)					\
	{									\
		type *thiz;							\
		thiz = ENESIM_OBJECT_INSTANCE_CHECK(n,type,			\
				prefix##_descriptor_get());			\
		*a = thiz->name;						\
	}
#endif
