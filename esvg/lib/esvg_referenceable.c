/* Esvg - SVG
 * Copyright (C) 2011 Jorge Luis Zapata, Vincent Torri
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "esvg_private_main.h"
#include "esvg_private_attribute_presentation.h"
#include "esvg_private_context.h"
#include "esvg_private_element.h"
#include "esvg_private_referenceable.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_REFERENCEABLE_MAGIC_CHECK(d) \
	do {\
		if (!EINA_MAGIC_CHECK(d, ESVG_REFERENCEABLE_MAGIC))\
			EINA_MAGIC_FAIL(d, ESVG_REFERENCEABLE_MAGIC);\
	} while(0)

typedef struct _Esvg_Referenceable_Descriptor_Internal
{
	Edom_Tag_Free free;
	Esvg_Referenceable_Setup setup;
	Esvg_Referenceable_Renderer_New renderer_new;
} Esvg_Referenceable_Descriptor_Internal;

typedef struct _Esvg_Referenceable
{
	EINA_MAGIC
	/* properties */
	/* interface */
	Esvg_Referenceable_Descriptor_Internal descriptor;
	/* private */
	/* TODO remove this */
	Enesim_Renderer *current;
	/* TODO use this */
	Eina_List *references;
	void *data;
} Esvg_Referenceable;

typedef struct _Esvg_Referenceable_Reference
{
	Edom_Tag *t;
	Ender_Element *referencer;
	/* instead of the renderer this could be just a void *
	 * where the child classes should fill this with its
	 * own data
	 */
	void *data;
} Esvg_Referenceable_Reference;

static Esvg_Referenceable * _esvg_referenceable_get(Edom_Tag *t)
{
	Esvg_Referenceable *thiz;

	thiz = esvg_element_data_get(t);
	ESVG_REFERENCEABLE_MAGIC_CHECK(thiz);

	return thiz;
}

/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                         The Esvg Element interface                         *
 *----------------------------------------------------------------------------*/
static void _esvg_referenceable_free(Edom_Tag *t)
{
	Esvg_Referenceable *thiz;

	thiz = _esvg_referenceable_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}

static Esvg_Element_Setup_Return _esvg_referenceable_setup(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *parent_context,
		Esvg_Element_Context *context,
		Esvg_Attribute_Presentation *attr,
		Enesim_Error **error)
{
	Esvg_Referenceable *thiz;

	thiz = _esvg_referenceable_get(t);
	/* a referenceable should always get its attributes from its
	 * parent tree
	 */
	if (!thiz->current)
		return ESVG_SETUP_FAILED;

	if (thiz->descriptor.setup)
		return thiz->descriptor.setup(t, c, context, attr, thiz->current, error);
	return ESVG_SETUP_OK;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool esvg_is_referenceable_internal(Edom_Tag *t)
{
	Esvg_Referenceable *thiz;
	Eina_Bool ret;

	if (!esvg_is_element_internal(t))
		return EINA_FALSE;
	thiz = esvg_element_data_get(t);
	ret = EINA_MAGIC_CHECK(thiz, ESVG_REFERENCEABLE_MAGIC);

	return ret;
}

/* remove this */
Enesim_Renderer * esvg_referenceable_renderer_new(Edom_Tag *t)
{
	Esvg_Referenceable *thiz;
	Enesim_Renderer *r;

	printf("entering\n");
	thiz = _esvg_referenceable_get(t);
	printf("entering\n");
	r = thiz->descriptor.renderer_new(t);
	printf("renderer new!!!! %p\n", r);

	return r;
}

void esvg_referenceable_propagate(Edom_Tag *t, Ender_Element *e)
{
	/* get the final state */
	/* call the propagate interface */
}

void esvg_referenceable_reference_add(Edom_Tag *t, Ender_Element *e)
{
	/* get the renderer */
	/* create the reference struct */
	/* trigger the add interface */
	/* return the renderer ? */
}

void esvg_referenceable_reference_remove(Edom_Tag *t, Ender_Element *e)
{
	/* trigger the remove interface */
}

void esvg_referenceable_reference_foreach(Edom_Tag *t)
{
	/* iterate over the list of references */
	/* and call the function */
}

void esvg_referenceable_renderer_set(Edom_Tag *t, Enesim_Renderer *r)
{
	Esvg_Referenceable *thiz;

	if (!r) return;
	thiz = _esvg_referenceable_get(t);
	thiz->current = r;
}

/* The ender wrapper */
#include "generated/esvg_generated_referenceable.c"

void * esvg_referenceable_data_get(Edom_Tag *t)
{
	Esvg_Referenceable *thiz;

	thiz = _esvg_referenceable_get(t);
	return thiz->data;
}

Edom_Tag * esvg_referenceable_new(Esvg_Referenceable_Descriptor *descriptor, Esvg_Type type,
		void *data)
{
	Esvg_Referenceable *thiz;
	Esvg_Element_Descriptor pdescriptor;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_Referenceable));
	if (!thiz) return NULL;

	EINA_MAGIC_SET(thiz, ESVG_REFERENCEABLE_MAGIC);
	thiz->data = data;
	/* our own descriptor */
	thiz->descriptor.setup = descriptor->setup;
	thiz->descriptor.renderer_new = descriptor->renderer_new;
	/* default values */

	pdescriptor.child_add = descriptor->child_add;
	pdescriptor.child_remove = descriptor->child_remove;
	pdescriptor.attribute_set = descriptor->attribute_set;
	pdescriptor.attribute_get = descriptor->attribute_get;
	pdescriptor.cdata_set = descriptor->cdata_set;
	pdescriptor.text_set = descriptor->text_set;
	pdescriptor.free = _esvg_referenceable_free;
	pdescriptor.initialize = descriptor->initialize;
	pdescriptor.setup = _esvg_referenceable_setup;

	t = esvg_element_new(&pdescriptor, type, thiz);

	return t;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_is_referenceable(Ender_Element *e)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	return esvg_is_referenceable_internal(t);
}
