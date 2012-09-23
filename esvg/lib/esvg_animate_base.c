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
#include "esvg_private_attribute_animation.h"
#include "esvg_private_context.h"
#include "esvg_private_element.h"
#include "esvg_private_animation.h"
#include "esvg_private_animate_base.h"

#include "esvg_animate_base.h"
/*
 * This file handles the common attribute handling for the
 * 'animate_base value attributes'. The elements that inherit
 * from this are the 'animate' and 'animateTransform'
 * TODO
 * several properties must not be a string, but a list of values
 * like for example the values, times, keys, etc
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_ANIMATE_BASE_MAGIC_CHECK(d) \
	do {\
		if (!EINA_MAGIC_CHECK(d, ESVG_ANIMATE_BASE_MAGIC))\
			EINA_MAGIC_FAIL(d, ESVG_ANIMATE_BASE_MAGIC);\
	} while(0)

static Ender_Property *ESVG_ANIMATE_BASE_TO;
static Ender_Property *ESVG_ANIMATE_BASE_FROM;
static Ender_Property *ESVG_ANIMATE_BASE_VALUES;
static Ender_Property *ESVG_ANIMATE_BASE_KEY_TIMES;
static Ender_Property *ESVG_ANIMATE_BASE_KEY_SPLINES;
static Ender_Property *ESVG_ANIMATE_BASE_CALC_MODE;

typedef struct _Esvg_Animate_Base_Values_Data
{
	Eina_List *values;
	Esvg_Animate_Base_Value_Get get;
} Esvg_Animate_Base_Values_Data;

typedef struct _Esvg_Animate_Base_Times_Data
{
	Eina_List *times;
	int64_t duration;
} Esvg_Animate_Base_Times_Data;

typedef struct _Esvg_Animate_Base_Descriptor_Internal
{
	Edom_Tag_Free free;
	Esvg_Element_Attribute_Set attribute_set;
	Edom_Tag_Attribute_Get attribute_get;
	Esvg_Animate_Base_Type_Descriptor_Get type_descriptor_get;
} Esvg_Animate_Base_Descriptor_Internal;

typedef struct _Esvg_Animate_Base
{
	EINA_MAGIC
	/* properties */
	Esvg_Animate_Base_Context current;
	/* interface */
	Esvg_Animate_Base_Descriptor_Internal descriptor;
	/* private */
	Etch *etch;
	Esvg_Attribute_Type attribute_type;
	Ender_Element *parent_e;
	Ender_Property *p;
	Edom_Tag *parent_t;
	Esvg_Animate_Base_Type_Descriptor *d;
	Eina_List *values;
	Eina_List *times;
	Eina_List *animations;
	void *data;
} Esvg_Animate_Base;

typedef struct _Esvg_Animate_Base_Animation
{
	Esvg_Animate_Base *thiz;
	Esvg_Animate_Base_Animation_Callback cb;
	Edom_Tag *t;
	Ender_Element *parent;
	Etch_Animation *a;
	void *data;
} Esvg_Animate_Base_Animation;

static Esvg_Animate_Base * _esvg_animate_base_get(Edom_Tag *t)
{
	Esvg_Animate_Base *thiz;

	thiz = esvg_animation_data_get(t);
	ESVG_ANIMATE_BASE_MAGIC_CHECK(thiz);

	return thiz;
}

static void _esvg_animate_base_animation_simple_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *user_data)
{
	Esvg_Animate_Base_Animation *data = user_data;
	Esvg_Animate_Base *thiz;
	Esvg_Attribute_Type old_type;
	void *kdata;

	thiz = data->thiz;
	kdata = etch_animation_keyframe_data_get(k);

	old_type = esvg_element_attribute_type_get(thiz->parent_t);
	esvg_element_attribute_type_set(thiz->parent_t, thiz->attribute_type);
	esvg_element_attribute_animate_set(thiz->parent_t, EINA_TRUE);
	/* call the implementation */
	data->cb(data->t, thiz->parent_e, thiz->p, curr, prev, kdata, data->data);
	/* restore the states */
	esvg_element_attribute_animate_set(thiz->parent_t, EINA_FALSE);
	esvg_element_attribute_type_set(thiz->parent_t, old_type);
}

static void _esvg_animate_base_animation_full_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *user_data)
{
	Esvg_Animate_Base_Animation *data = user_data;
	Esvg_Animate_Base *thiz;
	void *kdata;

	thiz = data->thiz;
	kdata = etch_animation_keyframe_data_get(k);

	data->cb(data->t, thiz->parent_e, thiz->p, curr, prev, kdata, data->data);
}

static Esvg_Animate_Base_Animation * _esvg_animate_base_animation_new(Esvg_Animate_Base *thiz,
		Etch *e,
		Etch_Data_Type etch_type,
		Etch_Animation_Callback etch_cb,
		Esvg_Animation_Context *actx,
		Esvg_Animate_Base_Context *abctx,
		Esvg_Animate_Base_Animation_Callback cb,
		void *data)
{
	Esvg_Animate_Base_Animation *a;
	Etch_Animation *etch_a;

	a = calloc(1, sizeof(Esvg_Animate_Base_Animation));
	a->thiz = thiz;
	a->cb = cb;
	a->data = data;

	etch_a = etch_animation_add(e, etch_type, etch_cb,
				NULL, NULL, a);
	/* the repeat count */
	etch_animation_repeat_set(etch_a, actx->timing.repeat_count);
	/* TODO chek the condition to trigger the animation on/off */
	etch_animation_enable(etch_a);
	a->a = etch_a;

	return a;
}

#if 0
static void _esvg_animate_key_splines_cb(const char *v, void *user_data)
{
	Etch_Data ndata;
	Etch_Data cdata;
	Etch_Data cp0;
	Etch_Data cp1;

	/* iterate over the attribute by either space or commas */
	/* get the current keyframe and the next */
	/* the range is from 0 to 1, so we need to multiply the value with that
	 * factor and set the value on the cubic argument */
}
#endif

static void _esvg_animate_base_values_cb(const char *v, void *user_data)
{
	Esvg_Animate_Base_Values_Data *data = user_data;
	void *get_data;

	if (data->get(v, &get_data))
		data->values = eina_list_append(data->values, get_data);
}

static void _esvg_animate_base_time_cb(const char *v, void *user_data)
{
	Esvg_Animate_Base_Times_Data *data = user_data;
	double percent;
	int64_t *t;

	percent = esvg_number_string_from(v, 1.0);
	if (percent < 0.0)
		percent = 0;
	else if (percent > 1.0)
		percent = 1.0;

	t = malloc(sizeof(int64_t));
	*t = data->duration * percent;
	data->times = eina_list_append(data->times, t);
}

static void _esvg_animate_base_key_splines_cb(const char *v, void *user_data)
{
	Eina_List **ret = user_data;
	Esvg_Animate_Key_Spline *spline;

	/* fetch the four values */
	spline = calloc(1, sizeof(Esvg_Animate_Key_Spline));
	esvg_animate_key_spline_string_from(spline, v);
	*ret = eina_list_append(*ret, spline);
}

static Eina_Bool _esvg_animate_base_values_generate(Esvg_Animate_Base_Context *c,
		Esvg_Animate_Base_Value_Get get_cb,
		Eina_List **values,
		Eina_Bool *has_from)
{
	*has_from = EINA_TRUE;

	if (c->value.values)
	{
		Esvg_Animate_Base_Values_Data data;

		data.values = *values;
		data.get = get_cb;
		esvg_list_string_from(c->value.values, ';',
			_esvg_animate_base_values_cb, &data);
		*values = data.values;
	}
	else
	{
		if (c->value.from)
		{
			void *data;
			if (get_cb(c->value.from, &data))
				*values = eina_list_append(*values, data);
		}
		else
		{
			/* mark the missing from */
			*has_from = EINA_FALSE;
		}

		if (c->value.to)
		{
			void *data;
			if (get_cb(c->value.to, &data))
				*values = eina_list_append(*values, data);
		}
#if 0
		else if (c->value.by)
		{
			/* if no from, then everything is dynamic until the animation starts */
			/* TODO append the from to the values */
		}
#endif
	}

	return EINA_TRUE;
}

static void _esvg_animate_base_values_free(Eina_List *values, Esvg_Animate_Base_Value_Free free_cb)
{
	void *data;

	if (!values) return;

	EINA_LIST_FREE (values, data);
		free_cb(data);
}

static Eina_Bool _esvg_animate_base_key_splines_generate(Esvg_Animate_Base_Context *c,
		Eina_List **ksplines)
{
	Eina_List *l = NULL;

	if (!c->value.key_splines)
		return EINA_TRUE;

	esvg_list_string_from(c->value.key_splines, ';',
		_esvg_animate_base_key_splines_cb, &l);
	*ksplines = l;
}


static Eina_Bool _esvg_animate_base_times_generate(Esvg_Animation_Context *ac,
		Esvg_Animate_Base_Context *c,
		Eina_List *values,
		Eina_List **times)
{
	/* generate the times list */
	/* get the duration */
	if (ac->timing.dur.type == ESVG_DURATION_TYPE_CLOCK)
	{
		if (c->value.key_times)
		{
			Esvg_Animate_Base_Times_Data data;

			data.times = *times;
			data.duration = ac->timing.dur.data.clock;

			esvg_list_string_from(c->value.key_times, ';',
					_esvg_animate_base_time_cb, &data);
			*times = data.times;
		}
		else
		{
			int64_t t = 0;
			int i;
			int length;
			int64_t duration;
			int64_t inc;

			length = eina_list_count(values);
			if (!length)
			{
				printf("no values?\n");
				return EINA_FALSE;
			}
			duration = ac->timing.dur.data.clock;
			inc = duration / (length - 1);
			for (i = 0; i < length; i++)
			{
				int64_t *d;

				d = malloc(sizeof(int64_t));
				*d = t;
				printf("adding time at %lld %lld (%lld %d)\n", t, inc, duration, length);
				*times = eina_list_append(*times, d);
				t += inc;
			}
		}
	}
	return EINA_TRUE;
}

static void _esvg_animate_base_times_free(Eina_List *times)
{
	int64_t *v;

	if (!times) return;
	EINA_LIST_FREE (times, v)
		free(v);
}
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static void _esvg_animate_base_to_set(Edom_Tag *t, const char *to)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->current.value.to)
	{
		free(thiz->current.value.to);
		thiz->current.value.to = NULL;
	}
	if (to)
		thiz->current.value.to = strdup(to);
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_to_get(Edom_Tag *t, const char **to)
{
	Esvg_Animate_Base *thiz;

	if (!to) return;
	thiz = _esvg_animate_base_get(t);
	*to = thiz->current.value.to;
}

static void _esvg_animate_base_from_set(Edom_Tag *t, const char *from)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->current.value.from)
	{
		free(thiz->current.value.from);
		thiz->current.value.from = NULL;
	}
	if (from)
		thiz->current.value.from = strdup(from);
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_from_get(Edom_Tag *t, const char **from)
{
	Esvg_Animate_Base *thiz;

	if (!from) return;
	thiz = _esvg_animate_base_get(t);
	*from = thiz->current.value.from;
}

static void _esvg_animate_base_values_set(Edom_Tag *t, const char *values)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->current.value.values)
	{
		free(thiz->current.value.values);
		thiz->current.value.values = NULL;
	}
	if (values)
		thiz->current.value.values = strdup(values);
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_values_get(Edom_Tag *t, const char **values)
{
	Esvg_Animate_Base *thiz;

	if (!values) return;
	thiz = _esvg_animate_base_get(t);
	*values = thiz->current.value.values;
}

static void _esvg_animate_base_calc_mode_set(Edom_Tag *t, Esvg_Calc_Mode calc_mode)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	thiz->current.value.calc_mode = calc_mode;
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_calc_mode_get(Edom_Tag *t, Esvg_Calc_Mode *calc_mode)
{
	Esvg_Animate_Base *thiz;

	if (!calc_mode) return;
	thiz = _esvg_animate_base_get(t);
	*calc_mode = thiz->current.value.calc_mode;
}

static void _esvg_animate_base_key_times_set(Edom_Tag *t, const char *key_times)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->current.value.key_times)
	{
		free(thiz->current.value.key_times);
		thiz->current.value.key_times = NULL;
	}
	if (key_times)
		thiz->current.value.key_times = strdup(key_times);
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_key_times_get(Edom_Tag *t, const char **key_times)
{
	Esvg_Animate_Base *thiz;

	if (!key_times) return;
	thiz = _esvg_animate_base_get(t);
	*key_times = thiz->current.value.key_times;
}

static void _esvg_animate_base_key_splines_set(Edom_Tag *t, const char *key_splines)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->current.value.key_splines)
	{
		free(thiz->current.value.key_splines);
		thiz->current.value.key_splines = NULL;
	}
	if (key_splines)
		thiz->current.value.key_splines = strdup(key_splines);
	thiz->current.changed = EINA_TRUE;
}

static void _esvg_animate_base_key_splines_get(Edom_Tag *t, const char **key_splines)
{
	Esvg_Animate_Base *thiz;

	if (!key_splines) return;
	thiz = _esvg_animate_base_get(t);
	*key_splines = thiz->current.value.key_splines;
}
/*----------------------------------------------------------------------------*
 *                         The Esvg Element interface                         *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_animate_base_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	/* value attributes */
	/* for from and to, we should not fetch the property an generate
	 * the final data type, we better pass it still as strings
	 */
	if (strcmp(key, "calcMode") == 0)
	{
		Esvg_Calc_Mode cm;

		esvg_calc_mode_string_from(&cm, value);
		esvg_animate_base_calc_mode_set(e, cm);
	}
	else if (strcmp(key, "values") == 0)
	{
		esvg_animate_base_values_set(e, value);
	}
	else if (strcmp(key, "keyTimes") == 0)
	{
		esvg_animate_base_key_times_set(e, value);
	}
	else if (strcmp(key, "keySplines") == 0)
	{
		esvg_animate_base_key_splines_set(e, value);
	}
	else if (strcmp(key, "from") == 0)
	{
		esvg_animate_base_from_set(e, value);
	}
	else if (strcmp(key, "to") == 0)
	{
		esvg_animate_base_to_set(e, value);
	}
	else if (strcmp(key, "by") == 0)
	{
	}
	else
	{
		Esvg_Animate_Base *thiz;
		Edom_Tag *t;

		t = ender_element_object_get(e);
		thiz = _esvg_animate_base_get(t);

		if (thiz->descriptor.attribute_set)
			return thiz->descriptor.attribute_set(e, key, value);
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

static Eina_Bool _esvg_animate_base_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static void _esvg_animate_base_free(Edom_Tag *t)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	if (thiz->descriptor.free)
		thiz->descriptor.free(t);
	free(thiz);
}

static Eina_Bool _esvg_animate_base_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Animation_Context *actx,
		Enesim_Error **error)
{
	Esvg_Animate_Base *thiz;
	Esvg_Animate_Base_Type_Descriptor *d;
	Ender_Element *svg_e;
	Ender_Container *ec;
	Eina_Bool has_from;
	Etch *etch;
	const char *name;

	thiz = _esvg_animate_base_get(t);
	/* in case of animations free them */
	if (thiz->animations)
	{

	}
	if (thiz->values)
	{
		_esvg_animate_base_values_free(thiz->values, d->value_free);
		thiz->values = NULL;
	}
	if (thiz->times)
	{
		_esvg_animate_base_times_free(thiz->times);
		thiz->times = NULL;
	}

	/* get the etch associated with the topmost svg */
	esvg_element_internal_topmost_get(t, &svg_e);
	if (!svg_e)
		return EINA_FALSE;

	etch = esvg_svg_etch_get(svg_e);
	if (!etch)
		return EINA_FALSE;

	/* get the descriptor */
	if (!thiz->descriptor.type_descriptor_get)
		return EINA_FALSE;

	ec = ender_property_container_get(actx->p);
	name = ender_container_registered_name_get(ec);

	if (!thiz->descriptor.type_descriptor_get(t, name, &d))
		return EINA_FALSE;
	if (!d->animation_generate)
		return EINA_FALSE;

	/* store our own needed context data */
	thiz->etch = etch;
	thiz->parent_e = actx->parent_e;
	thiz->parent_t = actx->parent_t;
	thiz->attribute_type = actx->target.attribute_type;
	thiz->p = actx->p;
	thiz->d = d;
	/* generate the values and times */
	_esvg_animate_base_values_generate(&thiz->current, d->value_get,
			&thiz->values, &has_from);
	_esvg_animate_base_times_generate(actx, &thiz->current, thiz->values, &thiz->times);
	/* call the animation generate to create the animations */
	d->animation_generate(t, thiz->values, thiz->times, actx, &thiz->current);

	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#define _esvg_animate_base_delete NULL
#define _esvg_animate_base_to_is_set NULL
#define _esvg_animate_base_from_is_set NULL
#define _esvg_animate_base_values_is_set NULL
#define _esvg_animate_base_calc_mode_is_set NULL
#define _esvg_animate_base_key_times_is_set NULL
#define _esvg_animate_base_key_splines_is_set NULL
#include "generated/esvg_generated_animate_base.c"

Eina_Bool esvg_is_animate_base_internal(Edom_Tag *t)
{
	Esvg_Animate_Base *thiz;
	Eina_Bool ret;

	if (!esvg_is_element_internal(t))
		return EINA_FALSE;
	thiz = esvg_element_data_get(t);
	ret = EINA_MAGIC_CHECK(thiz, ESVG_ANIMATE_BASE_MAGIC);

	return ret;
}

void * esvg_animate_base_data_get(Edom_Tag *t)
{
	Esvg_Animate_Base *thiz;

	thiz = _esvg_animate_base_get(t);
	return thiz->data;
}

Etch_Animation_Type esvg_animate_base_calc_mode_etch_to(Esvg_Calc_Mode c)
{
	switch (c)
	{
		case ESVG_CALC_MODE_DISCRETE:
		return ETCH_ANIMATION_DISCRETE;

		case ESVG_CALC_MODE_LINEAR:
		return ETCH_ANIMATION_LINEAR;

		case ESVG_CALC_MODE_SPLINE:
		return ETCH_ANIMATION_CUBIC;

		/* FIXME TODO */
		case ESVG_CALC_MODE_PACED:
		default:
		return ETCH_ANIMATION_LINEAR;
	}
}

Etch_Animation * esvg_animate_base_animation_simple_add(Edom_Tag *t, Etch_Data_Type dt,
		Esvg_Animation_Context *actx,
		Esvg_Animate_Base_Context *abctx,
		Esvg_Animate_Base_Animation_Callback cb, void *data)
{
	Esvg_Animate_Base *thiz;
	Esvg_Animate_Base_Animation *animation;

	thiz = _esvg_animate_base_get(t);
	animation = _esvg_animate_base_animation_new(thiz,
			thiz->etch, dt, _esvg_animate_base_animation_simple_cb,
			actx, abctx, cb, data);
	thiz->animations = eina_list_append(thiz->animations, animation);

	return animation->a;
}

/* the simple version of an animation add should set the animation type
 * the attribute animatable and then just call the value set function
 * pointer with the etch value as parameter
 */

void esvg_animate_base_animation_add_keyframe(Etch_Animation *a,
	Esvg_Animate_Base_Context *c,
	Etch_Data *etch_data,
	int64_t time, void *data)
{
	Etch_Animation_Keyframe *k;
	Etch_Animation_Type atype;

	atype = esvg_animate_base_calc_mode_etch_to(c->value.calc_mode);
	k = etch_animation_keyframe_add(a);
	etch_animation_keyframe_type_set(k, atype);
	etch_animation_keyframe_data_set(k, data, NULL);
	etch_animation_keyframe_value_set(k, etch_data);
	// set the time
	etch_animation_keyframe_time_set(k, time);
}

/* for simple animation generation, just pass the cb, the data type, the data to, etc, etc */
void esvg_animate_base_animation_generate(Edom_Tag *t,
		Eina_List *values,
		Eina_List *times,
		Esvg_Animation_Context *actx,
		Esvg_Animate_Base_Context *abctx,
		Etch_Data_Type dt,
		Esvg_Animate_Base_Value_Etch_Data_To data_to,
		Esvg_Animate_Base_Animation_Callback cb,
		void *data)
{
	Etch_Animation *a;
	Eina_List *tt;
	Eina_List *l;
	void *v;
	int64_t *time;

	if (!values || !times)
		return;

	a = esvg_animate_base_animation_simple_add(t, dt, actx, abctx, cb, data);
	tt = times;
	EINA_LIST_FOREACH(values, l, v)
	{
		Etch_Data edata;

		time = eina_list_data_get(tt);
		/* convert it to the destination etch type */
		data_to(v, &edata);
		/* add a keyframe */
		esvg_animate_base_animation_add_keyframe(a, abctx, &edata, *time, v);
		tt = eina_list_next(tt);
	}
}

Edom_Tag * esvg_animate_base_new(Esvg_Animate_Base_Descriptor *descriptor, Esvg_Type type,
		void *data)
{
	Esvg_Animate_Base *thiz;
	Esvg_Animation_Descriptor pdescriptor;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_Animate_Base));
	if (!thiz) return NULL;

	EINA_MAGIC_SET(thiz, ESVG_ANIMATE_BASE_MAGIC);
	thiz->data = data;
	/* default values */
	thiz->current.value.calc_mode = ESVG_CALC_MODE_LINEAR;
	/* our own descriptor */
	thiz->descriptor.type_descriptor_get = descriptor->type_descriptor_get;
	thiz->descriptor.attribute_set = descriptor->attribute_set;
	thiz->descriptor.attribute_get = descriptor->attribute_get;
	/* parent descriptor */
	pdescriptor.attribute_set = _esvg_animate_base_attribute_set;
	pdescriptor.attribute_get = _esvg_animate_base_attribute_get;
	pdescriptor.free = _esvg_animate_base_free;
	pdescriptor.initialize = descriptor->initialize;
	pdescriptor.setup = _esvg_animate_base_setup;

	t = esvg_animation_new(&pdescriptor, type, thiz);

	return t;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool esvg_is_animate_base(Ender_Element *e)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	return esvg_is_animate_base_internal(t);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_to_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_TO, v, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_to_get(Ender_Element *e, const char **v)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_to_get(t, v);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_from_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_FROM, v, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_from_get(Ender_Element *e, const char **v)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_from_get(t, v);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_values_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_VALUES, v, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_values_get(Ender_Element *e, const char **v)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_values_get(t, v);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_calc_mode_set(Ender_Element *e, Esvg_Calc_Mode calc_mode)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_CALC_MODE, calc_mode, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_calc_mode_get(Ender_Element *e, Esvg_Calc_Mode *calc_mode)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_calc_mode_get(t, calc_mode);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_key_times_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_KEY_TIMES, v, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_key_times_get(Ender_Element *e, const char **v)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_key_times_get(t, v);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_key_splines_set(Ender_Element *e, const char *v)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_BASE_KEY_SPLINES, v, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_base_key_splines_get(Ender_Element *e, const char **v)
{
	Edom_Tag *t;

	t = ender_element_object_get(e);
	_esvg_animate_base_key_splines_get(t, v);
}

