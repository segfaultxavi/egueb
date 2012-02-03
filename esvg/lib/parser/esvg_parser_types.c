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
#include "Esvg.h"
#include "esvg_private.h"
#include "esvg_values.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_IS_HEXA(c) \
	(((c >= 48) && (c <= 57)) || \
	((c >= 65) && (c <= 70)) || \
	((c >= 97) && (c <= 102)))

#define ESVG_SPACE_SKIP(t) \
	while (*t) { if ((*t == ' ') || (*t == '\n') || (*t == '\t')) t++; else break; }

#define ESVG_SPACE_COMMA_SKIP(t) \
	ESVG_SPACE_SKIP(t); \
	if (*t == ',') t++; \
	ESVG_SPACE_SKIP(t);

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

static Eina_Bool _esvg_transformation_translate_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double tx[2];
	int nvalues = 0;
	const char *tmp = attr_val;
	char *end;
	const size_t sz = 9;

	if (strncmp(tmp, "translate", sz) != 0)
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
		val = strtod(tmp, &end);
		if (errno == ERANGE)
			val = 0;
		if (end == tmp)
			break;
		tmp = end;
		tx[nvalues] = val;
		nvalues++;
		/* we store only the 2 first numbers */
		if (nvalues >= 2)
			break;
		/* skip the comma and the blanks */
		ESVG_SPACE_COMMA_SKIP(tmp);
	}

	if (tmp[0] != ')')
		return EINA_FALSE;
	tmp++;
	/* on translation the tx is mandatory but not ty */
	if (nvalues == 1)
		tx[1] = 0;
	enesim_matrix_translate(matrix, tx[0], tx[1]);

	*endptr = tmp;

	return EINA_TRUE;
}

/* FIXME same code as the translate, merge it */
static Eina_Bool _esvg_transformation_scale_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double tx[2];
	int nvalues = 0;
	const char *tmp = attr_val;
	char *end;
	const size_t sz = 5;

	if (strncmp(tmp, "scale", sz) != 0)
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
		val = strtod(tmp, &end);
		if (errno == ERANGE)
			val = 0;
		if (end == tmp)
			break;
		tmp = end;
		tx[nvalues] = val;
		nvalues++;
		/* we store only the 2 first numbers */
		if (nvalues >= 2)
			break;
		/* skip the comma and the blanks */
		ESVG_SPACE_COMMA_SKIP(tmp);
	}

	if (tmp[0] != ')')
		return EINA_FALSE;
	tmp++;
	if (nvalues == 1)
		tx[1] = tx[0];
	enesim_matrix_scale(matrix, tx[0], tx[1]);

	*endptr = tmp;

	return EINA_TRUE;
}


static Eina_Bool _esvg_transformation_rotate_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double rx[3];
	int nvalues = 0;
	const char *tmp = attr_val;
	char *end;
	const size_t sz = 6;

	ESVG_SPACE_SKIP(tmp);
	if (strncmp(tmp, "rotate", sz) != 0)
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
		val = strtod(tmp, &end);
		if (errno == ERANGE)
			val = 0;
		if (end == tmp)
			break;
		tmp = end;
		rx[nvalues] = val;
		nvalues++;
		/* we store only the 4 first numbers */
		if (nvalues >= 3)
			break;
		/* skip the comma and the blanks */
		ESVG_SPACE_COMMA_SKIP(tmp);
	}

	if (tmp[0] != ')')
		return EINA_FALSE;
	tmp++;
	/* FIXME handle the origin */
	enesim_matrix_rotate(matrix, rx[0] * M_PI / 180.0);

	*endptr = tmp;

	return EINA_TRUE;
}

static Eina_Bool _esvg_transformation_matrix_get(Enesim_Matrix *matrix, const char *attr_val, const char **endptr)
{
	double *values = (double *)matrix;
	int nvalues = 0;
	const char *tmp = attr_val;
	char *end;
	const size_t sz = 6;

	ESVG_SPACE_SKIP(tmp);
	if (strncmp(tmp, "matrix", sz) != 0)
		return EINA_FALSE;
	tmp += sz;
	ESVG_SPACE_SKIP(tmp);
	if (tmp[0] != '(')
		return EINA_FALSE;
	tmp++;
	/* six numbers */
	while (*tmp)
	{
		double val;

		ESVG_SPACE_SKIP(tmp);
		val = strtod(tmp, &end);
		if (errno == ERANGE)
			val = 0;
		if (end == tmp)
			break;
		tmp = end;
		*values = val;
		values++;
		nvalues++;
		/* we store only the 6 first numbers */
		if (nvalues >= 6)
			break;
		/* skip the comma and the blanks */
		ESVG_SPACE_COMMA_SKIP(tmp);
	}

	if (tmp[0] != ')')
		return EINA_FALSE;
	tmp++;

	*endptr = tmp;

	return EINA_TRUE;
}

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

static Eina_Bool _esvg_path_cubic_curve_get(char **attr, Esvg_Point *p1, Esvg_Point *p2, Esvg_Point *p)
{
	if (!_esvg_path_point_get(attr, p1))
		return EINA_FALSE;

	if (!_esvg_path_point_get(attr, p2))
		return EINA_FALSE;

	if (!_esvg_path_point_get(attr, p))
		return EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool _esvg_path_scubic_curve_get(char **attr, Esvg_Point *p1, Esvg_Point *p)
{
	if (!_esvg_path_point_get(attr, p1))
		return EINA_FALSE;

	if (!_esvg_path_point_get(attr, p))
		return EINA_FALSE;

	return EINA_TRUE;
}

static Eina_Bool _esvg_path_arc_get(char **attr, Esvg_Point *radii, double *angle, double *large, double *sweep, Esvg_Point *p)
{
	if (!_esvg_path_point_get(attr, radii))
		return EINA_FALSE;

	if (!_esvg_path_number_get(attr, angle))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	if (!_esvg_path_number_get(attr, large))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	if (!_esvg_path_number_get(attr, sweep))
	{
		ERR("can not convert number");
		return EINA_FALSE;
	}

	if (!_esvg_path_point_get(attr, p))
		return EINA_FALSE;

	return EINA_TRUE;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
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

double esvg_number_get(const char *attr_val, double default_nbr)
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

Eina_Bool esvg_length_get(Esvg_Length *length, const char *attr_val, Esvg_Length default_length)
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
	/* get the tag from the specified uri */
	ret_tag = esvg_parser_svg_tag_find(topmost, id);
	if (!ret_tag)
	{
		printf("tag not found!!!!!!\n");
		return EINA_FALSE;
	}
	*tag = ret_tag;

	return EINA_TRUE;
}

Eina_Bool esvg_parser_is_uri(const char *attr)
{
	if (strncmp(attr, "url(", 4))
		return EINA_FALSE;
	return EINA_TRUE;
}

/*
 * [ <absoluteURI> | <relativeURI> ] [ "#" <elementID> ]
 */
Eina_Bool esvg_uri_get(Edom_Tag **tag, Edom_Tag *rel, const char *attr)
{
	char url[PATH_MAX];
	size_t len;

	if (!esvg_parser_is_uri(attr))
		return EINA_FALSE;

	len = strlen(attr) - 5;
	strncpy(url, attr + 4, len);
	url[len] = '\0';

	return esvg_href_get(tag, rel, url);
}

/*
 * none, currentColor, <color>, <uri>?
 */
Eina_Bool esvg_paint_get(Esvg_Paint *paint, Edom_Tag *tag, const char *attr)
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
	else if (esvg_uri_get(&found_tag, tag, attr))
	{
		Enesim_Renderer *r;

		r = esvg_parser_element_renderer_get(found_tag);
		paint->type = ESVG_PAINT_SERVER;
		/* FIXME double check the tag */
		/* FIXME in case the paint server has some pointer we should
		 * be able to re create it from a new one? */
		paint->value.paint_server = r;
	}
	else
	{
		return EINA_FALSE;
	}

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

Eina_Bool esvg_transformation_get(Enesim_Matrix *matrix, const char *attr)
{
	Eina_Bool ret;
	const char *endptr = NULL;

	enesim_matrix_identity(matrix);
	do
	{
		Enesim_Matrix parsed;
		enesim_matrix_identity(&parsed);
		ret = _esvg_transformation_matrix_get(&parsed, attr, &endptr);
		if (!ret)
		{
			ret = _esvg_transformation_translate_get(&parsed, attr, &endptr);
			if (!ret)
			{
				ret = _esvg_transformation_rotate_get(&parsed, attr, &endptr);
				printf("rotate = %d\n", ret);
				if (!ret)
				{
					ret = _esvg_transformation_scale_get(&parsed, attr, &endptr);
					printf("scale = %d\n", ret);
				}
			}
			else
			{
				printf("translate = %d\n", ret);
			}
		}
		if (ret)
		{
			enesim_matrix_compose(matrix, &parsed, matrix);
		}
		attr = endptr;
	}
	while (endptr && *endptr && ret);
	return ret;
}

/*
 * FIXME
 * done: M, m, L, l, H, h, V, v, C, c, S, s, Q, q, T, t, A, a
 */
Eina_Bool esvg_parser_path(const char *value, Enesim_Renderer *r)
{
	char *iter;
	Esvg_Path_Command cmd;
	Esvg_Path_Command_Type type;
	Eina_Bool is_relative;
	Eina_Bool is_line_h;
	Eina_Bool is_line_v;
	Esvg_Point p_cur;

	iter = (char *)value;
	ESVG_SPACE_SKIP(iter);

	/* First char must be 'M' or 'm' */
	if ((*iter != 'M') &&
	    (*iter != 'm'))
	{
		ERR("First char not 'M' or 'm'");
		return EINA_FALSE;
	}

	/* 1st point is mandatory */
	iter++;
	ESVG_SPACE_SKIP(iter);

	if (!_esvg_path_point_get(&iter, &p_cur))
	{
		ERR("Can not get first point");
		return EINA_FALSE;
	}
	else
	{
		cmd.type = ESVG_PATH_MOVE_TO;
		cmd.relative = EINA_FALSE;
		cmd.data.move_to.x = p_cur.x;
		cmd.data.move_to.y = p_cur.y;
		esvg_path_command_add(r, &cmd);
	}

	is_line_h = EINA_FALSE;
	is_line_v = EINA_FALSE;

	while (*iter)
	{
		Esvg_Point p;

		ESVG_SPACE_SKIP(iter);

		if (*iter == 'M')
		{
			type = ESVG_PATH_MOVE_TO;
			is_relative = EINA_FALSE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'm')
		{
			type = ESVG_PATH_MOVE_TO;
			is_relative = EINA_TRUE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'L')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_FALSE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'l')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_TRUE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'H')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_FALSE;
			is_line_h = EINA_TRUE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'h')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_TRUE;
			is_line_h = EINA_TRUE;
			is_line_v = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'V')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_FALSE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'v')
		{
			type = ESVG_PATH_LINE_TO;
			is_relative = EINA_TRUE;
			is_line_h = EINA_FALSE;
			is_line_v = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'C')
		{
			type = ESVG_PATH_CUBIC_TO;
			is_relative = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'c')
		{
			type = ESVG_PATH_CUBIC_TO;
			is_relative = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'S')
		{
			type = ESVG_PATH_SCUBIC_TO;
			is_relative = EINA_FALSE;
			iter++;
		}
		else if (*iter == 's')
		{
			type = ESVG_PATH_SCUBIC_TO;
			is_relative = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'Q')
		{
			type = ESVG_PATH_QUADRATIC_TO;
			is_relative = EINA_FALSE;
			iter++;
		}
		else if (*iter == 'q')
		{
			type = ESVG_PATH_QUADRATIC_TO;
			is_relative = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'T')
		{
			type = ESVG_PATH_SQUADRATIC_TO;
			is_relative = EINA_FALSE;
			iter++;
		}
		else if (*iter == 't')
		{
			type = ESVG_PATH_SQUADRATIC_TO;
			is_relative = EINA_TRUE;
			iter++;
		}
		else if (*iter == 'A')
		{
			type = ESVG_PATH_ARC_TO;
			is_relative = EINA_FALSE;
			iter++;

		}
		else if (*iter == 'a')
		{
			type = ESVG_PATH_ARC_TO;
			is_relative = EINA_TRUE;
			iter++;

		}
		else if (*iter == 'z')
		{
			cmd.type = ESVG_PATH_CLOSE;
			cmd.is_closed = EINA_TRUE;
			esvg_path_command_add(r, &cmd);
			iter++;
			continue;
		}
		/* in case there are spaces at the end */
		else if (*iter == '\0')
		{
			break;
		}
		else
		{
			/* FIXME we can reach this point in case there is a simplified version
			 * of the path where there's no command and we should use the previous one
			 */
			ERR("unsupported path data instruction (%c) %s", *iter, iter);
			//return EINA_FALSE;
		}

		if ((type == ESVG_PATH_MOVE_TO) ||
		    (type == ESVG_PATH_LINE_TO))
		{
			if (is_line_h || is_line_v)
			{
				double v;

				if (!_esvg_path_number_get(&iter, &v))
				{
					ERR("Can not get point");
					break;
				}
				if (is_line_h)
				{
					p.x = v;
					p.y = 0;
				}
				else /* is_line_v */
				{
					p.x = 0;
					p.y = v;
				}
			}
			else
			{
				if (!_esvg_path_point_get(&iter, &p))
				{
					ERR("Can not get point");
					break;
				}
			}

			if (is_relative)
			{
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}

			if (type == ESVG_PATH_MOVE_TO)
			{
				cmd.data.move_to.x = p_cur.x;
				cmd.data.move_to.y = p_cur.y;
			}
			else /* ESVG_PATH_LINE_TO */
			{
				cmd.data.line_to.x = p_cur.x;
				cmd.data.line_to.y = p_cur.y;
			}

			cmd.type = type;
			esvg_path_command_add(r, &cmd);
		}

		if (type == ESVG_PATH_CUBIC_TO)
		{
			Esvg_Point p1;
			Esvg_Point p2;

			if (!_esvg_path_cubic_curve_get(&iter, &p1, &p2, &p))
				return EINA_FALSE;

			if (is_relative)
			{
				p1.x += p_cur.x;
				p1.y += p_cur.y;
				p2.x += p_cur.x;
				p2.y += p_cur.y;
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}
			cmd.type = type;
			cmd.data.cubic_to.ctrl_x0 = p1.x;
			cmd.data.cubic_to.ctrl_y0 = p1.y;
			cmd.data.cubic_to.ctrl_x1 = p2.x;
			cmd.data.cubic_to.ctrl_y1 = p2.y;
			cmd.data.cubic_to.x = p_cur.x;
			cmd.data.cubic_to.y = p_cur.y;
			esvg_path_command_add(r, &cmd);
		}

		if (type == ESVG_PATH_SCUBIC_TO)
		{
			Esvg_Point p1;

			if (!_esvg_path_scubic_curve_get(&iter, &p1, &p))
				return EINA_FALSE;

			if (is_relative)
			{
				p1.x += p_cur.x;
				p1.y += p_cur.y;
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}
			cmd.type = type;
			cmd.data.scubic_to.ctrl_x = p1.x;
			cmd.data.scubic_to.ctrl_y = p1.y;
			cmd.data.scubic_to.x = p_cur.x;
			cmd.data.scubic_to.y = p_cur.y;
			esvg_path_command_add(r, &cmd);
		}

		if (type == ESVG_PATH_QUADRATIC_TO)
		{
			Esvg_Point p1;

			if (!_esvg_path_scubic_curve_get(&iter, &p1, &p))
				return EINA_FALSE;

			if (is_relative)
			{
				p1.x += p_cur.x;
				p1.y += p_cur.y;
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}
			cmd.type = type;
			cmd.data.quadratic_to.ctrl_x = p1.x;
			cmd.data.quadratic_to.ctrl_y = p1.y;
			cmd.data.quadratic_to.x = p_cur.x;
			cmd.data.quadratic_to.y = p_cur.y;
			esvg_path_command_add(r, &cmd);
		}

		if (type == ESVG_PATH_SQUADRATIC_TO)
		{
			if (!_esvg_path_point_get(&iter, &p))
				return EINA_FALSE;

			if (is_relative)
			{
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}
			cmd.type = type;
			cmd.data.squadratic_to.x = p_cur.x;
			cmd.data.squadratic_to.y = p_cur.y;
			esvg_path_command_add(r, &cmd);
		}

		if (type == ESVG_PATH_ARC_TO)
		{
			Esvg_Point radii;
			double angle;
			double large;
			double sweep;

			if (!_esvg_path_arc_get(&iter, &radii, &angle, &large, &sweep, &p))
				return EINA_FALSE;

			if (is_relative)
			{
				radii.x += p_cur.x;
				radii.y += p_cur.y;
				p_cur.x += p.x;
				p_cur.y += p.y;
				cmd.relative = EINA_TRUE;
			}
			else
			{
				p_cur = p;
				cmd.relative = EINA_FALSE;
			}
			cmd.type = type;
			cmd.data.arc_to.rx = radii.x;
			cmd.data.arc_to.ry = radii.y;
			cmd.data.arc_to.angle = angle;
			cmd.data.arc_to.large = large;
			cmd.data.arc_to.sweep = sweep;
			cmd.data.arc_to.x = p_cur.x;
			cmd.data.arc_to.y = p_cur.y;
			esvg_path_command_add(r, &cmd);
		}
	}

	return EINA_TRUE;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
