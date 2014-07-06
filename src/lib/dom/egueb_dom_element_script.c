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
#include "egueb_dom_element.h"
#include "egueb_dom_scripter.h"
#include "egueb_dom_scripter.h"
#include "egueb_dom_attr_string.h"
#include "egueb_dom_event_mutation.h"
#include "egueb_dom_event_script.h"
#include "egueb_dom_element_script.h"
#include "egueb_dom_character_data.h"
#include "egueb_dom_element_private.h"
#include "egueb_dom_string_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EGUEB_DOM_ELEMENT_SCRIPT_DESCRIPTOR egueb_dom_element_script_descriptor_get()
#define EGUEB_DOM_ELEMENT_SCRIPT_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k, 		\
		Egueb_Dom_Element_Script_Class, EGUEB_DOM_ELEMENT_SCRIPT_DESCRIPTOR)
#define EGUEB_DOM_ELEMENT_SCRIPT_CLASS_GET(o) EGUEB_DOM_ELEMENT_SCRIPT_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EGUEB_DOM_ELEMENT_SCRIPT(o) ENESIM_OBJECT_INSTANCE_CHECK(o, 		\
		Egueb_Dom_Element_Script, EGUEB_DOM_ELEMENT_SCRIPT_DESCRIPTOR)

typedef struct _Egueb_Dom_Element_Script
{
	Egueb_Dom_Element base;
	Egueb_Dom_Node *content_type;
	Egueb_Dom_String *last_content_type;
	Egueb_Dom_Scripter *scripter;
	void *script;
	Eina_Bool cdata_changed;
} Egueb_Dom_Element_Script;

typedef struct _Egueb_Dom_Element_Script_Class
{
	Egueb_Dom_Element_Class base;
} Egueb_Dom_Element_Script_Class;

static void _egueb_dom_element_script_node_inserted_or_removed_cb(Egueb_Dom_Event *e,
		void *data)
{
	Egueb_Dom_Element_Script *thiz;
	Egueb_Dom_Node *n = data;
	Egueb_Dom_Node *target = NULL;
	Egueb_Dom_Node *related = NULL;
	Egueb_Dom_Node_Type type;

	related = egueb_dom_event_mutation_related_get(e);
	if (related != n)
	{
		goto not_us;
	}

	target = egueb_dom_event_target_get(e);
	type = egueb_dom_node_type_get(target);
	if (type != EGUEB_DOM_NODE_TYPE_CDATA_SECTION)
	{
		goto not_cdata;
	}

	/* finally inform ourselves that the cdata has changed and so, we need
	 * to create a new vm
	 */
	thiz = EGUEB_DOM_ELEMENT_SCRIPT(n);
	thiz->cdata_changed = EINA_TRUE;
not_cdata:
	egueb_dom_node_unref(target);
not_us:
	egueb_dom_node_unref(related);
}
/*----------------------------------------------------------------------------*
 *                             Element interface                              *
 *----------------------------------------------------------------------------*/
static Eina_Bool _egueb_dom_element_script_process(Egueb_Dom_Element *e)
{
	Egueb_Dom_Element_Script *thiz;
	Egueb_Dom_String *content_type = NULL;
	Eina_Bool content_type_changed = EINA_FALSE;

	thiz = EGUEB_DOM_ELEMENT_SCRIPT(e);

	egueb_dom_attr_final_get(thiz->content_type, &content_type);
	if (!egueb_dom_string_is_equal(content_type, thiz->last_content_type))
		content_type_changed = EINA_TRUE;

	if (!thiz->cdata_changed && !content_type_changed)
		goto done;

	if (thiz->scripter)
	{
		ERR("Impossible to re-generate the whole scripting VM");
	}
	else
	{
		Egueb_Dom_Event *ev;
		Egueb_Dom_String *type;

		ERR("Processing the script");
		/* TODO check the attribute */
		type = egueb_dom_string_new_with_string("application/ecmascript");
		ev = egueb_dom_event_script_new(type);
		egueb_dom_node_event_dispatch(EGUEB_DOM_NODE(e), egueb_dom_event_ref(ev), NULL, NULL);
		/* instantiate the vm */
		thiz->scripter = egueb_dom_event_script_scripter_get(ev);
		egueb_dom_event_unref(ev);

		if (thiz->scripter)
		{
			Egueb_Dom_Node *cdata;
			Egueb_Dom_String *cdata_txt;

			cdata = egueb_dom_node_child_first_get(EGUEB_DOM_NODE(e));
			cdata_txt = egueb_dom_character_data_data_get(cdata);
			/* compile the script */
			egueb_dom_scripter_load(thiz->scripter, cdata_txt, &thiz->script);
			/* TODO add the global variables */
			/* run it */
			egueb_dom_scripter_script_run(thiz->scripter,thiz->script);
			egueb_dom_string_unref(cdata_txt);
		}
	}

	thiz->cdata_changed = EINA_FALSE;
done:
	egueb_dom_string_unref(content_type);
	return EINA_TRUE;
}

static Egueb_Dom_String * _egueb_dom_element_script_tag_name_get(Egueb_Dom_Element *e)
{
	return egueb_dom_string_ref(EGUEB_DOM_NAME_SCRIPT);
}
/*----------------------------------------------------------------------------*
 *                               Node interface                               *
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 *                              Object interface                              *
 *----------------------------------------------------------------------------*/
ENESIM_OBJECT_INSTANCE_BOILERPLATE(EGUEB_DOM_ELEMENT_DESCRIPTOR,
		Egueb_Dom_Element_Script, Egueb_Dom_Element_Script_Class,
		egueb_dom_element_script);

static void _egueb_dom_element_script_class_init(void *k)
{
	Egueb_Dom_Element_Class *klass;

	klass = EGUEB_DOM_ELEMENT_CLASS(k);
	klass->tag_name_get = _egueb_dom_element_script_tag_name_get;
	klass->process = _egueb_dom_element_script_process;
}

static void _egueb_dom_element_script_instance_init(void *o)
{
	Egueb_Dom_Element_Script *thiz;
	Egueb_Dom_Node *n;

	thiz = EGUEB_DOM_ELEMENT_SCRIPT(o);
	n = EGUEB_DOM_NODE(o);
	/* add the attributes */
	thiz->content_type = egueb_dom_attr_string_new(
			egueb_dom_string_ref(EGUEB_DOM_NAME_CONTENT_TYPE),
			NULL, EINA_FALSE, EINA_FALSE, EINA_FALSE);
	egueb_dom_element_attribute_add(n, egueb_dom_node_ref(thiz->content_type), NULL);

	/* add the events */
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_INSERTED,
			_egueb_dom_element_script_node_inserted_or_removed_cb,
			EINA_TRUE, thiz);
	egueb_dom_node_event_listener_add(n,
			EGUEB_DOM_EVENT_MUTATION_NODE_REMOVED,
			_egueb_dom_element_script_node_inserted_or_removed_cb,
			EINA_TRUE, thiz);
}

static void _egueb_dom_element_script_instance_deinit(void *o)
{
	Egueb_Dom_Element_Script *thiz;

	thiz = EGUEB_DOM_ELEMENT_SCRIPT(o);
	if (thiz->scripter)
	{
		egueb_dom_scripter_script_free(thiz->scripter, thiz->script);
	}
	egueb_dom_node_unref(thiz->content_type);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eina_Bool egueb_dom_element_is_script(Egueb_Dom_Node *n)
{
	if (!n) return EINA_FALSE;
	if (!enesim_object_instance_inherits(ENESIM_OBJECT_INSTANCE(n),
			EGUEB_DOM_ELEMENT_SCRIPT_DESCRIPTOR))
		return EINA_FALSE;
	return EINA_TRUE;
}

EAPI Egueb_Dom_Node * egueb_dom_element_script_new(void)
{
	Egueb_Dom_Element_Script *thiz;

	thiz = ENESIM_OBJECT_INSTANCE_NEW(egueb_dom_element_script);
	return EGUEB_DOM_NODE(thiz);
}
