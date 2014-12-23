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
#include "egueb_svg_point.h"
#include "egueb_svg_path_seg.h"

#include "egueb_dom_value_private.h"
#include "egueb_svg_path_seg_private.h"
#include "egueb_svg_path_seg_list_private.h"
#include "egueb_svg_value_path_seg_list_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                             Value interface                                *
 *----------------------------------------------------------------------------*/
EGUEB_DOM_VALUE_LIST_BOILERPLATE(egueb_svg_path_seg_list,
		egueb_svg_path_seg_descriptor_get());
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
const Egueb_Dom_Value_Descriptor * egueb_svg_value_path_seg_list_descriptor_get(void)
{
	return &_egueb_svg_path_seg_list_descriptor;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
