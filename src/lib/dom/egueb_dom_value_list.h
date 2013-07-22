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

#ifndef _EGUEB_DOM_VALUE_LIST_H_
#define _EGUEB_DOM_VALUE_LIST_H_

EAPI void egueb_dom_value_list_data_from(Egueb_Dom_Value *v,
		Egueb_Dom_Value_Data *data);
EAPI void egueb_dom_value_list_data_to(Egueb_Dom_Value *v,
		Egueb_Dom_Value_Data *data);
EAPI void egueb_dom_value_list_free(Egueb_Dom_Value *v);
EAPI void egueb_dom_value_list_copy(const Egueb_Dom_Value *v,
		Egueb_Dom_Value *copy, Eina_Bool content);
EAPI void egueb_dom_list_interpolate(Egueb_Dom_Value *v,
		Egueb_Dom_Value *a, Egueb_Dom_Value *b, double m,
		Egueb_Dom_Value *add, Egueb_Dom_Value *acc, int mul);

#endif
