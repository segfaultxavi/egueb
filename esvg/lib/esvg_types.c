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

#include <stdlib.h>
#include <errno.h>

#include "esvg_private_main.h"
#include "esvg_private_attribute_presentation.h"
#include "esvg_parser_private.h" // remove this
#include "esvg_types.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_IS_HEXA(c) \
	(((c >= 48) && (c <= 57)) || \
	((c >= 65) && (c <= 70)) || \
	((c >= 97) && (c <= 102)))

#define ESVG_SPACE_SKIP(t) \
	while (*t) { if ((*t == ' ') || (*t == '\n') || (*t == '\t') || (*t == '\r')) t++; else break; }

#define ESVG_SPACE_COMMA_SKIP(t) \
	ESVG_SPACE_SKIP(t); \
	if (*t == ',') t++; \
	ESVG_SPACE_SKIP(t);
/*----------------------------------------------------------------------------*
 *                               Generic helpers                              *
 *----------------------------------------------------------------------------*/
/* parse a string of type funcname(a, b, .... numelements)
 * in, out numelements
 * in attr_val
 * out endptr
 * out elements
 */
static Eina_Bool _esvg_function_get(const char *attr_val, const char **endptr,
		const char *funcname, int *numelements, double *elements)
{
	int nvalues = 0;
	const char *tmp = attr_val;
	char *end;
	size_t sz;

	sz = strlen(funcname);
	ESVG_SPACE_SKIP(tmp);
	if (strncmp(tmp, funcname, sz) != 0)
		return EINA_FALSE;
	tmp += sz;
	ESVG_SPACE_SKIP(tmp);
	if (tmp[0] != '(')
		return EINA_FALSE;
	tmp++;
	while (*tmp)
	{
		double val;

		ESVG_SPACE_SKIP(tmp);
		if (tmp[0] == ')')
			goto end;
		val = strtod(tmp, &end);
		if (errno == ERANGE)
			val = 0;
		if (end == tmp)
			break;
		tmp = end;
		elements[nvalues] = val;
		nvalues++;
		/* if we passed the limit, break */
		if (nvalues >= *numelements)
			break;
		/* skip the comma and the blanks */
		ESVG_SPACE_COMMA_SKIP(tmp);
	}

	if (tmp[0] != ')')
		return EINA_FALSE;
end:
	tmp++;
	*numelements = nvalues;
	*endptr = tmp;

	return EINA_TRUE;
}

static Eina_Bool _esvg_long_get(const char *iter, const char **tmp, long *l)
{
	char *endptr;
	long val;

	val = strtol(iter, &endptr, 10);
	if ((errno != ERANGE) &&
	    !((val == 0) && (iter == endptr)))
	{
		*tmp = endptr;
		*l = val;
		return EINA_TRUE;
	}

	return EINA_FALSE;
}

static unsigned char _esvg_c_to_h(char c)
{
	unsigned char h;

	if ((c >= 48) && (c <= 57))
		h = c - 48;
	else if ((c >= 65) && (c <= 70))
		h = c - 55;
	else
		h = c - 87;
	return h;
}

static unsigned char _esvg_c_to_hh(char c)
{
	unsigned char h = _esvg_c_to_h(c);

	return h | ((h << 4) & 240);
}

static unsigned char _esvg_cc_to_hh(const char *cc)
{
	unsigned char h1;
	unsigned char h2;

	h1 = _esvg_c_to_h(cc[0]);
	h2 = _esvg_c_to_h(cc[1]);
	return h2 | ((h1 << 4) & 240);
}

static const char * _id_get(const char *uri)
{
	const char *tmp;
	const char *id;

	/* TODO split the uri, for local or relative */
	/* TODO get the id (#ElementId) */
	/* only check on the ids */
	for (tmp = uri; *tmp && *tmp != '#'; tmp++)

	if (*tmp != '#') return NULL;
	id = tmp + 1;

	return id;
}
/*----------------------------------------------------------------------------*
 *                           Color related functions                          *
 *----------------------------------------------------------------------------*/
Eina_Bool _esvg_color_keyword_from(Esvg_Color *color, const char *attr_val)
{
	static Eina_Hash *_colors = NULL;
	Enesim_Argb *argb;
	typedef struct _Esvg_Color_Entry
	{
		const char *name;
		Enesim_Argb color;
	} Esvg_Color_Entry;

	if (!_colors)
	{
		/* Css color table */
		Esvg_Color_Entry colors[] = {
			{ "aliceblue", 0xfff0f8ff },
			{ "antiquewhite", 0xfffaebd7 },
			{ "aqua", 0xff00ffff },
			{ "aquamarine", 0xff7fffd4 },
			{ "azure", 0xfff0ffff },
			{ "beige", 0xfff5f5dc },
			{ "bisque", 0xffffe4c4 },
			{ "black", 0xff000000 },
			{ "blanchedalmond", 0xffffebcd },
			{ "blue", 0xff0000ff },
			{ "blueviolet", 0xff8a2be2 },
			{ "brown", 0xffa52a2a },
			{ "burlywood", 0xffdeb887 },
			{ "cadetblue", 0xff5f9ea0 },
			{ "chartreuse", 0xff7fff00 },
			{ "chocolate", 0xffd2691e },
			{ "coral", 0xffff7f50 },
			{ "cornflowerblue", 0xff6495ed },
			{ "cornsilk", 0xfffff8dc },
			{ "crimson", 0xffdc143c },
			{ "cyan", 0xff00ffff },
			{ "darkblue", 0xff00008b },
			{ "darkcyan", 0xff008b8b },
			{ "darkgoldenrod", 0xffb8860b },
			{ "darkgray", 0xffa9a9a9 },
			{ "darkgrey", 0xffa9a9a9 },
			{ "darkgreen", 0xff006400 },
			{ "darkkhaki", 0xffbdb76b },
			{ "darkmagenta", 0xff8b008b },
			{ "darkolivegreen", 0xff556b2f },
			{ "darkorange", 0xffff8c00 },
			{ "darkorchid", 0xff9932cc },
			{ "darkred", 0xff8b0000 },
			{ "darksalmon", 0xffe9967a },
			{ "darkseagreen", 0xff8fbc8f },
			{ "darkslateblue", 0xff483d8b },
			{ "darkslategray", 0xff2f4f4f },
			{ "darkslategrey", 0xff2f4f4f },
			{ "darkturquoise", 0xff00ced1 },
			{ "darkviolet", 0xff9400d3 },
			{ "deeppink", 0xffff1493 },
			{ "deepskyblue", 0xff00bfff },
			{ "dimgray", 0xff696969 },
			{ "dimgrey", 0xff696969 },
			{ "dodgerblue", 0xff1e90ff },
			{ "firebrick", 0xffb22222 },
			{ "floralwhite", 0xfffffaf0 },
			{ "forestgreen", 0xff228b22 },
			{ "fuchsia", 0xffff00ff },
			{ "gainsboro", 0xffdcdcdc },
			{ "ghostwhite", 0xfff8f8ff },
			{ "gold", 0xffffd700 },
			{ "goldenrod", 0xffdaa520 },
			{ "gray", 0xff808080 },
			{ "grey", 0xff808080 },
			{ "green", 0xff008000 },
			{ "greenyellow", 0xffadff2f },
			{ "honeydew", 0xfff0fff0 },
			{ "hotpink", 0xffff69b4 },
			{ "indianred", 0xffcd5c5c },
			{ "indigo", 0xff4b0082 },
			{ "ivory", 0xfffffff0 },
			{ "khaki", 0xfff0e68c },
			{ "lavender", 0xffe6e6fa },
			{ "lavenderblush", 0xfffff0f5 },
			{ "lawngreen", 0xff7cfc00 },
			{ "lemonchiffon", 0xfffffacd },
			{ "lightblue", 0xffadd8e6 },
			{ "lightcoral", 0xfff08080 },
			{ "lightcyan", 0xffe0ffff },
			{ "lightgoldenrodyellow", 0xfffafad2 },
			{ "lightgray", 0xffd3d3d3 },
			{ "lightgrey", 0xffd3d3d3 },
			{ "lightgreen", 0xff90ee90 },
			{ "lightpink", 0xffffb6c1 },
			{ "lightsalmon", 0xffffa07a },
			{ "lightseagreen", 0xff20b2aa },
			{ "lightskyblue", 0xff87cefa },
			{ "lightslategray", 0xff778899 },
			{ "lightslategrey", 0xff778899 },
			{ "lightsteelblue", 0xffb0c4de },
			{ "lightyellow", 0xffffffe0 },
			{ "lime", 0xff00ff00 },
			{ "limegreen", 0xff32cd32 },
			{ "linen", 0xfffaf0e6 },
			{ "magenta", 0xffff00ff },
			{ "maroon", 0xff800000 },
			{ "mediumaquamarine", 0xff66cdaa },
			{ "mediumblue", 0xff0000cd },
			{ "mediumorchid", 0xffba55d3 },
			{ "mediumpurple", 0xff9370d8 },
			{ "mediumseagreen", 0xff3cb371 },
			{ "mediumslateblue", 0xff7b68ee },
			{ "mediumspringgreen", 0xff00fa9a },
			{ "mediumturquoise", 0xff48d1cc },
			{ "mediumvioletred", 0xffc71585 },
			{ "midnightblue", 0xff191970 },
			{ "mintcream", 0xfff5fffa },
			{ "mistyrose", 0xffffe4e1 },
			{ "moccasin", 0xffffe4b5 },
			{ "navajowhite", 0xffffdead },
			{ "navy", 0xff000080 },
			{ "oldlace", 0xfffdf5e6 },
			{ "olive", 0xff808000 },
			{ "olivedrab", 0xff6b8e23 },
			{ "orange", 0xffffa500 },
			{ "orangered", 0xffff4500 },
			{ "orchid", 0xffda70d6 },
			{ "palegoldenrod", 0xffeee8aa },
			{ "palegreen", 0xff98fb98 },
			{ "paleturquoise", 0xffafeeee },
			{ "palevioletred", 0xffd87093 },
			{ "papayawhip", 0xffffefd5 },
			{ "peachpuff", 0xffffdab9 },
			{ "peru", 0xffcd853f },
			{ "pink", 0xffffc0cb },
			{ "plum", 0xffdda0dd },
			{ "powderblue", 0xffb0e0e6 },
			{ "purple", 0xff800080 },
			{ "red", 0xffff0000 },
			{ "rosybrown", 0xffbc8f8f },
			{ "royalblue", 0xff4169e1 },
			{ "saddlebrown", 0xff8b4513 },
			{ "salmon", 0xfffa8072 },
			{ "sandybrown", 0xfff4a460 },
			{ "seagreen", 0xff2e8b57 },
			{ "seashell", 0xfffff5ee },
			{ "sienna", 0xffa0522d },
			{ "silver", 0xffc0c0c0 },
			{ "skyblue", 0xff87ceeb },
			{ "slateblue", 0xff6a5acd },
			{ "slategray", 0xff708090 },
			{ "slategrey", 0xff708090 },
			{ "snow", 0xfffffafa },
			{ "springgreen", 0xff00ff7f },
			{ "steelblue", 0xff4682b4 },
			{ "tan", 0xffd2b48c },
			{ "teal", 0xff008080 },
			{ "thistle", 0xffd8bfd8 },
			{ "tomato", 0xffff6347 },
			{ "turquoise", 0xff40e0d0 },
			{ "violet", 0xffee82ee },
			{ "wheat", 0xfff5deb3 },
			{ "white", 0xffffffff },
			{ "whitesmoke", 0xfff5f5f5 },
			{ "yellow", 0xffffff00 },
			{ "yellowgreen", 0xff9acd32 }
		};
		unsigned int i;

		_colors = eina_hash_string_superfast_new(NULL);
		for (i = 0; i < sizeof(colors) / sizeof(Esvg_Color_Entry); i++)
		{
			argb = malloc(sizeof(Enesim_Argb));
			*argb = colors[i].color;
			eina_hash_add(_colors, colors[i].name, argb);
		}
	}
	/* convert the attr to lowercase */
	argb = eina_hash_find(_colors, attr_val);

	if (!argb) return EINA_FALSE;

	color->r = (*argb >> 16) & 0xff;
	color->g = (*argb >> 8) & 0xff;
	color->b = (*argb & 0xff);

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                     Transformation related functions                       *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_transformation_matrix_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	int numelements = 6;
	double mx[6];

	if (!_esvg_function_get(attr_val, endptr, "matrix", &numelements, mx))
		return EINA_FALSE;
	if (numelements < 6)
		return EINA_FALSE;
	enesim_matrix_values_set(matrix, mx[0], mx[2], mx[4], mx[1], mx[3], mx[5], 0, 0, 1);

	return EINA_TRUE;
}

static Eina_Bool _esvg_transformation_translate_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	int numelements = 2;
	double tx[2];

	if (!_esvg_function_get(attr_val, endptr, "translate", &numelements, tx))
		return EINA_FALSE;
	if (numelements < 1)
		return EINA_FALSE;
	/* on translation the tx is mandatory but not ty */
	if (numelements == 1)
		tx[1] = 0;
	enesim_matrix_translate(matrix, tx[0], tx[1]);

	return EINA_TRUE;
}

static Eina_Bool _esvg_transformation_skewx_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double angle;
	int numelements = 1;

	if (!_esvg_function_get(attr_val, endptr, "skewX", &numelements, &angle))
		return EINA_FALSE;
	if (numelements < 1)
		return EINA_FALSE;

	enesim_matrix_values_set(matrix, 1, tan(angle * M_PI / 180.0), 0, 0, 1, 0, 0, 0, 1);
	return EINA_TRUE;
}

static Eina_Bool _esvg_transformation_skewy_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double angle;
	int numelements = 1;

	if (!_esvg_function_get(attr_val, endptr, "skewY", &numelements, &angle))
		return EINA_FALSE;
	if (numelements < 1)
		return EINA_FALSE;

	enesim_matrix_values_set(matrix, 1, 0, 0, tan(angle * M_PI / 180.0), 1, 0, 0, 0, 1);
	return EINA_TRUE;
}

static Eina_Bool _esvg_transformation_scale_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double sx[2];
	int numelements = 2;

	if (!_esvg_function_get(attr_val, endptr, "scale", &numelements, sx))
		return EINA_FALSE;
	if (numelements < 1)
		return EINA_FALSE;
	if (numelements == 1)
		sx[1] = sx[0];
	enesim_matrix_scale(matrix, sx[0], sx[1]);

	return EINA_TRUE;
}


static Eina_Bool _esvg_transformation_rotate_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double rx[3];
	int numelements = 3;

	if (!_esvg_function_get(attr_val, endptr, "rotate", &numelements, rx))
		return EINA_FALSE;
	if (numelements < 1)
		return EINA_FALSE;
	enesim_matrix_rotate(matrix, rx[0] * M_PI / 180.0);
	/* handle the origin */
	if (numelements > 1)
	{
		Enesim_Matrix tx;

		if (numelements < 3)
			return EINA_FALSE;
		enesim_matrix_translate(&tx, -rx[1], -rx[2]);
		enesim_matrix_compose(&tx, matrix, matrix);
		enesim_matrix_translate(&tx, rx[1], rx[2]);
		enesim_matrix_compose(matrix, &tx, matrix);
	}

	return EINA_TRUE;
}

/*----------------------------------------------------------------------------*
 *                           Path related functions                           *
 *----------------------------------------------------------------------------*/
static Eina_Bool _esvg_path_number_get(char **attr, double *x)
{
	char *iter;
	char *endptr;

	iter = *attr;
	ESVG_SPACE_COMMA_SKIP(iter);
	*x = strtod(iter, &endptr);
	if (iter == endptr)
		return EINA_FALSE;

	*attr = endptr;

	return EINA_TRUE;
}

static Eina_Bool _esvg_path_flag_get(char **attr, Eina_Bool *b)
{
	char *iter;

	iter = *attr;
	ESVG_SPACE_COMMA_SKIP(iter);
	if (*iter == '0')
	{
		*b = EINA_FALSE;
	}
	else if (*iter == '1')
	{
		*b = EINA_TRUE;
	}
	else
	{
		ERR("can not convert flag");
		return EINA_FALSE;
	}
	*attr = iter + 1;
	return EINA_TRUE;
}

static Eina_Bool _esvg_path_point_get(char **attr, Esvg_Point *p)
{
	if (!_esvg_path_number_get(attr, &p->x))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	if (!_esvg_path_number_get(attr, &p->y))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_line_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point p;

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}
	
	cmd->type = ESVG_PATH_LINE_TO;
	cmd->relative = relative;
	cmd->data.line_to.x = p.x;
	cmd->data.line_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_move_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point p;

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}
	cmd->type = ESVG_PATH_MOVE_TO;
	cmd->relative = relative;
	cmd->data.move_to.x = p.x;
	cmd->data.move_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_hline_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	double c;

	if (!_esvg_path_number_get(value, &c))
	{
		ERR("Can not get coord");
		return EINA_FALSE;
	}
	cmd->type = ESVG_PATH_HLINE_TO;
	cmd->relative = relative;
	cmd->data.hline_to.c = c;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_vline_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	double c;

	if (!_esvg_path_number_get(value, &c))
	{
		ERR("Can not get coord");
		return EINA_FALSE;
	}
	cmd->type = ESVG_PATH_VLINE_TO;
	cmd->relative = relative;
	cmd->data.hline_to.c = c;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_cubic_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point ctrl0, ctrl1, p;

	if (!_esvg_path_point_get(value, &ctrl0))
	{
		ERR("Can not get control point %s", *value);
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(value, &ctrl1))
	{
		ERR("Can not get control point");
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}

	cmd->type = ESVG_PATH_CUBIC_TO;
	cmd->relative = relative;
	cmd->data.cubic_to.ctrl_x0 = ctrl0.x;
	cmd->data.cubic_to.ctrl_y0 = ctrl0.y;
	cmd->data.cubic_to.ctrl_x1 = ctrl1.x;
	cmd->data.cubic_to.ctrl_y1 = ctrl1.y;
	cmd->data.cubic_to.x = p.x;
	cmd->data.cubic_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_scubic_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point ctrl, p;

	if (!_esvg_path_point_get(value, &ctrl))
	{
		ERR("Can not get control point");
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}

	cmd->type = ESVG_PATH_SCUBIC_TO;
	cmd->relative = relative;
	cmd->data.scubic_to.ctrl_x = ctrl.x;
	cmd->data.scubic_to.ctrl_y = ctrl.y;
	cmd->data.scubic_to.x = p.x;
	cmd->data.scubic_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_quadratic_to(	Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point ctrl, p;

	if (!_esvg_path_point_get(value, &ctrl))
	{
		ERR("Can not get control point");
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}

	cmd->type = ESVG_PATH_QUADRATIC_TO;
	cmd->relative = relative;
	cmd->data.quadratic_to.ctrl_x = ctrl.x;
	cmd->data.quadratic_to.ctrl_y = ctrl.y;
	cmd->data.quadratic_to.x = p.x;
	cmd->data.quadratic_to.y = p.y;

	return EINA_TRUE;

}

static Eina_Bool esvg_parser_path_squadratic_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point p;

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}
	cmd->type = ESVG_PATH_SQUADRATIC_TO;
	cmd->relative = relative;
	cmd->data.squadratic_to.x = p.x;
	cmd->data.squadratic_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_arc_to(Eina_Bool relative,
		char **value, Esvg_Path_Command *cmd)
{
	Esvg_Point p, radii;
	Eina_Bool large, sweep;
	double angle;

	if (!_esvg_path_point_get(value, &radii))
	{
		ERR("can not get radii");
		return EINA_FALSE;
	}

	if (!_esvg_path_number_get(value, &angle))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	if (!_esvg_path_flag_get(value, &large))
	{
		ERR("can not convert the large flag");
		return EINA_FALSE;
	}

	if (!_esvg_path_flag_get(value, &sweep))
	{
		ERR("can not convert the sweep flag");
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(value, &p))
	{
		ERR("Can not get point");
		return EINA_FALSE;
	}

	cmd->type = ESVG_PATH_ARC_TO;
	cmd->relative = relative;
	cmd->data.arc_to.rx = radii.x;
	cmd->data.arc_to.ry = radii.y;
	cmd->data.arc_to.angle = angle;
	cmd->data.arc_to.large = large;
	cmd->data.arc_to.sweep = sweep;
	cmd->data.arc_to.x = p.x;
	cmd->data.arc_to.y = p.y;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_path_close(char **value,
		Esvg_Path_Command *cmd)
{
	cmd->type = ESVG_PATH_CLOSE;
	cmd->relative = EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool esvg_parser_command(char command, char **value,
		Esvg_Path_Command *cmd)
{
	Eina_Bool ret = EINA_TRUE;

	switch (command)
	{
		case 'L':
		ret = esvg_parser_path_line_to(EINA_FALSE, value, cmd);
		break;

		case 'l':
		ret = esvg_parser_path_line_to(EINA_TRUE, value, cmd);
		break;

		case 'M':
		ret = esvg_parser_path_move_to(EINA_FALSE, value, cmd);
		break;

		case 'm':
		ret = esvg_parser_path_move_to(EINA_TRUE, value, cmd);
		break;

		case 'H':
		ret = esvg_parser_path_hline_to(EINA_FALSE, value, cmd);
		break;

		case 'h':
		ret = esvg_parser_path_hline_to(EINA_TRUE, value, cmd);
		break;

		case 'V':
		ret = esvg_parser_path_vline_to(EINA_FALSE, value, cmd);
		break;

		case 'v':
		ret = esvg_parser_path_vline_to(EINA_TRUE, value, cmd);
		break;

		case 'C':
		ret = esvg_parser_path_cubic_to(EINA_FALSE, value, cmd);
		break;

		case 'c':
		ret = esvg_parser_path_cubic_to(EINA_TRUE, value, cmd);
		break;

		case 'S':
		ret = esvg_parser_path_scubic_to(EINA_FALSE, value, cmd);
		break;

		case 's':
		ret = esvg_parser_path_scubic_to(EINA_TRUE, value, cmd);
		break;

		case 'Q':
		ret = esvg_parser_path_quadratic_to(EINA_FALSE, value, cmd);
		break;

		case 'q':
		ret = esvg_parser_path_quadratic_to(EINA_TRUE, value, cmd);
		break;

		case 'T':
		ret = esvg_parser_path_squadratic_to(EINA_FALSE, value, cmd);
		break;

		case 't':
		ret = esvg_parser_path_squadratic_to(EINA_TRUE, value, cmd);
		break;

		case 'A':
		ret = esvg_parser_path_arc_to(EINA_FALSE, value, cmd);
		break;

		case 'a':
		ret = esvg_parser_path_arc_to(EINA_TRUE, value, cmd);
		break;

		case 'z':
		case 'Z':
		ret = esvg_parser_path_close(value, cmd);
		break;

		default:
		ret = EINA_FALSE;
		break;
	}
	return ret;
}


/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Esvg_Length ESVG_LENGTH_0 = { 0.0, ESVG_UNIT_LENGTH_PX };
Esvg_Length ESVG_LENGTH_1 = { 1.0, ESVG_UNIT_LENGTH_PX };
Esvg_Length ESVG_LENGTH_100_PERCENT = { 100.0, ESVG_UNIT_LENGTH_PERCENT };

Esvg_Stroke_Line_Cap esvg_stroke_line_cap_get(const char *value)
{
	Esvg_Stroke_Line_Cap stroke_line_cap = ESVG_LINE_CAP_BUTT;

	if (!strcmp(value, "butt"))
		stroke_line_cap = ESVG_LINE_CAP_BUTT;
	else if (!strcmp(value, "round"))
		stroke_line_cap = ESVG_LINE_CAP_ROUND;
	else if (!strcmp(value, "square"))
		stroke_line_cap = ESVG_LINE_CAP_SQUARE;

	return stroke_line_cap;
}

Esvg_Stroke_Line_Join esvg_stroke_line_join_get(const char *value)
{
	Esvg_Stroke_Line_Join stroke_line_join = ESVG_LINE_JOIN_MITER;

	if (!strcmp(value, "miter"))
		stroke_line_join = ESVG_LINE_JOIN_MITER;
	else if (!strcmp(value, "round"))
		stroke_line_join = ESVG_LINE_JOIN_ROUND;
	else if (!strcmp(value, "bevel"))
		stroke_line_join = ESVG_LINE_JOIN_BEVEL;

	return stroke_line_join;
}

/* x1,y1 x2,y2 ... */
void esvg_parser_points(const char *value, Esvg_Parser_Points_Cb cb, void *data)
{
	const char *tmp;
	char *endptr;

	ESVG_SPACE_SKIP(value);
	tmp = value;
	while (*tmp)
	{
		Esvg_Point p;

		p.x = strtod(tmp, &endptr);
		tmp = endptr;
		ESVG_SPACE_COMMA_SKIP(tmp);
		p.y = strtod(tmp, &endptr);
		tmp = endptr;
		ESVG_SPACE_COMMA_SKIP(tmp);

		cb(&p, data);
	}
}

/* FIXME: fix parsing with ' ' and ',' (do like rgb(c,c,c)) */
Esvg_View_Box esvg_view_box_get(const char *attr_val)
{
	Esvg_View_Box vb = { 0, 0, 0, 0 };
	const char *iter;
	const char *tmp;
	char *endptr;
	double val;
	double *vbp;
	int nbr = 0;

        iter = tmp = attr_val;
	vbp = (double *)&vb;
	while (*tmp)
	{
		while (*tmp)
		{
			if ((*tmp == ' ') || (*tmp == ','))
				tmp++;
			else
			{
				iter = tmp;
				break;
			}
		}
		val = strtod(iter, &endptr);
		if ((errno != ERANGE) &&
		    !((val == 0) && (attr_val == endptr)))
		{
			*vbp = val;
			vbp++;
			tmp = endptr;
			nbr++;
			/* we store only the 4 first numbers */
			if (nbr >= 4)
			{
				return vb;
			}
		}
	}

	return vb;
}

/*
 * #rgb
 * #rrggbb
 * rgb(c, c, c)
 */
Eina_Bool esvg_color_get(Esvg_Color *color, const char *attr_val)
{
	size_t sz;

	sz = strlen(attr_val);

	/*
	 * check if it starts with the #:
	 * #rgb
	 * #rrggbb
	 */
	if (attr_val[0] == '#')
	{
		if (sz == 4)
		{
			if (ESVG_IS_HEXA(attr_val[1]) &&
			    ESVG_IS_HEXA(attr_val[2]) &&
			    ESVG_IS_HEXA(attr_val[3]))
			{
				color->r = _esvg_c_to_hh(attr_val[1]);
				color->g = _esvg_c_to_hh(attr_val[2]);
				color->b = _esvg_c_to_hh(attr_val[3]);
				return EINA_TRUE;
			}
		}
		else if (sz == 7)
		{
			if (ESVG_IS_HEXA(attr_val[1]) &&
			    ESVG_IS_HEXA(attr_val[2]) &&
			    ESVG_IS_HEXA(attr_val[3]) &&
			    ESVG_IS_HEXA(attr_val[4]) &&
			    ESVG_IS_HEXA(attr_val[5]) &&
			    ESVG_IS_HEXA(attr_val[6]))
			{
				color->r = _esvg_cc_to_hh(attr_val + 1);
				color->g = _esvg_cc_to_hh(attr_val + 3);
				color->b = _esvg_cc_to_hh(attr_val + 5);
				return EINA_TRUE;
			}
		}
		return EINA_FALSE;
	}
	/*
	 * check if it starts with the rbg(:
	 * rgb(c,c,c) (size at least 10)
	 */
	else if (sz >= 10)
	{
		/* starts with rgb( and finish with ) ? */
		if ((attr_val[0] == 'r') &&
		    (attr_val[1] == 'g') &&
		    (attr_val[2] == 'b') &&
		    (attr_val[3] == '(') &&
		    (attr_val[sz - 1] == ')'))
		{
			unsigned char cl[3];
			const char *iter;
			const char *tmp;
			long val;
			int nbr = 0;

			/* n, n, n */
			/* n%, n%, n% */
			iter = tmp = attr_val + 4;
			while (*tmp)
			{
				ESVG_SPACE_SKIP(tmp);
				if (*tmp == ',')
				{
                                        tmp++;
					iter = tmp;
				}
				if (_esvg_long_get(iter, &tmp, &val))
				{
					if ((*tmp != ' ') && (*tmp != ',') && (*tmp != '%') && (*tmp != ')'))
						break;
					if ((val >= 0) && (val <= 255))
					{
						if (*tmp == '%')
						{
							tmp++;
							cl[nbr] = (unsigned char)((255L * val) / 100L);
						}
						else
						{
							cl[nbr] = (unsigned char)val;
						}
						nbr++;
                                        }
                                        if (nbr == 3)
                                        {
						/* check the last parameter */
						ESVG_SPACE_SKIP(tmp);
						if ((*tmp == ')') && (*(tmp + 1) == '\0'))
						{
							/* parsing is correct, we set the color and return */
							color->r = cl[0];
							color->g = cl[1];
							color->b = cl[2];
							return EINA_TRUE;
						}
                                        }
				}
                                else
					break;
			}
		}
		return EINA_FALSE;
	}
	/* is a keyword */
	else
	{
		return _esvg_color_keyword_from(color, attr_val);
	}
}

Eina_Bool esvg_href_get(Edom_Tag **tag, Edom_Tag *rel, const char *href)
{
	Edom_Tag *topmost;
	Edom_Tag *ret_tag;
	const char *id;

	topmost = edom_tag_topmost_get(rel);
	if (!topmost)
	{
		printf("WTF?\n");
		return EINA_FALSE;
	}

	id = _id_get(href);
#if 0
	/* FIXME pass a cb to get the object? */
	/* get the tag from the specified uri */
	ret_tag = esvg_parser_svg_tag_find(topmost, id);
	if (!ret_tag)
	{
		printf("tag not found %s!!!!!!\n", id);
		return EINA_FALSE;
	}
	*tag = ret_tag;
#endif

	return EINA_TRUE;
}

Eina_Bool esvg_parser_gradient_units_get(Esvg_Gradient_Units *gu, const char *attr)
{
	if (strncmp(attr, "userSpaceOnUse", 14) == 0)
	{
		*gu = ESVG_USER_SPACE_ON_USE;
	}
	else if (strncmp(attr, "objectBoundingBox", 17) == 0)
	{
		*gu = ESVG_OBJECT_BOUNDING_BOX;
	}
	else
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

Eina_Bool esvg_parser_fill_rule_get(Esvg_Fill_Rule *rule, const char *attr)
{
	if (strncmp(attr, "nonzero", 7) == 0)
	{
		*rule = ESVG_NON_ZERO;
	}
	else if (strncmp(attr, "evenodd", 7) == 0)
	{
		*rule = ESVG_EVEN_ODD;
	}
	else
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

Eina_Bool esvg_parser_spread_method_get(Esvg_Spread_Method *smethod, const char *attr)
{
	if (strncmp(attr, "pad", 3) == 0)
	{
		*smethod = ESVG_SPREAD_METHOD_PAD;
	}
	else if (strncmp(attr, "reflect", 7) == 0)
	{
		*smethod = ESVG_SPREAD_METHOD_REFLECT;
	}
	else if (strncmp(attr, "repeat", 6) == 0)
	{
		*smethod = ESVG_SPREAD_METHOD_REPEAT;
	}
	else
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

Eina_Bool esvg_transformation_get(Enesim_Matrix *matrix, const char *attr)
{
	Eina_Bool ret;
	const char *endptr = NULL;
	typedef Eina_Bool (*Matrix_Get)(Enesim_Matrix *matrix, const char *attr_val, const char **endptr);

	Matrix_Get m[6] = {
		_esvg_transformation_matrix_get,
		_esvg_transformation_translate_get,
		_esvg_transformation_rotate_get,
		_esvg_transformation_scale_get,
		_esvg_transformation_skewx_get,
		_esvg_transformation_skewy_get,
	};
	enesim_matrix_identity(matrix);
	do
	{
		Enesim_Matrix parsed;
		int i;

		enesim_matrix_identity(&parsed);
		for (i = 0; i < 6; i++)
		{
			ret = m[i](&parsed, attr, &endptr);
			if (ret) break;
		}
		if (ret)
		{
			printf("found, composing\n");
			enesim_matrix_compose(matrix, &parsed, matrix);
		}
		attr = endptr;
	}
	while (endptr && *endptr && ret);

	return ret;
}

Eina_Bool esvg_parser_path(const char *value, Esvg_Parser_Command_Cb cb, void *data)
{
	Eina_Bool ret = EINA_TRUE;
	Eina_Bool first = EINA_TRUE;
	char last_command = 0;
	char *iter = (char *)value;

	if (!cb) return EINA_FALSE;

	ESVG_SPACE_SKIP(iter);
	/* First char must be 'M' or 'm' */
	if ((*iter != 'M') &&
	    (*iter != 'm'))
	{
		ERR("First char not 'M' or 'm'");
		return EINA_FALSE;
	}
	while (*iter)
	{
		Esvg_Path_Command cmd;
		char command;

 		command = *iter;
		iter++;
		ret = esvg_parser_command(command, &iter, &cmd);
		if (!ret)
		{
			/* try with the last command */
			iter--;
			ret = esvg_parser_command(last_command, &iter, &cmd);
			if (ret)
			{
				cb(&cmd, data);
			}
		}
		else
		{
			/* everything went ok, update the last command */
			last_command = command;
			cb(&cmd, data);
		}

		if (!ret)
		{
			ERR("Unsupported path data instruction (%c) %s", command, iter);
			break;
		}
		/* for the 'move' case the next elements should be lines */
		if ((command == 'm' || command == 'M') && first)
		{
			/* the next commands should be lines */
			if (command == 'm')
				last_command = 'l';
			else
				last_command = 'L';
		}
		first = EINA_FALSE;
		ESVG_SPACE_COMMA_SKIP(iter);
	}
	return ret;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI double esvg_number_string_from(const char *attr_val, double default_nbr)
{
	char *endptr;
	double val = default_nbr;

	if (!attr_val || !*attr_val)
		return val;

	val = strtod(attr_val, &endptr);
	if (errno == ERANGE)
		return val;
	if ((val == 0) && (attr_val == endptr))
		return val;

	/* else, conversion has been done */
	if ((endptr == NULL) || (*endptr == '\0'))
	{
		return val;
	}

	ERR("Number %s is invalid", attr_val);
	return default_nbr;
}

EAPI Eina_Bool esvg_length_string_from(Esvg_Length *length, const char *attr_val, Esvg_Length default_length)
{
	char *endptr;
	double val;

	*length = default_length;
	if (!attr_val || !*attr_val)
		return EINA_FALSE;

	val = strtod(attr_val, &endptr);
	if (errno == ERANGE)
		return EINA_FALSE;
	if ((val == 0) && (attr_val == endptr))
		return EINA_FALSE;

	/* else, conversion has been done */
	if ((endptr == NULL) || (*endptr == '\0'))
	{
		length->value = val;
		length->unit = ESVG_UNIT_LENGTH_PX;
		return EINA_TRUE;
	}

	/* strlen(endptr) >= 1 */
	if (endptr[1] == '\0')
	{
		if (endptr[0] == '%')
		{
			length->value = val;
			length->unit = ESVG_UNIT_LENGTH_PERCENT;
		}
	}
	else if (endptr[2] == '\0')
	{
		if (endptr[0] == 'e')
		{
			if (endptr[1] == 'm')
			{
				length->value = val;
				length->unit = ESVG_UNIT_LENGTH_EM;
			}
			else if (endptr[1] == 'x')
			{
				length->value = val;
				length->unit = ESVG_UNIT_LENGTH_EX;
			}
		}
		else if (endptr[0] == 'p')
		{
			if (endptr[1] == 'c')
			{
				length->value = val;
				length->unit = ESVG_UNIT_LENGTH_PC;
			}
			else if (endptr[1] == 't')
			{
				length->value = val;
				length->unit = ESVG_UNIT_LENGTH_PT;
			}
			else if (endptr[1] == 'x')
			{
				length->value = val;
				length->unit = ESVG_UNIT_LENGTH_PX;
			}
		}
		else if ((endptr[0] == 'c') && (endptr[1] == 'm'))
		{
			length->value = val;
			length->unit = ESVG_UNIT_LENGTH_CM;
		}
		else if ((endptr[0] == 'm') && (endptr[1] == 'm'))
		{
			length->value = val;
			length->unit = ESVG_UNIT_LENGTH_MM;
		}
		else if ((endptr[0] == 'i') && (endptr[1] == 'n'))
		{
			length->value = val;
			length->unit = ESVG_UNIT_LENGTH_IN;
		}
	}
	return EINA_TRUE;
}

EAPI Eina_Bool esvg_length_is_equal(Esvg_Length *length1, Esvg_Length *length2)
{
	if (!length1 || !length2)
		return EINA_FALSE;
	if ((length1->value == length2->value) &&(length1->unit == length2->unit))
		return EINA_TRUE;

	return EINA_FALSE;
}

EAPI double esvg_length_final_get(const Esvg_Length *l, double parent_length)
{
	double ret;
	/* Here we should transform the length/coord to an absolute
	 * value. For some cases we'll need the dpi, should it be global of
	 * the lib? per svg instance?
	 */
	/* FIXME make this numbers preprocessor macros */
	switch (l->unit)
	{
		case ESVG_UNIT_LENGTH_PERCENT:
		ret = (parent_length * l->value) / 100.0;
		break;

		case ESVG_UNIT_LENGTH_PT:
		ret = 1.25 * l->value;
		break;

		case ESVG_UNIT_LENGTH_PC:
		ret = 15 * l->value;
		break;

		case ESVG_UNIT_LENGTH_CM:
		ret = 35.43307 * l->value;
		break;

		case ESVG_UNIT_LENGTH_MM:
		ret = 3.543307 * l->value;
		break;

		case ESVG_UNIT_LENGTH_IN:
		ret = 90 * l->value;
		break;

		case ESVG_UNIT_LENGTH_PX:
		/* FIXME this one depends on the font size */
		case ESVG_UNIT_LENGTH_EM:
		case ESVG_UNIT_LENGTH_EX:
		default:
		ret = l->value;
		break;
	}

	return ret;
}

EAPI Eina_Bool esvg_string_is_uri(const char *attr)
{
	if (strncmp(attr, "url(", 4))
		return EINA_FALSE;
	return EINA_TRUE;
}

/*
 * [ <absoluteURI> | <relativeURI> ] [ "#" <elementID> ]
 */
EAPI void * esvg_uri_string_from(const char *attr, Esvg_Uri_Descriptor *descriptor, void *data)
{
	char url[PATH_MAX];
	size_t len;
	const char *id;

	if (!esvg_string_is_uri(attr))
		return NULL;

	len = strlen(attr) - 5;
	strncpy(url, attr + 4, len);
	url[len] = '\0';

	id = _id_get(url);
	if (!id) return NULL;

	return descriptor->local_get(id, data);
}


/*
 * none, currentColor, <color>, <uri>?
 */
EAPI Eina_Bool esvg_paint_string_from(Esvg_Paint *paint, const char *attr)
{
	Edom_Tag *found_tag;

	/* none */
	if (strncmp(attr, "none", 4) == 0)
	{
		paint->type = ESVG_PAINT_NONE;
	}
	/* currentColor */
	else if (strncmp(attr, "currentColor", 12) == 0)
	{
		paint->type = ESVG_PAINT_CURRENT_COLOR;
	}
	/* color name */
	else if (esvg_color_get(&paint->value.color, attr))
	{
		paint->type = ESVG_PAINT_COLOR;
	}
	/* uri */
	else if (esvg_string_is_uri(attr))
	{
		paint->type = ESVG_PAINT_SERVER;
		paint->value.paint_server = strdup(attr);
	}
	else
	{
		return EINA_FALSE;
	}

	return EINA_TRUE;
}


EAPI const char * esvg_type_string_to(Esvg_Type type)
{
	switch (type)
	{
		case ESVG_A: return "a";
		case ESVG_ALTGLYPH: return "";
		case ESVG_ALTGLYPHDEF: return "";
		case ESVG_ALTGLYPHITEM: return "";
		case ESVG_ANIMATE: return "animate";
		case ESVG_ANIMATECOLOR: return "animateColor";
		case ESVG_ANIMATEMOTION: return "animateMotion";
		case ESVG_ANIMATETRANSFORM: return "animateTransform";
		case ESVG_CIRCLE: return "circle";
		case ESVG_CLIPPATH: return "clipPath";
		case ESVG_COLOR_PROFILE: return "" /* 10 */;
		case ESVG_CURSOR: return "cursor";
		case ESVG_DEFINITION_SRC: return "";
		case ESVG_DEFS: return "defs";
		case ESVG_DESC: return "desc";
		case ESVG_ELLIPSE: return "ellipse";
		case ESVG_FEBLEND: return "feBlend";
		case ESVG_FECOLORMATRIX: return "";
		case ESVG_FECOMPONENTTRANSFER: return "";
		case ESVG_FECOMPOSITE: return "";
		case ESVG_FECONVOLVEMATRIX: return "" /* 20 */;
		case ESVG_FEDIFFUSELIGHTING: return "";
		case ESVG_FEDISPLACEMENTMAP: return "";
		case ESVG_FEDISTANTLIGHT: return "";
		case ESVG_FEFLOOD: return "";
		case ESVG_FEFUNCA: return "";
		case ESVG_FEFUNCB: return "";
		case ESVG_FEFUNCG: return "";
		case ESVG_FEFUNCR: return "";
		case ESVG_FEGAUSSIANBLUR: return "";
		case ESVG_FEIMAGE: return "" /* 30 */;
		case ESVG_FEMERGE: return "";
		case ESVG_FEMERGENODE: return "";
		case ESVG_FEMORPHOLOGY: return "";
		case ESVG_FEOFFSET: return "";
		case ESVG_FEPOINTLIGHT: return "";
		case ESVG_FESPECULARLIGHTING: return "";
		case ESVG_FESPOTLIGHT: return "";
		case ESVG_FETILE: return "";
		case ESVG_FETURBULENCE: return "";
		case ESVG_FILTER: return "" /* 40 */;
		case ESVG_FONT: return "";
		case ESVG_FONT_FACE: return "";
		case ESVG_FONT_FACE_FORMAT: return "";
		case ESVG_FONT_FACE_NAME: return "";
		case ESVG_FONT_FACE_SRC: return "";
		case ESVG_FONT_FACE_URI: return "";
		case ESVG_FOREIGNOBJECT: return "";
		case ESVG_G: return "g";
		case ESVG_GLYPH: return "";
		case ESVG_GLYPHREF: return "" /* 50 */;
		case ESVG_HKERN: return "";
		case ESVG_IMAGE: return "image";
		case ESVG_LINE: return "line";
		case ESVG_LINEARGRADIENT: return "linearGradient";
		case ESVG_MARKER: return "marker";
		case ESVG_MASK: return "mask";
		case ESVG_METADATA: return "";
		case ESVG_MISSING_GLYPH: return "";
		case ESVG_MPATH: return "";
		case ESVG_PATH: return "path" /* 60 */;
		case ESVG_PATTERN: return "pattern";
		case ESVG_POLYGON: return "polygon";
		case ESVG_POLYLINE: return "polyline";
		case ESVG_RADIALGRADIENT: return "radialGradient";
		case ESVG_RECT: return "rect";
		case ESVG_SCRIPT: return "";
		case ESVG_SET: return "";
		case ESVG_STOP: return "stop";
		case ESVG_STYLE: return "style";
		case ESVG_SVG: return "svg" /* 70 */;
		case ESVG_SWITCH: return "";
		case ESVG_SYMBOL: return "";
		case ESVG_TEXT: return "text";
		case ESVG_TEXTPATH: return "";
		case ESVG_TITLE: return "";
		case ESVG_TREF: return "";
		case ESVG_TSPAN: return "";
		case ESVG_USE: return "use";
		case ESVG_VIEW: return "";
		case ESVG_VKERN: return "";
		default: return "unknown";
	}
}

EAPI Eina_Bool esvg_type_is_paint_server(Esvg_Type type)
{
	switch (type)
	{
		case ESVG_LINEARGRADIENT:
		case ESVG_RADIALGRADIENT:
		case ESVG_PATTERN:
		return EINA_TRUE;

		default:
		return EINA_FALSE;
	}
}
