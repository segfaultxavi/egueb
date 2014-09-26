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
#include "egueb_css_private.h"

#include "egueb_css_main.h"
#include "egueb_css_font_style.h"
#include "egueb_css_font_style_private.h"
#include "egueb_dom_attr_basic_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Value interface                                *
 *----------------------------------------------------------------------------*/
EGUEB_DOM_VALUE_ENUM_BOILERPLATE(egueb_css_font_style, Egueb_Css_Font_Style);
/*----------------------------------------------------------------------------*
 *                           Attribute interface                              *
 *----------------------------------------------------------------------------*/
EGUEB_DOM_ATTR_BASIC_BOILERPLATE(Egueb_Css_Font_Style,
		Egueb_Css_Font_Style_Attr, egueb_css_font_style_value,
		egueb_css_font_style_attr)
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool egueb_css_font_style_string_from(Egueb_Css_Font_Style *thiz,
		const char *str)
{
	Eina_Bool ret = EINA_TRUE;

	if (!strcmp(str, "normal"))
		*thiz = EGUEB_CSS_FONT_STYLE_NORMAL;
	else if (!strcmp(str, "italic"))
		*thiz = EGUEB_CSS_FONT_STYLE_ITALIC;
	else if (!strcmp(str, "oblique"))
		*thiz = EGUEB_CSS_FONT_STYLE_OBLIQUE;
	else
		return EINA_FALSE;
	return ret;
}

char * egueb_css_font_style_string_to(Egueb_Css_Font_Style thiz)
{
	switch (thiz)
	{
		case EGUEB_CSS_FONT_STYLE_NORMAL:
		return strdup("normal");

		case EGUEB_CSS_FONT_STYLE_ITALIC:
		return strdup("italic");

		case EGUEB_CSS_FONT_STYLE_OBLIQUE:
		return strdup("oblique");

		default:
		return NULL;
	}
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Node * egueb_css_font_style_attr_new(
		Eina_Bool animatable, Eina_Bool stylable,
		Eina_Bool inheritable)
{
	Egueb_Dom_Node *n;

	n = ENESIM_OBJECT_INSTANCE_NEW(egueb_css_font_style_attr);
	egueb_dom_attr_init(n, egueb_dom_string_ref(EGUEB_CSS_NAME_FONT_STYLE),
			animatable, stylable, inheritable);
	return n;
}

EAPI const Egueb_Dom_Value_Descriptor *
		egueb_css_font_style_value_descriptor_get(void)
{
	return &_egueb_css_font_style_descriptor;
}
