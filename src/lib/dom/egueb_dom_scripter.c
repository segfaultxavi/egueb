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
#include "egueb_dom_private.h"
#include "egueb_dom_string.h"
#include "egueb_dom_main.h"
#include "egueb_dom_node_list.h"
#include "egueb_dom_node_map_named.h"
#include "egueb_dom_node.h"

#include "egueb_dom_scripter.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Egueb_Dom_Scripter
{
	Egueb_Dom_Node *doc;
	const Egueb_Dom_Scripter_Descriptor *d;
	void *prv;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Scripter * egueb_dom_scripter_new(const Egueb_Dom_Scripter_Descriptor *d)
{
	Egueb_Dom_Scripter *thiz;
	void *prv;

	if (!d) return NULL;
	if (!d->new) return NULL;

	prv = d->new();
	if (!prv) return NULL;

	thiz = calloc(1, sizeof(Egueb_Dom_Scripter));
	thiz->d = d;
	thiz->prv = prv;
	return thiz;
}

EAPI void * egueb_dom_scripter_data_get(Egueb_Dom_Scripter *thiz)
{
	if (!thiz) return NULL;
	return thiz->prv;
}

EAPI Eina_Bool egueb_dom_scripter_gloabal_add(Egueb_Dom_Scripter *thiz, Egueb_Dom_String *name, void *obj, const char *type)
{
	if (!thiz) return EINA_FALSE;
	if (!name) return EINA_FALSE;
	if (!thiz->d->global_add) return EINA_FALSE;

	return thiz->d->global_add(thiz->prv, name, obj, type);
}

EAPI Eina_Bool egueb_dom_scripter_load(Egueb_Dom_Scripter *thiz, Egueb_Dom_String *s, void **obj)
{
	if (!thiz) return EINA_FALSE;
	if (!thiz->d->load) return EINA_FALSE;

	return thiz->d->load(thiz->prv, s, obj);
}

EAPI Eina_Bool egueb_dom_scripter_run(Egueb_Dom_Scripter *thiz, void *obj, Egueb_Dom_Node *ctx)
{
	if (!thiz) return EINA_FALSE;
	if (!thiz->d->run) return EINA_FALSE;

	return thiz->d->run(thiz->prv, obj, ctx);
}
