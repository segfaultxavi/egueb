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
#ifndef _EGUEB_CSS_STYLE_PRIVATE_H_
#define _EGUEB_CSS_STYLE_PRIVATE_H_

typedef struct _Egueb_Css_Engine_Style Egueb_Css_Engine_Style;

Egueb_Css_Engine_Style * egueb_css_engine_style_new(void);
Egueb_Css_Engine_Style * egueb_css_engine_style_load_from_file(const char *file);
Egueb_Css_Engine_Style * egueb_css_engine_style_load_from_content(const char *content, size_t len);
void egueb_css_engine_style_free(Egueb_Css_Engine_Style *thiz);

void egueb_css_engine_style_inline_apply(Egueb_Dom_Node *n, const char *style);
void egueb_css_engine_style_inline_unapply(Egueb_Dom_Node *n, const char *style);
void egueb_css_engine_style_rule_add(Egueb_Css_Engine_Style *thiz, Egueb_Css_Engine_Rule *r);
void egueb_css_engine_style_apply(Egueb_Css_Engine_Style *thiz, Egueb_Dom_Node *n);
void egueb_css_engine_style_unapply(Egueb_Css_Engine_Style *thiz, Egueb_Dom_Node *n);

struct _Egueb_Css_Engine_Style
{
	Eina_List *rules;
};

#endif

