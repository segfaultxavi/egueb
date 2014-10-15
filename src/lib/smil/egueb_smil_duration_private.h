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

#ifndef _EGUEB_SMIL_DURATION_PRIVATE_H_
#define _EGUEB_SMIL_DURATION_PRIVATE_H_

Eina_Bool egueb_smil_duration_string_from(Egueb_Smil_Duration *d, const char *attr);
char * egueb_smil_duration_string_to(Egueb_Smil_Duration *d);
void egueb_smil_duration_interpolate(Egueb_Smil_Duration *v,
		Egueb_Smil_Duration *a, Egueb_Smil_Duration *b, double m,
		Egueb_Smil_Duration *add, Egueb_Smil_Duration *acc, int mul);

#endif
