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
#include "esvg_private_attribute_animation.h"
#include "esvg_private_animation.h"
#include "esvg_private_animate_base.h"
#include "esvg_private_svg.h"

#include "esvg_animate_transform.h"
/* This file handles the 'animateTransform' tag. The idea
 * is that you can animate transformations by animating
 * the numbers that define a transformation and its type
 * For example you can animate a rotate from "0 60 60" to "360 60 60"
 * the syntax of the value is relative to the transform type
 * and uses the same format as the transform attribute
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Ender_Property *ESVG_ANIMATE_TRANSFORM_TYPE;

typedef union _Esvg_Animate_Transform_Data
{
	struct {
		double angle;
		double cx;
		double cy;	
	} rotate;

	struct {
		double sx;
		double sy;
	} scale;

	struct {
		double tx;
		double ty;
	} translate;

	struct {
		double angle;
	} skew;
} Esvg_Animate_Transform_Data;

typedef struct _Esvg_Animate_Transform
{
	/* properties */
	Esvg_Animate_Transform_Type type;
	/* interface */
	/* private */
	Esvg_Animate_Transform_Data data;
	Etch_Animation *anim;
	Ender_Property *prop;
	Ender_Element *parent_e;
	Edom_Tag *parent_t;
} Esvg_Animate_Transform;

static Esvg_Animate_Transform * _esvg_animate_transform_get(Edom_Tag *t)
{
	Esvg_Animate_Transform *thiz;

	if (esvg_element_internal_type_get(t) != ESVG_ANIMATETRANSFORM)
		return NULL;
	thiz = esvg_animate_base_data_get(t);

	return thiz;
}

/*----------------------------------------------------------------------------*
 *                         The Etch animator callbacks                        *
 *----------------------------------------------------------------------------*/
static void _esvg_animate_transform_rotate_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *data)
{
	Esvg_Animate_Transform *thiz = data;
	Esvg_Animated_Transform v;
	Enesim_Matrix m;

	enesim_matrix_rotate(&m, curr->data.d);
	v.base = m;

	ender_element_property_value_set(thiz->parent_e, thiz->prop, &v, NULL);
}

static void _esvg_animate_transform_translate_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *data)
{
	Esvg_Animate_Transform *thiz = data;
	Esvg_Animated_Transform v;
	Enesim_Matrix m;

	enesim_matrix_rotate(&m, curr->data.d);
	v.base = m;

	ender_element_property_value_set(thiz->parent_e, thiz->prop, &v, NULL);
}

static void _esvg_animate_transform_scale_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *data)
{
	Esvg_Animate_Transform *thiz = data;
	Esvg_Animated_Transform v;
	Enesim_Matrix m;

	enesim_matrix_rotate(&m, curr->data.d);
	v.base = m;

	ender_element_property_value_set(thiz->parent_e, thiz->prop, &v, NULL);
}

static void _esvg_animate_transform_skewx_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *data)
{
	Esvg_Animate_Transform *thiz = data;
	Esvg_Animated_Transform v;
	Enesim_Matrix m;

	enesim_matrix_rotate(&m, curr->data.d);
	v.base = m;

	ender_element_property_value_set(thiz->parent_e, thiz->prop, &v, NULL);
}

static void _esvg_animate_transform_skewy_cb(Etch_Animation_Keyframe *k,
		const Etch_Data *curr,
		const Etch_Data *prev,
		void *data)
{
	Esvg_Animate_Transform *thiz = data;
	Esvg_Animated_Transform v;
	Enesim_Matrix m;

	enesim_matrix_rotate(&m, curr->data.d);
	v.base = m;

	ender_element_property_value_set(thiz->parent_e, thiz->prop, &v, NULL);
}

static Eina_Bool _esvg_animate_transform_container_etch_to(Esvg_Animate_Transform *thiz, Etch *etch,
		Ender_Property *p, Esvg_Animate_Base_Context *c)
{
	Ender_Container *ec;
	Etch_Animation *a;
	Etch_Data from;
	Etch_Data to;
	Etch_Animation_Callback cb = NULL;
	Etch_Animation_Keyframe *kf;
	const char *name;

	ec = ender_property_container_get(p);
	name = ender_container_registered_name_get(ec);

	if (strcmp(name, "esvg_animated_transform"))
		return EINA_FALSE;
	/* get the type that we will animate: rotate, scale, etc */
	switch (thiz->type)
	{
		case ESVG_ANIMATE_TRANSFORM_TYPE_TRANSLATE:
		cb = _esvg_animate_transform_translate_cb;
		break;

		case ESVG_ANIMATE_TRANSFORM_TYPE_ROTATE:
		cb = _esvg_animate_transform_rotate_cb;
		break;

		case ESVG_ANIMATE_TRANSFORM_TYPE_SCALE:
		cb = _esvg_animate_transform_scale_cb;
		break;

		case ESVG_ANIMATE_TRANSFORM_TYPE_SKEWX:
		cb = _esvg_animate_transform_skewx_cb;
		break;

		case ESVG_ANIMATE_TRANSFORM_TYPE_SKEWY:
		cb = _esvg_animate_transform_skewy_cb;
		break;
	}

	if (!cb) return EINA_FALSE;

	/* we should add one one animation for each data found */
	/* the animation always animates doubles */
	a = etch_animation_add(etch, ETCH_DOUBLE, cb, NULL, NULL, thiz);
	/* add the keyframes */
	{
		//thiz->from = esvg_number_string_from(c->value.from, 1.0);
		//thiz->to = esvg_number_string_from(c->value.to, 1.0);
		//from.data.d = thiz->from;
		//to.data.d = thiz->to;
		from.data.d = 0;
		from.data.d = 360;

		/* TODO handle the type case */
		/* TODO for multiple values, check that the values are different to create animations */
		/* TODO replace the anim with a list of animations */
		/* TODO make global the possibility to parse comma or space separated values optionally */
	}

	/* FIXME for now we add two keyframes */
	/* second keyframe */
	kf = etch_animation_keyframe_add(a);
	etch_animation_keyframe_type_set(kf, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(kf, &from);
	etch_animation_keyframe_time_set(kf, 3 * ETCH_SECOND);
	/* third keyframe */
	kf = etch_animation_keyframe_add(a);
	etch_animation_keyframe_type_set(kf, ETCH_ANIMATION_LINEAR);
	etch_animation_keyframe_value_set(kf, &to);
	etch_animation_keyframe_time_set(kf, 5 * ETCH_SECOND);
	etch_animation_enable(a);

	printf("everything went ok!\n");

	thiz->anim = a;
	thiz->prop = p;

	return EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                         The Esvg Element interface                         *
 *----------------------------------------------------------------------------*/
static void _esvg_animate_transform_free(Edom_Tag *t)
{
	Esvg_Animate_Transform *thiz;

	thiz = _esvg_animate_transform_get(t);
	free(thiz);
}

static Eina_Bool _esvg_animate_transform_attribute_set(Ender_Element *e,
		const char *key, const char *value)
{
	if (!strcmp(key, "type"))
	{
		Esvg_Animate_Transform_Type type;

		esvg_animate_transform_type_string_from(&type, value);
		esvg_animate_transform_type_set(e, type);
	}
	return EINA_TRUE;
}

static Eina_Bool _esvg_animate_transform_attribute_get(Edom_Tag *tag, const char *attribute, char **value)
{
	return EINA_FALSE;
}

static Eina_Bool _esvg_animate_transform_setup(Edom_Tag *t,
		Esvg_Context *c,
		Esvg_Animation_Context *actx,
		Esvg_Animate_Base_Context *abctx,
		Enesim_Error **error)
{
	Esvg_Animate_Transform *thiz;
	Ender_Element *svg_e;
	Ender_Element *parent_e;
	Edom_Tag *parent_t;
	Ender_Property *p;
	Etch *etch;

	thiz = _esvg_animate_transform_get(t);
	printf("animate_transform setup!\n");

	/* same */
	esvg_element_internal_topmost_get(t, &svg_e);
	if (!svg_e) goto done;

	etch = esvg_svg_etch_get(svg_e);
	if (!etch) goto done;

	if (thiz->anim)
		etch_animation_delete(thiz->anim);

	/* FIXME should we get a reference here ? */
	p = actx->p;
	thiz->parent_t = actx->parent_t;
	thiz->parent_e = actx->parent_e;

	/* we should only process lengths, colors, integers, booleans, etc */
	if (!_esvg_animate_transform_container_etch_to(thiz, etch, p, abctx))
		goto done;

	/* check the type and create an animator of that container type */
	/* on every animation callback set the animation mode on the element */
	/* then call the property set */
	/* restore the mode */

done:

	return EINA_TRUE;
}

static Esvg_Animate_Base_Descriptor _descriptor = {
	/* .attribute_get 	= */ _esvg_animate_transform_attribute_get,
	/* .free 		= */ _esvg_animate_transform_free,
	/* .initialize 		= */ NULL,
	/* .attribute_set 	= */ _esvg_animate_transform_attribute_set,
	/* .setup		= */ _esvg_animate_transform_setup,
};
/*----------------------------------------------------------------------------*
 *                           The Ender interface                              *
 *----------------------------------------------------------------------------*/
static void _esvg_animate_transform_type_set(Edom_Tag *t, Esvg_Animate_Transform_Type type)
{
	Esvg_Animate_Transform *thiz;

	thiz = _esvg_animate_transform_get(t);
	thiz->type = type;
}

static void _esvg_animate_transform_type_get(Edom_Tag *t, Esvg_Animate_Transform_Type *type)
{
	Esvg_Animate_Transform *thiz;

	if (!type) return;
	thiz = _esvg_animate_transform_get(t);
	*type = thiz->type;
}

static Edom_Tag * _esvg_animate_transform_new(void)
{
	Esvg_Animate_Transform *thiz;
	Edom_Tag *t;

	thiz = calloc(1, sizeof(Esvg_Animate_Transform));

	t = esvg_animate_base_new(&_descriptor, ESVG_ANIMATETRANSFORM, thiz);
	return t;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The ender wrapper */
#define _esvg_animate_transform_type_is_set NULL
#include "generated/esvg_generated_animate_transform.c"
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ender_Element * esvg_animate_transform_new(void)
{
	return ender_element_new_with_namespace("animate_transform", "esvg");
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_transform_type_set(Ender_Element *e, Esvg_Animate_Transform_Type type)
{
	ender_element_property_value_set(e, ESVG_ANIMATE_TRANSFORM_TYPE, type, NULL);
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_animate_transform_type_get(Ender_Element *e, Esvg_Animate_Transform_Type *type)
{
}


