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
#ifndef _ESVG_TYPES_H
#define _ESVG_TYPES_H

typedef enum _Esvg_Type {
	ESVG_A,
	ESVG_ALTGLYPH,
	ESVG_ALTGLYPHDEF,
	ESVG_ALTGLYPHITEM,
	ESVG_ANIMATE,
	ESVG_ANIMATECOLOR,
	ESVG_ANIMATEMOTION,
	ESVG_ANIMATETRANSFORM,
	ESVG_CIRCLE,
	ESVG_CLIPPATH,
	ESVG_COLOR_PROFILE, /* 10 */
	ESVG_CURSOR,
	ESVG_DEFINITION_SRC,
	ESVG_DEFS,
	ESVG_DESC,
	ESVG_ELLIPSE,
	ESVG_FEBLEND,
	ESVG_FECOLORMATRIX,
	ESVG_FECOMPONENTTRANSFER,
	ESVG_FECOMPOSITE,
	ESVG_FECONVOLVEMATRIX, /* 20 */
	ESVG_FEDIFFUSELIGHTING,
	ESVG_FEDISPLACEMENTMAP,
	ESVG_FEDISTANTLIGHT,
	ESVG_FEFLOOD,
	ESVG_FEFUNCA,
	ESVG_FEFUNCB,
	ESVG_FEFUNCG,
	ESVG_FEFUNCR,
	ESVG_FEGAUSSIANBLUR,
	ESVG_FEIMAGE, /* 30 */
	ESVG_FEMERGE,
	ESVG_FEMERGENODE,
	ESVG_FEMORPHOLOGY,
	ESVG_FEOFFSET,
	ESVG_FEPOINTLIGHT,
	ESVG_FESPECULARLIGHTING,
	ESVG_FESPOTLIGHT,
	ESVG_FETILE,
	ESVG_FETURBULENCE,
	ESVG_FILTER, /* 40 */
	ESVG_FONT,
	ESVG_FONT_FACE,
	ESVG_FONT_FACE_FORMAT,
	ESVG_FONT_FACE_NAME,
	ESVG_FONT_FACE_SRC,
	ESVG_FONT_FACE_URI,
	ESVG_FOREIGNOBJECT,
	ESVG_G,
	ESVG_GLYPH,
	ESVG_GLYPHREF, /* 50 */
	ESVG_HKERN,
	ESVG_IMAGE,
	ESVG_LINE,
	ESVG_LINEARGRADIENT,
	ESVG_MARKER,
	ESVG_MASK,
	ESVG_METADATA,
	ESVG_MISSING_GLYPH,
	ESVG_MPATH,
	ESVG_PATH, /* 60 */
	ESVG_PATTERN,
	ESVG_POLYGON,
	ESVG_POLYLINE,
	ESVG_RADIALGRADIENT,
	ESVG_RECT,
	ESVG_SCRIPT,
	ESVG_SET,
	ESVG_STOP,
	ESVG_STYLE,
	ESVG_SVG, /* 70 */
	ESVG_SWITCH,
	ESVG_SYMBOL,
	ESVG_TEXT,
	ESVG_TEXTPATH,
	ESVG_TITLE,
	ESVG_TREF,
	ESVG_TSPAN,
	ESVG_USE,
	ESVG_VIEW,
	ESVG_VKERN,
	ESVG_PARSE_TAGS
} Esvg_Type;


/* FIXME implement the unit type as the spec or just
 * ABSOLUTE/RELATIVE?
 */
typedef enum _Esvg_Unit_Length
{
	ESVG_UNIT_LENGTH_EM,
	ESVG_UNIT_LENGTH_EX,
	ESVG_UNIT_LENGTH_PX,
	ESVG_UNIT_LENGTH_PT,
	ESVG_UNIT_LENGTH_PC,
	ESVG_UNIT_LENGTH_CM,
	ESVG_UNIT_LENGTH_MM,
	ESVG_UNIT_LENGTH_IN,
	ESVG_UNIT_LENGTH_PERCENT,
	ESVG_UNIT_LENGTHS
} Esvg_Unit_Length;

typedef struct _Esvg_Length
{
	double value;
	Esvg_Unit_Length unit;
} Esvg_Length;

typedef Esvg_Length Esvg_Coord;

/* We need to use uint32_t here because we can not pass
 * data < 32bits through varargs
 */
typedef struct _Esvg_Color
{
	uint32_t r;
	uint32_t g;
	uint32_t b;
} Esvg_Color;

/* FIXME when doing clipping or masking, we also use this
 * so better change the name
 */
typedef enum _Esvg_Gradient_Units
{
	ESVG_USER_SPACE_ON_USE,
	ESVG_OBJECT_BOUNDING_BOX,
	ESVG_GRADIENT_UNITS,
} Esvg_Gradient_Units, Esvg_Pattern_Units;

typedef Esvg_Gradient_Units Esvg_Clip_Path_Units;

typedef enum _Esvg_Paint_Server_Type
{
	ESVG_PAINT_SERVER_LINEAR_GRADIENT,
	ESVG_PAINT_SERVER_RADIAL_GRADIENT,
	ESVG_PAINT_SERVER_PATTERN,
	ESVG_PAINT_SERVER_TYPES
} Esvg_Paint_Server_Type;

typedef enum _Esvg_Spread_Method
{
	ESVG_SPREAD_METHOD_PAD,
	ESVG_SPREAD_METHOD_REFLECT,
	ESVG_SPREAD_METHOD_REPEAT,
} Esvg_Spread_Method;

typedef enum _Esvg_Paint_Type
{
	ESVG_PAINT_NONE,
	ESVG_PAINT_CURRENT_COLOR,
	ESVG_PAINT_COLOR,
	ESVG_PAINT_SERVER,
	ESVG_PAINT_TYPES,
} Esvg_Paint_Type;

typedef struct _Esvg_Paint
{
	Esvg_Paint_Type type;
	union {
		Esvg_Color color;
		char *paint_server;
	} value;
} Esvg_Paint;

typedef enum _Esvg_Path_Command_Type
{
	ESVG_PATH_MOVE_TO,
	ESVG_PATH_LINE_TO,
	ESVG_PATH_HLINE_TO,
	ESVG_PATH_VLINE_TO,
	ESVG_PATH_CUBIC_TO,
	ESVG_PATH_SCUBIC_TO,
	ESVG_PATH_QUADRATIC_TO,
	ESVG_PATH_SQUADRATIC_TO,
	ESVG_PATH_ARC_TO,
	ESVG_PATH_CLOSE,
	ESVG_PATH_COMMAND_TYPES
} Esvg_Path_Command_Type;

typedef struct _Esvg_Path_Command
{
	Esvg_Path_Command_Type type;
	union {
		struct {
			double c;
		} hline_to, vline_to;
		struct {
			double x;
			double y;
		} move_to, line_to, squadratic_to;
		struct {
			double ctrl_x0;
			double ctrl_y0;
			double ctrl_x1;
			double ctrl_y1;
			double x;
			double y;
		} cubic_to;
		struct {
			double ctrl_x;
			double ctrl_y;
			double x;
			double y;
		} scubic_to, quadratic_to;
		struct {
			double rx;
			double ry;
			double angle;
			double large;
			double sweep;
			double x;
			double y;
		} arc_to;
 	} data;
	Eina_Bool relative;
	Eina_Bool is_closed;
} Esvg_Path_Command;

typedef struct _Esvg_Point
{
	double x;
	double y;
} Esvg_Point;

typedef struct _Esvg_View_Box
{
	double min_x;
	double min_y;
	double width;
	double height;
} Esvg_View_Box;

typedef enum _Esvg_Stroke_Line_Cap
{
	ESVG_LINE_CAP_BUTT,
	ESVG_LINE_CAP_ROUND,
	ESVG_LINE_CAP_SQUARE,
} Esvg_Stroke_Line_Cap;

typedef enum _Esvg_Stroke_Line_Join
{
	ESVG_LINE_JOIN_MITER,
	ESVG_LINE_JOIN_ROUND,
	ESVG_LINE_JOIN_BEVEL,
} Esvg_Stroke_Line_Join;

/* attributes */
typedef struct _Esvg_Attribute_Regular {
} Esvg_Attribute_Regular;

typedef enum _Esvg_Fill_Rule
{
	ESVG_NON_ZERO,
	ESVG_EVEN_ODD
} Esvg_Fill_Rule;

typedef void * (*Esvg_Uri_Local_Get)(const char *name, void *data);
typedef void * (*Esvg_Uri_Absolute_Get)(const char *name, void *data);

typedef struct _Esvg_Uri_Descriptor
{
	Esvg_Uri_Local_Get local_get;
	Esvg_Uri_Absolute_Get absolute_get;
} Esvg_Uri_Descriptor;

EAPI Eina_Bool esvg_string_is_uri(const char *attr);
EAPI void * esvg_uri_string_from(const char *attr, Esvg_Uri_Descriptor *descriptor, void *data);

extern Esvg_Length ESVG_LENGTH_0;
extern Esvg_Length ESVG_LENGTH_1;
extern Esvg_Length ESVG_LENGTH_100_PERCENT;
#define ESVG_COORD_0 ESVG_LENGTH_0

static inline Eina_Bool esvg_length_is_relative(Esvg_Length *length)
{
	if (length->unit == ESVG_UNIT_LENGTH_PERCENT)
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

EAPI const char * esvg_type_string_to(Esvg_Type type);

EAPI double esvg_number_string_from(const char *attr_val, double default_nbr);

EAPI Eina_Bool esvg_length_string_from(Esvg_Length *length, const char *attr_val, Esvg_Length default_length);
EAPI Eina_Bool esvg_length_is_equal(Esvg_Length *length1, Esvg_Length *length2);
EAPI double esvg_length_final_get(const Esvg_Length *l, double parent_length);
EAPI Eina_Bool esvg_paint_string_from(Esvg_Paint *paint, const char *attr);
Esvg_View_Box esvg_view_box_get(const char *attr_val);

#endif
