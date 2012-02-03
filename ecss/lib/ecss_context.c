/* Ecss - CSS
 * Copyright (C) 2011 Jorge Luis Zapata
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
#include "Ecss.h"
#include "ecss_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/* Keep track of the selector state */
typedef struct _Ecss_State_Selector
{
	Ecss_Rule *r;
	Ecss_Selector *s; /* the current selector in case we have a compound selector */
	int child;
	int sibling;
} Ecss_State_Selector;

typedef struct _Ecss_State
{
	Eina_List *active;
	Eina_List *inactive;
} Ecss_State;

static void _element_rule_apply(Ecss_Context *c, Ecss_Element *e, Ecss_Rule *r)
{
	Ecss_Declaration *d;
	Eina_List *l;

	EINA_LIST_FOREACH(r->declarations, l, d)
	{
		c->property_set(e, d->property, d->value);
	}
}

static Eina_Bool _element_matches(Ecss_Context *c, Ecss_Element *e, Ecss_State_Selector *ss)
{
	Ecss_Rule *r = ss->r;
	Ecss_Selector *s = ss->s;
	Eina_Bool ret = EINA_FALSE;

	if (!s->subject)
	{
		ret = EINA_TRUE;
	}
	else
	{
		if (!strcmp(c->get_name(e), s->subject))
			ret = EINA_TRUE;
	}

	if (ret)
	{
		Ecss_Filter *f;
		Eina_List *l;

		EINA_LIST_FOREACH(s->filters, l, f)
		{
			if (!ecss_filter_test(f, c, e))
				return EINA_FALSE;
		}
	}
	return ret;
}

static void _process_element(Ecss_Context *c, Ecss_Element *e, Ecss_State *state)
{
	Ecss_Element *e1;
	Ecss_State_Selector *ss;
	Eina_List *l;
	Eina_List *l_next;
	Eina_List *new_active = NULL;
	Eina_List *new_inactive = NULL;
	Eina_List *old_active;
	Eina_List *old_inactive;

	/* process this element */
	/* move from the inactive rule to the new active list or apply directly */
	old_inactive = state->inactive;
	EINA_LIST_FOREACH(state->inactive, l, ss)
	{
		Ecss_Selector *s_next;
		Ecss_Selector *s = ss->s;

		if (_element_matches(c, e, ss))
		{
			/* can we apply directly? */
			s_next = ecss_selector_next_get(s);
			if (!s_next)
			{
				/* apply */
				_element_rule_apply(c, e, ss->r);
			}
			else
			{
				new_active = eina_list_append(new_active, ss);
			}
		}
		else
		{
			new_inactive = eina_list_append(new_inactive, ss);
		}
	}
	old_active = state->active;
	EINA_LIST_FOREACH(state->active, l, ss)
	{
		Ecss_Selector *s_next;
		Ecss_Selector *s = ss->s;
		Eina_Bool keep = EINA_FALSE; /* keep in the list of actives for later */
		Eina_Bool apply = EINA_FALSE; /* apply if it matches */

		switch (s->c)
		{
			case ECSS_NONE:
			/* this case should not happen */
			printf(">>> error <<<\n");
			break;

			case ECSS_DESCENDANT:
			if (ss->child > 0)
			{
				apply = EINA_TRUE;
				keep = EINA_TRUE;
			}
			break;

			case ECSS_CHILD:
			if (ss->child == 1)
				apply = EINA_TRUE;
			break;

			case ECSS_ADJACENT_SIBLING:
			if (ss->sibling == 1)
				apply = EINA_TRUE;
			break;

			case ECSS_SIBLING:
			if (ss->sibling > 0)
			{
				apply = EINA_TRUE;
				keep = EINA_TRUE;
			}
			break;

		}

		if (apply)
		{
			if (_element_matches(c, e, ss))
			{
				s_next = ecss_selector_next_get(s);
				if (!s_next)
				{
					/* apply */
					_element_rule_apply(c, e, ss->r);
				}
				else
				{
					ss->s = s_next;
					keep = EINA_TRUE;
				}
			}
		}
		if (keep)
		{
			new_active = eina_list_append(new_active, ss);
		}
		else
		{
			new_inactive = eina_list_append(new_inactive, ss);
		}
		/* given that we are going to test the first child we can safely
		 * increment the child here */
		ss->child++;
	}

	state->active = new_active;
	state->inactive = new_inactive;

	e1 = c->get_child(e);
	while (e1)
	{
		_process_element(c, e1, state);
		e1 = c->get_next_sibling(e1);
		/* increment the active sibling count */
		EINA_LIST_FOREACH(new_active, l, ss)
		{
			ss->sibling++;
		}
	}
	/* TODO for every s_next we have advanced, set the prev one */
	/* decrement the child and reset the sibling */
	EINA_LIST_FOREACH(new_active, l, ss)
	{
		ss->child--;
		ss->sibling = 0;
	}

	state->active = old_active;
	state->inactive = old_inactive;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void ecss_context_style_apply(Ecss_Context *c, Ecss_Style *s, Ecss_Element *e)
{
	Ecss_State state;
	Ecss_Rule *r;
	Eina_List *l;

	if (!c) return;
	if (!s) return;
	if (!e) return;

	state.active = NULL;
	state.inactive = NULL;

	/* put every rule on the inactive list */
	EINA_LIST_FOREACH(s->rules, l, r)
	{
		Ecss_State_Selector *ss;
		Ecss_Selector *s = r->selector;

		ss = malloc(sizeof(Ecss_State_Selector));
		ss->r = r;
		ss->s = s;
		ss->child = 0;
		ss->sibling = 0;
		state.inactive = eina_list_append(state.inactive, ss);
	}
	/* parse the tree top -> bottom */
	_process_element(c, e, &state);
	/* TODO destroy the state */
}
