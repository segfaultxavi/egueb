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

#ifndef _EGUEB_CSS_FONT_VARIANT_H_
#define _EGUEB_CSS_FONT_VARIANT_H_

typedef enum _Egueb_Css_Font_Variant {
	EGUEB_CSS_FONT_VARIANT_NORMAL,
	EGUEB_CSS_FONT_VARIANT_SMALL_CAPS,
} Egueb_Css_Font_Variant;

EAPI Eina_Bool egueb_css_font_variant_string_from(Egueb_Css_Font_Variant *thiz, const char *attr_val);
EAPI const char * egueb_css_font_variant_string_to(Egueb_Css_Font_Variant thiz);

#endif
