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
#ifndef _EGUEB_SVG_POINT_PRIVATE_H
#define _EGUEB_SVG_POINT_PRIVATE_H

void egueb_svg_point_interpolate(Egueb_Svg_Point *v,
		Egueb_Svg_Point *a, Egueb_Svg_Point *b, double m,
		Egueb_Svg_Point *add, Egueb_Svg_Point *acc, int mul);
Eina_Bool egueb_svg_point_string_from(Egueb_Svg_Point *thiz, const char *attr);
char * egueb_svg_point_string_to(Egueb_Svg_Point *thiz);

/** @} */

#endif

