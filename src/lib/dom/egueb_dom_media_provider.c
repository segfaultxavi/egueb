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

#include "egueb_dom_media_provider.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Egueb_Dom_Media_Provider
{
	const Egueb_Dom_Media_Provider_Descriptor *desc;
	Enesim_Renderer *image;
	void *data;
	int ref;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Egueb_Dom_Media_Provider * egueb_dom_media_provider_new(
		const Egueb_Dom_Media_Provider_Descriptor *desc,
		Enesim_Renderer *image, void *data)
{
	Egueb_Dom_Media_Provider *thiz;

	if (!desc) return NULL;
	if (!image) return NULL;

	thiz = calloc(1, sizeof(Egueb_Dom_Media_Provider));
	thiz->image = image;
	thiz->desc = desc;
	thiz->data = data;
	thiz->ref = 1;

	return thiz;
}

EAPI Egueb_Dom_Media_Provider * egueb_dom_media_provider_ref(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return thiz;
	thiz->ref++;
	return thiz;
}

EAPI void egueb_dom_media_provider_unref(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return;
	thiz->ref--;
	if (!thiz->ref)
	{
		if (thiz->desc->destroy)
			thiz->desc->destroy(thiz->data);
		enesim_renderer_unref(thiz->image);
		free(thiz);
	} 
}

EAPI void * egueb_dom_media_provider_data_get(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return NULL;
	return thiz->data;
}

EAPI Enesim_Renderer * egueb_dom_media_provider_renderer_get(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return NULL;
	return enesim_renderer_ref(thiz->image);
}

EAPI void egueb_dom_media_provider_open(Egueb_Dom_Media_Provider *thiz, Egueb_Dom_String *uri)
{
	if (!thiz) return;
	thiz->desc->open(thiz->data, uri);
}

EAPI void egueb_dom_media_provider_close(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return;
	thiz->desc->close(thiz->data);
}

EAPI void egueb_dom_media_provider_play(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return;
	thiz->desc->play(thiz->data);
}

EAPI void egueb_dom_media_provider_pause(Egueb_Dom_Media_Provider *thiz)
{
	if (!thiz) return;
	thiz->desc->pause(thiz->data);
}

