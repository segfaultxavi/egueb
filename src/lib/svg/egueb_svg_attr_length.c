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

#include "egueb_svg_main_private.h"

#include "egueb_dom_attr.h"
#include "egueb_svg_length.h"

#include "egueb_dom_attr_private.h"
#include "egueb_dom_attr_primitive_private.h"
#include "egueb_svg_attr_length.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
EGUEB_DOM_ATTR_PRIMITIVE_BOILERPLATE(Egueb_Svg_Length,
		Egueb_Svg_Attr_Length, egueb_svg_length,
		egueb_svg_attr_length)
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * egueb_svg_attr_length_new(Egueb_Dom_String *name,
		const Egueb_Svg_Length *def, Eina_Bool animatable,
		Eina_Bool stylable, Eina_Bool inheritable)
{
	Egueb_Dom_Node *n;

	n = ENESIM_OBJECT_INSTANCE_NEW(egueb_svg_attr_length);
	egueb_dom_attr_init(n, name, animatable, stylable, inheritable);
	if (def) egueb_dom_attr_set(n, EGUEB_DOM_ATTR_TYPE_DEFAULT, def);
	return n;
}
