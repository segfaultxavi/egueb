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
#include "esvg_types.h"
#include "esvg_private_attribute.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/*----------------------------------------------------------------------------*
 *                                 Length                                     *
 *----------------------------------------------------------------------------*/
static void * _esvg_animate_length_new(void)
{
	Esvg_Length *v;

	v = calloc(1, sizeof(Esvg_Length));
	return v;
}

static Eina_Bool _esvg_animate_length_get(const char *attr, void **value)
{
	Esvg_Length *v = *value;

	esvg_length_string_from(v, attr);
	return EINA_TRUE;
}

static void * _esvg_animate_length_destination_new(void)
{
	Esvg_Animated_Length *v;
	v = calloc(1, sizeof(Esvg_Animated_Length));
	return v;
}

static void _esvg_animate_length_destination_value_to(void *destination, void **value)
{
	
}

static void _esvg_animate_length_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	Esvg_Length *va = a;
	Esvg_Length *vb = b;
	Esvg_Length *vacc = acc;
	Esvg_Animated_Length *vadd = add;
	Esvg_Animated_Length *r = res;

	r->base.unit = va->unit;
	etch_interpolate_double(va->value, vb->value, m, &r->base.value);
	if (vacc)
		r->base.value += vacc->value * mul;
	if (vadd)
		r->base.value += vadd->anim.value;
}

/*----------------------------------------------------------------------------*
 *                                 Number                                     *
 *----------------------------------------------------------------------------*/
static void * _esvg_animate_number_new(void)
{
	double *v;

	v = calloc(1, sizeof(double));
	return v;
}

static Eina_Bool _esvg_animate_number_get(const char *attr, void **value)
{
	double *v = *value;

	*v = esvg_number_string_from(attr, 1.0);
	return EINA_TRUE;
}

static void * _esvg_animate_number_destination_new(void)
{
	Esvg_Animated_Number *v;
	v = calloc(1, sizeof(Esvg_Animated_Number));
	return v;
}

static void _esvg_animate_number_destination_value_to(void *destination, void **value)
{
	
}

static void _esvg_animate_number_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	Esvg_Number *va = a;
	Esvg_Number *vb = b;
	Esvg_Animated_Number *vadd = add;
	Esvg_Animated_Number *r = res;

	etch_interpolate_double(*va, *vb, m, &r->base);
	if (vadd)
		r->base += vadd->anim;
}

/*----------------------------------------------------------------------------*
 *                                 String                                     *
 *----------------------------------------------------------------------------*/
static void * _esvg_animate_string_new(void)
{
	return NULL;
}

static Eina_Bool _esvg_animate_string_get(const char *attr, void **value)
{
	char *v;

	v = strdup(attr);
	*value = v;

	return EINA_TRUE;
}

static void * _esvg_animate_string_destination_new(void)
{
	Esvg_Animated_String *v;
	v = calloc(1, sizeof(Esvg_Animated_String));
	return v;
}

static void _esvg_animate_string_destination_value_to(void *destination, void **value)
{
	
}

static void _esvg_animate_string_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	char *va = a;
	char *vb = b;
	Esvg_Animated_String *r = res;

	/* TODO
	etch_interpolate_string(va, vb, m, &r->base);
	*/
}

/*----------------------------------------------------------------------------*
 *                   The path command type descriptor                         *
 *----------------------------------------------------------------------------*/
static void _esvg_animate_path_command_get_cb(Esvg_Path_Command *cmd, void *data)
{
	Esvg_Path_Command *pcmd;
	Eina_List **dst = data;

	pcmd = calloc(1, sizeof(Esvg_Path_Command));
	*pcmd = *cmd;
	*dst = eina_list_append(*dst, pcmd);
}

static void * _esvg_animate_path_command_new(void)
{
	return NULL;
}

static Eina_Bool _esvg_animate_path_command_get(const char *attr, void **value)
{
	esvg_path_string_from(attr, _esvg_animate_path_command_get_cb, value);
	return EINA_TRUE;
}

static void _esvg_animate_path_command_free(void *d)
{
	Esvg_Path_Command *pcmd;
	Eina_List *l = d;

	EINA_LIST_FREE(l, pcmd)
		free(pcmd);
}

static void * _esvg_animate_path_command_destination_new(void)
{
	Esvg_Animated_List *v;

	v = calloc(1, sizeof(Esvg_Animated_List));
	return v;
}

static void _esvg_animate_path_command_destination_get(void *d, Eina_List *values)
{
	Esvg_Animated_List *v = d;
	Esvg_Path_Command *cmd;
	Eina_List *value;
	Eina_List *l;

	value = values->data;
	EINA_LIST_FOREACH (value, l, cmd)
	{
		Esvg_Path_Command *ncmd;

		ncmd = calloc(1, sizeof(Esvg_Path_Command));
		*ncmd = *cmd;
		v->base = eina_list_append(v->base, ncmd);
	}
}

static void _esvg_animate_path_command_destination_free(void *data)
{
	Esvg_Animated_List *v = data;
	Esvg_Path_Command *cmd;

	EINA_LIST_FREE (v->base, cmd)
		free(cmd);
	free(v);
}

static void _esvg_animate_path_command_destination_value_to(void *destination, void **value)
{
	Esvg_Animated_List *v = destination;

	*value = v->base;
}

static void _esvg_animate_path_command_interpolate(void *a, void *b, double m,
		void *add, void *acc, int mul, void *res)
{
	Esvg_Animated_List *r = res;
	Esvg_Path_Command *ca;
	Eina_List *va = a;
	Eina_List *vb = b;
	Eina_List *l1, *l2, *l3;
	
	l2 = vb;
	l3 = r->base;
	EINA_LIST_FOREACH (va, l1, ca)
	{
		Esvg_Path_Command *cb = l2->data;
		Esvg_Path_Command *cr = l3->data;

		if (m <= 0.5)
			cr->relative = ca->relative;
		else
			cr->relative = cb->relative;
		switch (ca->type)
		{
			case ESVG_PATH_MOVE_TO:
			etch_interpolate_double(ca->data.move_to.x, cb->data.move_to.x, m, &cr->data.move_to.x);
			etch_interpolate_double(ca->data.move_to.y, cb->data.move_to.y, m, &cr->data.move_to.y);
			break;
			case ESVG_PATH_LINE_TO:
			etch_interpolate_double(ca->data.line_to.x, cb->data.line_to.x, m, &cr->data.line_to.x);
			etch_interpolate_double(ca->data.line_to.y, cb->data.line_to.y, m, &cr->data.line_to.y);
			break;
			case ESVG_PATH_HLINE_TO:
			etch_interpolate_double(ca->data.hline_to.c, cb->data.hline_to.c, m, &cr->data.hline_to.c);
			break;
			case ESVG_PATH_VLINE_TO:
			etch_interpolate_double(ca->data.vline_to.c, cb->data.vline_to.c, m, &cr->data.vline_to.c);
			break;
			case ESVG_PATH_CUBIC_TO:
			etch_interpolate_double(ca->data.cubic_to.ctrl_x1, cb->data.cubic_to.ctrl_x1, m, &cr->data.cubic_to.ctrl_x1);
			etch_interpolate_double(ca->data.cubic_to.ctrl_y1, cb->data.cubic_to.ctrl_y1, m, &cr->data.cubic_to.ctrl_y1);
			etch_interpolate_double(ca->data.cubic_to.ctrl_x0, cb->data.cubic_to.ctrl_x0, m, &cr->data.cubic_to.ctrl_x0);
			etch_interpolate_double(ca->data.cubic_to.ctrl_y0, cb->data.cubic_to.ctrl_y0, m, &cr->data.cubic_to.ctrl_y0);
			etch_interpolate_double(ca->data.cubic_to.x, cb->data.cubic_to.x, m, &cr->data.cubic_to.x);
			etch_interpolate_double(ca->data.cubic_to.y, cb->data.cubic_to.y, m, &cr->data.cubic_to.y);
			break;
			case ESVG_PATH_SCUBIC_TO:
			etch_interpolate_double(ca->data.scubic_to.ctrl_x, cb->data.scubic_to.ctrl_x, m, &cr->data.scubic_to.ctrl_x);
			etch_interpolate_double(ca->data.scubic_to.ctrl_y, cb->data.scubic_to.ctrl_y, m, &cr->data.scubic_to.ctrl_y);
			etch_interpolate_double(ca->data.scubic_to.x, cb->data.scubic_to.x, m, &cr->data.scubic_to.x);
			etch_interpolate_double(ca->data.scubic_to.y, cb->data.scubic_to.y, m, &cr->data.scubic_to.y);
			break;
			case ESVG_PATH_QUADRATIC_TO:
			etch_interpolate_double(ca->data.quadratic_to.ctrl_x, cb->data.quadratic_to.ctrl_x, m, &cr->data.quadratic_to.ctrl_x);
			etch_interpolate_double(ca->data.quadratic_to.ctrl_y, cb->data.quadratic_to.ctrl_y, m, &cr->data.quadratic_to.ctrl_y);
			etch_interpolate_double(ca->data.quadratic_to.x, cb->data.quadratic_to.x, m, &cr->data.quadratic_to.x);
			etch_interpolate_double(ca->data.quadratic_to.y, cb->data.quadratic_to.y, m, &cr->data.quadratic_to.y);
			break;
			case ESVG_PATH_SQUADRATIC_TO:
			etch_interpolate_double(ca->data.squadratic_to.x, cb->data.squadratic_to.x, m, &cr->data.squadratic_to.x);
			etch_interpolate_double(ca->data.squadratic_to.y, cb->data.squadratic_to.y, m, &cr->data.squadratic_to.y);
			break;
			case ESVG_PATH_ARC_TO:
			etch_interpolate_double(ca->data.arc_to.x, cb->data.arc_to.x, m, &cr->data.arc_to.x);
			etch_interpolate_double(ca->data.arc_to.y, cb->data.arc_to.y, m, &cr->data.arc_to.y);
			break;
			case ESVG_PATH_CLOSE:
			break;
		}
		l2 = l2->next;
		l3 = l3->next;
	}
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* The different animated desriptors */
Esvg_Attribute_Animated_Descriptor esvg_attribute_animated_length_descriptor = {
	/* .value_new 			= */ _esvg_animate_length_new,
	/* .value_get 			= */ _esvg_animate_length_get,
	/* .value_free 			= */ free,
	/* .destination_new 		= */ _esvg_animate_length_destination_new,
	/* .destination_get 		= */ NULL,
	/* .destination_free 		= */ free,
	/* .destination_value_to 	= */ _esvg_animate_length_destination_value_to,
	/* .interpolate 		= */ _esvg_animate_length_interpolate,
};

Esvg_Attribute_Animated_Descriptor esvg_attribute_animated_number_descriptor = {
	/* .value_new 		= */ _esvg_animate_number_new,
	/* .value_get 		= */ _esvg_animate_number_get,
	/* .value_free 		= */ free,
	/* .destination_new 	= */ _esvg_animate_number_destination_new,
	/* .destination_get 	= */ NULL,
	/* .destination_free 	= */ free,
	/* .destination_value_to = */ _esvg_animate_number_destination_value_to,
	/* .interpolate 	= */ _esvg_animate_number_interpolate,
};

Esvg_Attribute_Animated_Descriptor esvg_attribute_animated_string_descriptor = {
	/* .value_new 		= */ _esvg_animate_string_new,
	/* .value_get 		= */ _esvg_animate_string_get,
	/* .value_free 		= */ free,
	/* .destination_new 	= */ _esvg_animate_string_destination_new,
	/* .destination_get 	= */ NULL,
	/* .destination_free 	= */ free,
	/* .destination_value_to 	= */ _esvg_animate_string_destination_value_to,
	/* .interpolate 	= */ _esvg_animate_string_interpolate,
};

Esvg_Attribute_Animated_Descriptor esvg_attribute_animated_path_command_descriptor = {
	/* .value_new 		= */ _esvg_animate_path_command_new,
	/* .value_get 		= */ _esvg_animate_path_command_get,
	/* .value_free 		= */ _esvg_animate_path_command_free,
	/* .destination_new 	= */ _esvg_animate_path_command_destination_new,
	/* .destination_get 	= */ _esvg_animate_path_command_destination_get,
	/* .destination_free 	= */ _esvg_animate_path_command_destination_free,
	/* .destination_value_to 	= */ _esvg_animate_path_command_destination_value_to,
	/* .interpolate 	= */ _esvg_animate_path_command_interpolate,
};

Esvg_Attribute_Animated_Descriptor * esvg_attribute_animated_descriptor_get(const char *name)
{
	Esvg_Attribute_Animated_Descriptor *d = NULL;

	if (!strcmp(name, "esvg_animated_length") || !strcmp(name, "esvg_animated_coord"))
	{
		d = &esvg_attribute_animated_length_descriptor;
	}
	else if (!strcmp(name, "esvg_animated_number"))
	{
		d = &esvg_attribute_animated_number_descriptor;
	}
	else if (!strcmp(name, "esvg_animated_string"))
	{
		d = &esvg_attribute_animated_string_descriptor;
	}
	else if (!strcmp(name, "esvg_animated_path_command"))
	{
		d = &esvg_attribute_animated_path_command_descriptor;
	}
	return d;
}

/*----------------------------------------------------------------------------*
 *                                  List                                      *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_list_add(Esvg_Attribute_Animated_List *aa,
	void *data,
	Eina_Bool animate)
{
	Esvg_Attribute_List *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	a->v = eina_list_append(a->v, data);
	a->is_set = EINA_TRUE;
}

void esvg_attribute_animated_list_get(Esvg_Attribute_Animated_List *aa,
	Esvg_Animated_List *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_animated_list_final_get(Esvg_Attribute_Animated_List *aa, Eina_List **v)
{
	if (aa->animated && aa->anim.is_set)
		*v = aa->anim.v;
	else
		*v = aa->base.v;
}
/*----------------------------------------------------------------------------*
 *                                Transform                                    *
 *----------------------------------------------------------------------------*/
void esvg_attribute_transform_set(Esvg_Attribute_Transform *a,
		const Enesim_Matrix *v, const Enesim_Matrix *def)
{
	if (!v)
	{
		esvg_attribute_transform_unset(a, def);
	}
	else
	{
		a->v = *v;
		a->is_set = EINA_TRUE;
	}
}

void esvg_attribute_transform_unset(Esvg_Attribute_Transform *a,
		const Enesim_Matrix *def)
{
	a->is_set = EINA_FALSE;
	a->v = *def;
}

void esvg_attribute_animated_transform_set(Esvg_Attribute_Animated_Transform *aa,
	const Esvg_Animated_Transform *v,
	const Enesim_Matrix *def,
	Eina_Bool animate)
{
	Esvg_Attribute_Transform *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_transform_set(a, &v->base, def);
	else
		esvg_attribute_transform_unset(a, def);
}

void esvg_attribute_animated_transform_get(Esvg_Attribute_Animated_Transform *aa,
	Esvg_Animated_Transform *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_animated_transform_final_get(Esvg_Attribute_Animated_Transform *aa, Enesim_Matrix *m)
{
	if (aa->animated && aa->anim.is_set)
		*m = aa->anim.v;
	else
		*m = aa->base.v;
}

Eina_Bool esvg_attribute_animated_transform_is_set(Esvg_Attribute_Animated_Transform *aa)
{
	return aa->animated ? EINA_TRUE : aa->base.is_set;
}
/*----------------------------------------------------------------------------*
 *                                  Color                                     *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_color_merge_rel(const Esvg_Attribute_Animated_Color *rel,
		const Esvg_Attribute_Animated_Color *v,
		Esvg_Attribute_Color *d)
{
	const Esvg_Attribute_Color *rr = NULL;
	const Esvg_Attribute_Color *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_color_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_color_merge(const Esvg_Attribute_Animated_Color *v,
		Esvg_Attribute_Color *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_color_merge_rel(const Esvg_Attribute_Color *rel,
		const Esvg_Attribute_Color *v,
		Esvg_Attribute_Color *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_color_set(Esvg_Attribute_Animated_Color *aa,
	const Esvg_Animated_Color *v,
	const Esvg_Color *def,
	Eina_Bool animate)
{
	Esvg_Attribute_Color *a;

	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_color_set(a, &v->base, def);
	else
		esvg_attribute_color_unset(a, def);
}

void esvg_attribute_animated_color_get(Esvg_Attribute_Animated_Color *aa,
	Esvg_Animated_Color *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_color_unset(Esvg_Attribute_Color *a, const Esvg_Color *def)
{
	a->v = *def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_color_set(Esvg_Attribute_Color *a, const Esvg_Color *v,
		const Esvg_Color *def)
{
	if (!v)
	{
		esvg_attribute_color_unset(a, def);
	}
	else
	{
		a->v = *v;
		a->is_set = EINA_TRUE;
	}
}
/*----------------------------------------------------------------------------*
 *                                 String                                     *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_string_merge_rel(const Esvg_Attribute_Animated_String *rel,
		const Esvg_Attribute_Animated_String *v,
		Esvg_Attribute_String *d)
{
	const Esvg_Attribute_String *rr = NULL;
	const Esvg_Attribute_String *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_string_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_string_merge(const Esvg_Attribute_Animated_String *v,
		Esvg_Attribute_String *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_string_merge_rel(const Esvg_Attribute_String *rel,
		const Esvg_Attribute_String *v,
		Esvg_Attribute_String *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_string_set(Esvg_Attribute_Animated_String *aa,
	const Esvg_Animated_String *v,
	Eina_Bool animate)
{
	Esvg_Attribute_String *a;

	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_string_set(a, v->base);
	else
		esvg_attribute_string_unset(a);
}

void esvg_attribute_animated_string_get(Esvg_Attribute_Animated_String *aa,
	Esvg_Animated_String *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_string_unset(Esvg_Attribute_String *a)
{
	if (a->v)
	{
		free(a->v);
		a->v = NULL;
		a->is_set = EINA_FALSE;
	}
}


void esvg_attribute_string_set(Esvg_Attribute_String *a, const char *v)
{
	if (a->v == v)
		return;

	esvg_attribute_string_unset(a);
	if (v)
	{
		a->v = strdup(v);
		a->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_string_final_get(Esvg_Attribute_Animated_String *aa, char **v)
{
	if (aa->animated && aa->anim.is_set)
		*v = aa->anim.v;
	else
		*v = aa->base.v;
}
/*----------------------------------------------------------------------------*
 *                                 Length                                     *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_length_merge_rel(const Esvg_Attribute_Animated_Length *rel,
		const Esvg_Attribute_Animated_Length *v,
		Esvg_Attribute_Length *d)
{
	const Esvg_Attribute_Length *rr = NULL;
	const Esvg_Attribute_Length *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_length_merge_rel(rr, vv, d);
}

/* FIXME rename the merge variants */
void esvg_attribute_animated_length_merge(const Esvg_Attribute_Animated_Length *v,
		Esvg_Attribute_Length *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_length_merge_rel(const Esvg_Attribute_Length *rel,
		const Esvg_Attribute_Length *v,
		Esvg_Attribute_Length *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_length_set(Esvg_Attribute_Animated_Length *aa,
	const Esvg_Animated_Length *v,
	const Esvg_Length *def,
	Eina_Bool animate)
{
	Esvg_Attribute_Length *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_length_set(a, &v->base, def);
	else
		esvg_attribute_length_unset(a, def);
}

void esvg_attribute_animated_length_get(Esvg_Attribute_Animated_Length *aa,
	Esvg_Animated_Length *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

Eina_Bool esvg_attribute_animated_length_is_set(Esvg_Attribute_Animated_Length *aa)
{
	if ((aa->animated && aa->anim.is_set) || (aa->base.is_set))
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

void esvg_attribute_animated_length_final_get(Esvg_Attribute_Animated_Length *aa,
	Esvg_Length *v)
{
	if (aa->animated && aa->anim.is_set)
		*v = aa->anim.v;
	else
		*v = aa->base.v;
}

void esvg_attribute_length_unset(Esvg_Attribute_Length *a, const Esvg_Length *def)
{
	a->v = *def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_length_set(Esvg_Attribute_Length *a, const Esvg_Length *v,
		const Esvg_Length *def)
{
	if (!v)
	{
		esvg_attribute_length_unset(a, def);
	}
	else
	{
		a->v = *v;
		a->is_set = EINA_TRUE;
	}
}

/*----------------------------------------------------------------------------*
 *                                  Bool                                      *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_bool_merge_rel(const Esvg_Attribute_Animated_Bool *rel,
		const Esvg_Attribute_Animated_Bool *v,
		Esvg_Attribute_Bool *d)
{
	const Esvg_Attribute_Bool *rr = NULL;
	const Esvg_Attribute_Bool *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_bool_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_bool_merge(const Esvg_Attribute_Animated_Bool *v,
		Esvg_Attribute_Bool *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_bool_merge_rel(const Esvg_Attribute_Bool *rel,
		const Esvg_Attribute_Bool *v,
		Esvg_Attribute_Bool *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_bool_set(Esvg_Attribute_Animated_Bool *aa,
	const Esvg_Animated_Bool *v,
	Eina_Bool def,
	Eina_Bool animate)
{
	Esvg_Attribute_Bool *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_bool_set(a, v->base);
	else
		esvg_attribute_bool_unset(a, def);
}

void esvg_attribute_animated_bool_get(Esvg_Attribute_Animated_Bool *aa,
	Esvg_Animated_Bool *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_bool_unset(Esvg_Attribute_Bool *a, Eina_Bool def)
{
	a->v = def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_bool_set(Esvg_Attribute_Bool *a, Eina_Bool v)
{
	a->v = v;
	a->is_set = EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                                 Number                                     *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_number_merge_rel(const Esvg_Attribute_Animated_Number *rel,
		const Esvg_Attribute_Animated_Number *v,
		Esvg_Attribute_Number *d)
{
	const Esvg_Attribute_Number *rr = NULL;
	const Esvg_Attribute_Number *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_number_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_number_merge(const Esvg_Attribute_Animated_Number *v,
		Esvg_Attribute_Number *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_number_merge_rel(const Esvg_Attribute_Number *rel,
		const Esvg_Attribute_Number *v,
		Esvg_Attribute_Number *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

/* TODO pass the possible range values */
void esvg_attribute_animated_number_set(Esvg_Attribute_Animated_Number *aa,
	const Esvg_Animated_Number *v,
	double def,
	Eina_Bool animate)
{
	Esvg_Attribute_Number *a;
	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_number_set(a, v->base);
	else
		esvg_attribute_number_unset(a, def);
}

void esvg_attribute_animated_number_get(Esvg_Attribute_Animated_Number *aa,
	Esvg_Animated_Number *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_number_unset(Esvg_Attribute_Number *a, double def)
{
	a->v = def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_number_set(Esvg_Attribute_Number *a, double v)
{
	a->v = v;
	a->is_set = EINA_TRUE;
}


/*----------------------------------------------------------------------------*
 *                                  Paint                                     *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_paint_merge_rel(const Esvg_Attribute_Animated_Paint *rel,
		const Esvg_Attribute_Animated_Paint *v,
		Esvg_Attribute_Paint *d)
{
	const Esvg_Attribute_Paint *rr = NULL;
	const Esvg_Attribute_Paint *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_paint_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_paint_merge(const Esvg_Attribute_Animated_Paint *v,
		Esvg_Attribute_Paint *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_paint_merge_rel(const Esvg_Attribute_Paint *rel,
		const Esvg_Attribute_Paint *v,
		Esvg_Attribute_Paint *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_animated_paint_set(Esvg_Attribute_Animated_Paint *aa,
	const Esvg_Animated_Paint *v,
	const Esvg_Paint *def,
	Eina_Bool animate)
{
	Esvg_Attribute_Paint *a;

	/* get the attribute to change */
	if (animate)
		a = &aa->anim;
	else
		a = &aa->base;
	/* get the value to set */
	if (v)
		esvg_attribute_paint_set(a, &v->base, def);
	else
		esvg_attribute_paint_unset(a, def);
}

void esvg_attribute_animated_paint_get(Esvg_Attribute_Animated_Paint *aa,
	Esvg_Animated_Paint *v)
{
	if (!v) return;

	v->base = aa->base.v;
	if (aa->animated && aa->anim.is_set)
		v->anim = aa->anim.v;
	else
		v->anim = v->base;
}

void esvg_attribute_paint_unset(Esvg_Attribute_Paint *a, const Esvg_Paint *def)
{
	a->v = *def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_paint_set(Esvg_Attribute_Paint *a, const Esvg_Paint *v,
		const Esvg_Paint *def)
{
	if (!v)
	{
		esvg_attribute_paint_unset(a, def);
	}
	else
	{
		a->v = *v;
		a->is_set = EINA_TRUE;
	}
}

/*----------------------------------------------------------------------------*
 *                                  Enum                                      *
 *----------------------------------------------------------------------------*/
void esvg_attribute_animated_enum_merge_rel(const Esvg_Attribute_Animated_Enum *rel,
		const Esvg_Attribute_Animated_Enum *v,
		Esvg_Attribute_Enum *d)
{
	const Esvg_Attribute_Enum *rr = NULL;
	const Esvg_Attribute_Enum *vv = NULL;

	if (v->animated && v->anim.is_set)
		vv = &v->anim;
	if (!vv)
		vv = &v->base;

	if (rel->animated && rel->anim.is_set)
		rr = &rel->anim;
	if (!rr)
		rr = &rel->base;

	esvg_attribute_enum_merge_rel(rr, vv, d);
}

void esvg_attribute_animated_enum_merge(const Esvg_Attribute_Animated_Enum *v,
		Esvg_Attribute_Enum *d)
{
	if (v->animated && v->anim.is_set)
	{
		d->v = v->anim.v;
		d->is_set = v->anim.is_set;
	}
	else
	{
		d->v = v->base.v;
		d->is_set = v->base.is_set;
	}
}

void esvg_attribute_enum_merge_rel(const Esvg_Attribute_Enum *rel,
		const Esvg_Attribute_Enum *v,
		Esvg_Attribute_Enum *d)
{
	if (!v->is_set)
	{
		d->v = rel->v;
		d->is_set = rel->is_set;
	}
	else
	{
		d->v = v->v;
		d->is_set = EINA_TRUE;
	}
}

void esvg_attribute_enum_unset(Esvg_Attribute_Enum *a, int def)
{
	a->v = def;
	a->is_set = EINA_FALSE;
}

void esvg_attribute_enum_set(Esvg_Attribute_Enum *a, int v)
{
	a->v = v;
	a->is_set = EINA_TRUE;
}
/*----------------------------------------------------------------------------*
 *                              Normal attributes                             *
 *----------------------------------------------------------------------------*/
#if 0
/* color default = black
 * opacity default = 1.0
 * fill default = black
 * stroke default = none
 * fill opacity > 1 && < 0
 * stroke opacity > 1 && < 0
 * stop color = black
 * stop opacity = 1.0
 */
 */
void esvg_attribute_fill_rule_set(Esvg_Attribute_Presentation *thiz, Esvg_Fill_Rule rule)
{
	thiz->fill_rule = rule;
	thiz->fill_rule_set = EINA_TRUE;
}

void esvg_attribute_fill_rule_unset(Esvg_Attribute_Presentation *thiz)
{
	thiz->fill_rule_set = EINA_FALSE;
}

void esvg_attribute_stroke_line_cap_set(Esvg_Attribute_Presentation *thiz, Esvg_Stroke_Line_Cap cap)
{
	thiz->stroke_line_cap = cap;
	thiz->stroke_line_cap_set = EINA_TRUE;
}

void esvg_attribute_stroke_line_cap_unset(Esvg_Attribute_Presentation *thiz)
{
	thiz->stroke_line_cap_set = EINA_FALSE;
}

void esvg_attribute_stroke_line_join_set(Esvg_Attribute_Presentation *thiz, Esvg_Stroke_Line_Join join)
{
	thiz->stroke_line_join = join;
	thiz->stroke_line_join_set = EINA_TRUE;
}

void esvg_attribute_stroke_line_join_unset(Esvg_Attribute_Presentation *thiz)
{
	thiz->stroke_line_join_set = EINA_FALSE;
}

#endif


