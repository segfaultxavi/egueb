/* Esvg - SVG
 * Copyright (C) 2012 Jorge Luis Zapata
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
/* FIXME the parsing should be the SVGColor not the dom RGBColor ... */
#include "esvg_private_main.h"

#include "esvg_types.h"
#include "esvg_private_types.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define ESVG_LOG_DEFAULT _esvg_color_log

static int _esvg_color_log = -1;

static Ender_Property *ESVG_COLOR_R;
static Ender_Property *ESVG_COLOR_G;
static Ender_Property *ESVG_COLOR_B;

static Ender_Property *ESVG_COLOR_ANIMATED_BASE;
static Ender_Property *ESVG_COLOR_ANIMATED_ANIM;

#define _esvg_color_new NULL
#define _esvg_color_delete NULL
#define _esvg_color_r_set NULL
#define _esvg_color_r_get NULL
#define _esvg_color_r_is_set NULL
#define _esvg_color_g_set NULL
#define _esvg_color_g_get NULL
#define _esvg_color_g_is_set NULL
#define _esvg_color_b_set NULL
#define _esvg_color_b_get NULL
#define _esvg_color_b_is_set NULL
#include "generated/esvg_generated_color.c"

#define _esvg_color_animated_new NULL
#define _esvg_color_animated_delete NULL
#define _esvg_color_animated_base_set NULL
#define _esvg_color_animated_base_get NULL
#define _esvg_color_animated_base_is_set NULL
#define _esvg_color_animated_anim_set NULL
#define _esvg_color_animated_anim_get NULL
#define _esvg_color_animated_anim_is_set NULL
#include "generated/esvg_generated_color_animated.c"

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


static Eina_Bool _esvg_color_keyword_from(Esvg_Color *color, const char *attr_val)
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

	if (!argb)
	{
		return EINA_FALSE;
	}

	color->r = (*argb >> 16) & 0xff;
	color->g = (*argb >> 8) & 0xff;
	color->b = (*argb & 0xff);

	return EINA_TRUE;
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void esvg_color_init(void)
{
	_esvg_color_log = eina_log_domain_register("esvg_color", ESVG_LOG_COLOR_DEFAULT);
	if (_esvg_color_log < 0)
	{
		EINA_LOG_ERR("Can not create log domain.");
		return;
	}
	_esvg_color_init();
	_esvg_color_animated_init();
}

void esvg_color_shutdown(void)
{
	_esvg_color_shutdown();
	_esvg_color_animated_shutdown();
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Eina_Bool esvg_color_is_equal(const Esvg_Color *c1, const Esvg_Color *c2)
{
	/* sanity checks */
	if (c1 == c2) return EINA_TRUE;
	if (!c1) return EINA_FALSE;
	if (!c2) return EINA_FALSE;

	if ((c1->r == c2->r) && (c1->g == c2->g) && (c1->b == c2->b))
		return EINA_TRUE;
	return EINA_FALSE;
}

/*
 * #rgb
 * #rrggbb
 * rgb(c, c, c)
 */
EAPI Eina_Bool esvg_color_string_from(Esvg_Color *color, const char *attr_val)
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
	else if (sz >= 10 && (attr_val[0] == 'r') &&
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
			if (esvg_long_get(iter, &tmp, &val))
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
		return EINA_FALSE;
	}
	/* is a keyword */
	else
	{
		return _esvg_color_keyword_from(color, attr_val);
	}
}