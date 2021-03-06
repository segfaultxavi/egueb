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

#include "egueb_smil_main.h"
#include "egueb_smil_additive.h"

#include "egueb_dom_attr_private.h"
#include "egueb_dom_attr_basic_private.h"
#include "egueb_smil_value_additive_private.h"
#include "egueb_smil_attr_additive_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
EGUEB_DOM_ATTR_BASIC_BOILERPLATE(Egueb_Smil_Additive,
		Egueb_Smil_Attr_Additive, egueb_smil_value_additive,
		egueb_smil_attr_additive)
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * egueb_smil_attr_additive_new(void)
{
	Egueb_Dom_Node *n;

	n = ENESIM_OBJECT_INSTANCE_NEW(egueb_smil_attr_additive);
	egueb_dom_attr_init(n, egueb_dom_string_ref(EGUEB_SMIL_ADDITIVE),
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	egueb_dom_attr_set(n, EGUEB_DOM_ATTR_TYPE_DEFAULT,
			EGUEB_SMIL_ADDITIVE_REPLACE);

	return n;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
