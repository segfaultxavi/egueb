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

#ifndef _EGUEB_DOM_NODE_PRIVATE_H_
#define _EGUEB_DOM_NODE_PRIVATE_H_

#include "egueb_dom_feature.h"

typedef struct _Egueb_Dom_Document Egueb_Dom_Document;

struct _Egueb_Dom_Node
{
	Enesim_Object_Instance base;
	/* the tree hierarchy */
	EINA_INLIST;
	Egueb_Dom_Node *parent;
	Eina_Inlist *children;

	/* some attributes */
	Egueb_Dom_Node *owner_document;

	/* namespace handling */
	Egueb_Dom_String *namespace_uri;
	Egueb_Dom_String *prefix;

	/* event related data */
	Eina_Hash *events;
	Eina_List *monitors;

	/* user provided data */
	Eina_Hash *user_data;

	/* features */
	Eina_Hash *features;

	/* is in tree? that is, has an ancestor that has a document
	 * as a parent?
	 */
	Eina_Bool in_tree;

	/* internal */
	int ref;

	/* The freeze count */
	int freezed;

	/* we need this given that we might trigger events and thus do more
	 * refs/unrefs while the node is being destroyed
	 */
	Eina_Bool destroying;
	void *external_data;
};

typedef Eina_Bool (*Egueb_Dom_Node_Child_Appendable)(Egueb_Dom_Node *thiz,
		Egueb_Dom_Node *child);
typedef void (*Egueb_Dom_Node_Clone)(Egueb_Dom_Node *thiz, Eina_Bool live, Eina_Bool deep, Egueb_Dom_Node *clone);
typedef Ender_Item * (*Egueb_Dom_Node_Item_Get)(Egueb_Dom_Node *thiz);

typedef struct _Egueb_Dom_Node_Class
{
	Enesim_Object_Class base;
	Egueb_Dom_Node_Type type;
	Egueb_Dom_Node_Child_Appendable child_appendable;
	Egueb_Dom_Node_Clone clone;
	Egueb_Dom_Node_Item_Get item_get;
} Egueb_Dom_Node_Class;

#define EGUEB_DOM_NODE_DESCRIPTOR egueb_dom_node_descriptor_get()
Enesim_Object_Descriptor * egueb_dom_node_descriptor_get(void);

#define EGUEB_DOM_NODE_CLASS_GET(o) EGUEB_DOM_NODE_CLASS(			\
		(ENESIM_OBJECT_INSTANCE(o))->klass)
#define EGUEB_DOM_NODE_CLASS(k) ENESIM_OBJECT_CLASS_CHECK(k,			\
		Egueb_Dom_Node_Class, EGUEB_DOM_NODE_DESCRIPTOR)
#define EGUEB_DOM_NODE(o) ENESIM_OBJECT_INSTANCE_CHECK(o,			\
		Egueb_Dom_Node, EGUEB_DOM_NODE_DESCRIPTOR)

void egueb_dom_node_document_set(Egueb_Dom_Node *thiz,
		Egueb_Dom_Node *document);
void egueb_dom_node_document_set_recursive(Egueb_Dom_Node *thiz,
		Egueb_Dom_Node *document);
Eina_Bool egueb_dom_node_feature_add(Egueb_Dom_Node *thiz,
		Egueb_Dom_String *name, Egueb_Dom_String *version,
		Egueb_Dom_Feature *feature);
Egueb_Dom_Node * egueb_dom_node_get_ancestor_element(Egueb_Dom_Node *thiz);

#endif
