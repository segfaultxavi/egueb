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
#ifndef _ESVG_PARSER_H
#define _ESVG_PARSER_H

#include <Eina.h>
#include <Enesim.h>
#include <Edom.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef ESVG_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_ESVG_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

typedef enum _Esvg_Parser_Tag_Type {
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
	ESVG_SVG,
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
} Esvg_Parser_Tag_Type;

typedef Eina_Bool (*Esvg_Parser_Tag_Get)(void *data, int *tag, const char *name, size_t length);
typedef Eina_Bool (*Esvg_Parser_Tag_Open)(void *data, int tag, Edom_Context *context, const char *attributes, size_t length);
typedef void (*Esvg_Parser_Tag_Is_Supported)(void *data, int tag, Edom_Context *context);
typedef Eina_Bool (*Esvg_Parser_Tag_Attribute_Set)(void *data, Edom_Tag *tag, const char *attr, const char *value);
typedef char * (*Esvg_Parser_Tag_Attribute_Get)(void *data, Edom_Tag *tag, const char *attr);

typedef struct _Esvg_Parser_Descriptor
{
	Esvg_Parser_Tag_Get tag_get; /* whenever a tag name is found but esvg can not handle it */
	Esvg_Parser_Tag_Open tag_open; /* called whenever a tag is opened and esvg can not handle it */
	Esvg_Parser_Tag_Is_Supported tag_is_supported; /* called whenever a tag is found as a child of another but esvg can not recognize it as a valid child */
	Esvg_Parser_Tag_Attribute_Set tag_attribute_set; /* called whenever an attribute is not recognized by esvg */
	Esvg_Parser_Tag_Attribute_Get tag_attribute_get; /* called whenever an attribute is not recognized by esvg */
} Esvg_Parser_Descriptor;

EAPI Enesim_Renderer * esvg_parser_load(const char *filename, Esvg_Parser_Descriptor *descriptor, void *data);

EAPI Edom_Context * esvg_parser_context_new(Edom_Parser *parser,
		Edom_Context_Descriptor *descriptor,
		void *data);

/* some common parser descriptors */
/* the linking desriptor parses <a> tags with its childs and calls a user provided function
 * to set the destination uri whenever some element has been clicked
 */
typedef struct _Esvg_Parser_Linking Esvg_Parser_Linking;
typedef void (*Esvg_Parser_Linking_Href_Set)(void *data, Enesim_Renderer *r, const char *href);
typedef struct _Esvg_Parser_Linking_Descriptor
{
	Esvg_Parser_Linking_Href_Set href_set;
} Esvg_Parser_Linking_Descriptor;

EAPI Esvg_Parser_Linking * esvg_parser_linking_new(Esvg_Parser_Linking_Descriptor *descriptor,
		void *data);
EAPI const Esvg_Parser_Descriptor * esvg_parser_linking_descriptor_get(void);

#endif /*_ESVG_PARSER_H*/

