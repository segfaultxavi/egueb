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

#include <Eina.h>

#include "Edom.h"
/* TODO
 * + Add a function to iterate over the childs
 * + Add a descriptor function to iterate over the attributes
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Edom_Tag
{
	EINA_INLIST;
	Edom_Tag_Descriptor descriptor;
	Edom_Tag *parent;
	Edom_Tag *topmost;
	Edom_Tag *child;
	void *data;
};

static void _tag_dump(Edom_Tag *thiz, int level)
{
	Edom_Tag *child;
	const char *name;
	int i;

	for (i = 0; i < level; i++)
		printf(" ");
	name = edom_tag_name_get(thiz);
	printf("%s\n", name ? name : "(UNKNOWN)");
	child = thiz->child;
	while (child)
	{
		_tag_dump(child, level + 1);
		child = (Edom_Tag *)EINA_INLIST_GET(child)->next;
	}
}

static Eina_Bool _attributes_set(void *data, const char *key,
		const char *value)
{
	Edom_Tag *thiz = data;

	if (thiz->descriptor.attribute_set)
		return thiz->descriptor.attribute_set(thiz, key, value);
	return EINA_FALSE;
}

static Eina_Bool _attributes_get(void *data, const char *key, char **value)
{
	Edom_Tag *thiz = data;

	if (thiz->descriptor.attribute_get)
		return thiz->descriptor.attribute_get(thiz, key, value);
	return EINA_FALSE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void edom_tag_dump(Edom_Tag *thiz)
{
	if (!thiz) return;
	_tag_dump(thiz, 0);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Edom_Tag * edom_tag_new(Edom_Tag_Descriptor *descriptor,
		void *data)
{
	Edom_Tag *thiz;

	if (!descriptor)
		return NULL;

	thiz = calloc(1, sizeof(Edom_Tag));
	thiz->descriptor = *descriptor;
	thiz->data = data;

	return thiz;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void edom_tag_attributes_from_xml(Edom_Tag *thiz,
		const char *attributes, unsigned int length)
{
	eina_simple_xml_attributes_parse(attributes, length, _attributes_set, thiz);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool edom_tag_attribute_set(Edom_Tag *thiz, const Edom_Attribute *attribute)
{
	return  _attributes_set(thiz, attribute->name, attribute->value);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool edom_tag_attribute_get(Edom_Tag *thiz, Edom_Attribute *attribute)
{
	return  _attributes_get(thiz, attribute->name, &attribute->value);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI char * edom_tag_id_get(Edom_Tag *thiz)
{
	Edom_Attribute attr;

	attr.name = "id";
	if (!edom_tag_attribute_get(thiz, &attr))
		return NULL;
	return attr.value;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void edom_tag_id_set(Edom_Tag *thiz, const char *id)
{
	Edom_Attribute attr;

	attr.name = "id";
	attr.value = id;
	edom_tag_attribute_set(thiz, &attr);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI char * edom_tag_class_get(Edom_Tag *thiz)
{
	Edom_Attribute attr;

	attr.name = "class";
	if (!edom_tag_attribute_get(thiz, &attr))
		return NULL;
	return attr.value;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void edom_tag_class_set(Edom_Tag *thiz, const char *class)
{
	Edom_Attribute attr;

	attr.name = "class";
	attr.value = class;
	edom_tag_attribute_set(thiz, &attr);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * edom_tag_data_get(Edom_Tag *thiz)
{
	return thiz->data;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool edom_tag_child_add(Edom_Tag *thiz, Edom_Tag *child)
{
	Eina_Bool ret = EINA_TRUE;

	if (!child) return EINA_FALSE;

	if (thiz->descriptor.child_add)
		ret = thiz->descriptor.child_add(thiz, child);
	if (ret)
	{
		if (!thiz->child)
			thiz->child = child;
		else
			eina_inlist_append(EINA_INLIST_GET(thiz->child), EINA_INLIST_GET(child));
		child->parent = thiz;
	}

	return ret;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Edom_Tag * edom_tag_child_get(Edom_Tag *thiz)
{
	return thiz->child;
}


/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void edom_tag_child_foreach(Edom_Tag *thiz, Edom_Tag_Foreach foreach, void *data)
{
	Edom_Tag *child;

	if (!foreach) return;

	child = thiz->child;
	while (child)
	{
		Eina_Inlist *il;

		if (!foreach(thiz, child, data))
			break;

		il = EINA_INLIST_GET(child);
		if (!il->next)
			break;
		child = EINA_INLIST_CONTAINER_GET(il->next, Edom_Tag);
	}
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Edom_Tag * edom_tag_next_get(Edom_Tag *thiz)
{
	Eina_Inlist *il;

	il = EINA_INLIST_GET(thiz);
	return EINA_INLIST_CONTAINER_GET(il->next, Edom_Tag);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Edom_Tag * edom_tag_topmost_get(Edom_Tag *thiz)
{
	return thiz->topmost;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Edom_Tag * edom_tag_parent_get(Edom_Tag *thiz)
{
	return thiz->parent;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI const char * edom_tag_name_get(Edom_Tag *thiz)
{
	if (!thiz->descriptor.name_get) return NULL;
	return thiz->descriptor.name_get(thiz);
}

