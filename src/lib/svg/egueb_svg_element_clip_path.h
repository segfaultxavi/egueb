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

#ifndef _EGUEB_SVG_ELEMENT_CLIP_PATH_H_
#define _EGUEB_SVG_ELEMENT_CLIP_PATH_H_

EAPI Egueb_Dom_Node * egueb_svg_element_clip_path_new(void);
EAPI Eina_Bool egueb_svg_element_is_clip_path(Egueb_Dom_Node *n);
EAPI Eina_Error egueb_svg_element_clip_path_units_get(Egueb_Dom_Node *n,
		Egueb_Svg_Referenceable_Units_Animated *units);

#if 0
EAPI void egueb_svg_element_clip_path_units_set(Ender_Element *e, Egueb_Svg_Clip_Path_Units units);
#endif
#endif
