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
#include "egueb_smil_private.h"
#include "egueb_smil_key_spline_private.h"
#include "egueb_dom_value_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static char * egueb_smil_value_key_spline_string_to(Egueb_Smil_Key_Spline *value)
{
	return NULL;
}

static Eina_Bool egueb_smil_value_key_spline_string_from(
		Egueb_Smil_Key_Spline *value, const char *str)
{
	return egueb_smil_key_spline_string_from(value, str);
}


static void egueb_smil_value_key_spline_interpolate(Egueb_Smil_Key_Spline *v,
		Egueb_Smil_Key_Spline *a, Egueb_Smil_Key_Spline *b, double m,
		Egueb_Smil_Key_Spline *add, Egueb_Smil_Key_Spline *acc, int mul)
{
	ERR("Not implemented");
}

EGUEB_DOM_VALUE_PRIMITIVE_SIMPLE_BOILERPLATE(egueb_smil_value_key_spline,
		Egueb_Smil_Key_Spline);
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
const Egueb_Dom_Value_Descriptor * egueb_smil_value_key_spline_descriptor_get(void)
{
	return &_egueb_smil_value_key_spline_descriptor;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
