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
#include "egueb_svg_main_private.h"
#include "egueb_svg_main.h"
#include "egueb_svg_document.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Egueb_Dom_String *_egueb_svg_mime;
/* keep track of the initialization */
static int _egueb_svg_init_count = 0;

/*----------------------------------------------------------------------------*
 *                      Implementation interface                              *
 *----------------------------------------------------------------------------*/
static Egueb_Dom_List * _impl_mime_get(void)
{
	Egueb_Dom_List *ret;

	ret = egueb_dom_list_new(egueb_dom_value_dom_string_descriptor_get());
	egueb_dom_list_item_append(ret, egueb_dom_string_ref(_egueb_svg_mime));
	return ret;
}

static Egueb_Dom_Node * _impl_document_create(void)
{
	return egueb_svg_document_new();
}

static Egueb_Dom_Implementation_Descriptor _impl_descriptor = {
	/* .version 		= */ EGUEB_DOM_IMPLEMENTATION_DESCRIPTOR_VERSION,
	/* .mime_get		= */ _impl_mime_get,
	/* .document_create 	= */ _impl_document_create,
};

/* our helpful strings */
static void _egueb_svg_strings_init(void)
{
	/* mime */
	_egueb_svg_mime = egueb_dom_string_new_with_string("image/svg+xml");
	/* elements */
	EGUEB_SVG_NAME_A = egueb_dom_string_new_with_string("a");
	EGUEB_SVG_NAME_ANIMATE_TRANSFORM = egueb_dom_string_new_with_string("animateTransform");
	EGUEB_SVG_NAME_CIRCLE = egueb_dom_string_new_with_string("circle");
	EGUEB_SVG_NAME_CLIP_PATH = egueb_dom_string_new_with_string("clipPath");
	EGUEB_SVG_NAME_DEFS = egueb_dom_string_new_with_string("defs");
	EGUEB_SVG_NAME_DESC = egueb_dom_string_new_with_string("desc");
	EGUEB_SVG_NAME_ELLIPSE = egueb_dom_string_new_with_string("ellipse");
	EGUEB_SVG_NAME_G = egueb_dom_string_new_with_string("g");
	EGUEB_SVG_NAME_IMAGE = egueb_dom_string_new_with_string("image");
	EGUEB_SVG_NAME_LINE = egueb_dom_string_new_with_string("line");
	EGUEB_SVG_NAME_LINEAR_GRADIENT = egueb_dom_string_new_with_string("linearGradient");
	EGUEB_SVG_NAME_PATH = egueb_dom_string_new_with_string("path");
	EGUEB_SVG_NAME_PATTERN = egueb_dom_string_new_with_string("pattern");
	EGUEB_SVG_NAME_POLYGON = egueb_dom_string_new_with_string("polygon");
	EGUEB_SVG_NAME_POLYLINE = egueb_dom_string_new_with_string("polyline");
	EGUEB_SVG_NAME_RADIAL_GRADIENT = egueb_dom_string_new_with_string("radialGradient");
	EGUEB_SVG_NAME_RECT = egueb_dom_string_new_with_string("rect");
	EGUEB_SVG_NAME_TEXT = egueb_dom_string_new_with_string("text");
	EGUEB_SVG_NAME_TSPAN = egueb_dom_string_new_with_string("tspan");
	EGUEB_SVG_NAME_USE = egueb_dom_string_new_with_string("use");
	EGUEB_SVG_NAME_STOP = egueb_dom_string_new_with_string("stop");
	EGUEB_SVG_NAME_SVG = egueb_dom_string_new_with_string("svg");
	EGUEB_SVG_NAME_VIDEO = egueb_dom_string_new_with_string("video");
	/* attributes */
	EGUEB_SVG_NAME_CLASS = egueb_dom_string_new_with_string("class");
	EGUEB_SVG_NAME_CLIP_PATH = egueb_dom_string_new_with_string("clip-path");
	EGUEB_SVG_NAME_CLIP_PATH_UNITS = egueb_dom_string_new_with_string("clipPathUnits");
	EGUEB_SVG_NAME_COLOR = egueb_dom_string_new_with_string("color");
	EGUEB_SVG_NAME_CX = egueb_dom_string_new_with_string("cx");
	EGUEB_SVG_NAME_CY = egueb_dom_string_new_with_string("cy");
	EGUEB_SVG_NAME_D = egueb_dom_string_new_with_string("d");
	EGUEB_SVG_NAME_DX = egueb_dom_string_new_with_string("dx");
	EGUEB_SVG_NAME_DY = egueb_dom_string_new_with_string("dy");
	EGUEB_SVG_NAME_DISPLAY = egueb_dom_string_new_with_string("display");
	EGUEB_SVG_NAME_FILL = egueb_dom_string_new_with_string("fill");
	EGUEB_SVG_NAME_FILL_OPACITY = egueb_dom_string_new_with_string("fill-opacity");
	EGUEB_SVG_NAME_FILL_RULE = egueb_dom_string_new_with_string("fill-rule");
	EGUEB_SVG_NAME_FONT = egueb_dom_string_new_with_string("font");
	EGUEB_SVG_NAME_FONT_SIZE = egueb_dom_string_new_with_string("font-size");
	EGUEB_SVG_NAME_FONT_STYLE = egueb_dom_string_new_with_string("font-style");
	EGUEB_SVG_NAME_FONT_WEIGHT = egueb_dom_string_new_with_string("font-weight");
	EGUEB_SVG_NAME_FX = egueb_dom_string_new_with_string("fx");
	EGUEB_SVG_NAME_FY = egueb_dom_string_new_with_string("fy");
	EGUEB_SVG_NAME_HEIGHT = egueb_dom_string_new_with_string("height");
	EGUEB_SVG_NAME_GRADIENT_UNITS = egueb_dom_string_new_with_string("gradientUnits");
	EGUEB_SVG_NAME_GRADIENT_TRANSFORM = egueb_dom_string_new_with_string("gradientTransform");
	EGUEB_SVG_NAME_ID = egueb_dom_string_new_with_string("id");
	EGUEB_SVG_NAME_OFFSET = egueb_dom_string_new_with_string("offset");
	EGUEB_SVG_NAME_OPACITY = egueb_dom_string_new_with_string("opacity");
	EGUEB_SVG_NAME_OVERFLOW = egueb_dom_string_new_with_string("overflow");
	EGUEB_SVG_NAME_PATTERN_UNITS = egueb_dom_string_new_with_string("patternUnits");
	EGUEB_SVG_NAME_PATTERN_TRANSFORM = egueb_dom_string_new_with_string("patternTransform");
	EGUEB_SVG_NAME_POINTS = egueb_dom_string_new_with_string("points");
	EGUEB_SVG_NAME_R = egueb_dom_string_new_with_string("r");
	EGUEB_SVG_NAME_RX = egueb_dom_string_new_with_string("rx");
	EGUEB_SVG_NAME_RY = egueb_dom_string_new_with_string("ry");
	EGUEB_SVG_NAME_SPREAD_METHOD = egueb_dom_string_new_with_string("spreadMethod");
	EGUEB_SVG_NAME_STROKE = egueb_dom_string_new_with_string("stroke");
	EGUEB_SVG_NAME_STROKE_DASHARRAY = egueb_dom_string_new_with_string("stroke-dasharray");
	EGUEB_SVG_NAME_STROKE_LINECAP = egueb_dom_string_new_with_string("stroke-linecap");
	EGUEB_SVG_NAME_STROKE_LINEJOIN = egueb_dom_string_new_with_string("stroke-linejoin");
	EGUEB_SVG_NAME_STROKE_OPACITY = egueb_dom_string_new_with_string("stroke-opacity");
	EGUEB_SVG_NAME_STROKE_WIDTH = egueb_dom_string_new_with_string("stroke-width");
	EGUEB_SVG_NAME_STYLE = egueb_dom_string_new_with_string("style");
	EGUEB_SVG_NAME_STOP_COLOR = egueb_dom_string_new_with_string("stop-color");
	EGUEB_SVG_NAME_STOP_OPACITY = egueb_dom_string_new_with_string("stop-opacity");
	EGUEB_SVG_NAME_TEXT_ANCHOR = egueb_dom_string_new_with_string("text-anchor");
	EGUEB_SVG_NAME_TRANSFORM = egueb_dom_string_new_with_string("transform");
	EGUEB_SVG_NAME_VIEWBOX = egueb_dom_string_new_with_string("viewBox");
	EGUEB_SVG_NAME_VISIBILITY = egueb_dom_string_new_with_string("visibility");
	EGUEB_SVG_NAME_WIDTH = egueb_dom_string_new_with_string("width");
	EGUEB_SVG_NAME_X = egueb_dom_string_new_with_string("x");
	EGUEB_SVG_NAME_X1 = egueb_dom_string_new_with_string("x1");
	EGUEB_SVG_NAME_X2 = egueb_dom_string_new_with_string("x2");
	EGUEB_SVG_NAME_Y = egueb_dom_string_new_with_string("y");
	EGUEB_SVG_NAME_Y1 = egueb_dom_string_new_with_string("y1");
	EGUEB_SVG_NAME_Y2 = egueb_dom_string_new_with_string("y2");
	EGUEB_SVG_NAME_ZOOM_AND_PAN = egueb_dom_string_new_with_string("zoomAndPan");
	/* event attributes */
	EGUEB_SVG_NAME_ONCLICK = egueb_dom_string_new_with_string("onclick");
	EGUEB_SVG_NAME_ONMOUSEDOWN = egueb_dom_string_new_with_string("onmousedown");
	EGUEB_SVG_NAME_ONMOUSEUP = egueb_dom_string_new_with_string("onmouseup");
	EGUEB_SVG_NAME_ONMOUSEOVER = egueb_dom_string_new_with_string("onmouseover");
	EGUEB_SVG_NAME_ONMOUSEMOVE = egueb_dom_string_new_with_string("onmousemove");
	EGUEB_SVG_NAME_ONMOUSEOUT = egueb_dom_string_new_with_string("onmouseout");
	EGUEB_SVG_NAME_ONFOCUSOUT = egueb_dom_string_new_with_string("onfocusout");
	EGUEB_SVG_NAME_ONFOCUSIN = egueb_dom_string_new_with_string("onfocusin");
}

static void _egueb_svg_strings_shutdown(void)
{
	/* mime */
	egueb_dom_string_unref(_egueb_svg_mime);
	/* elements */
	egueb_dom_string_unref(EGUEB_SVG_NAME_A);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ANIMATE_TRANSFORM);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CIRCLE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CLIP_PATH);
	egueb_dom_string_unref(EGUEB_SVG_NAME_DEFS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_DESC);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ELLIPSE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_G);
	egueb_dom_string_unref(EGUEB_SVG_NAME_IMAGE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_LINE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_LINEAR_GRADIENT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_PATH);
	egueb_dom_string_unref(EGUEB_SVG_NAME_PATTERN);
	egueb_dom_string_unref(EGUEB_SVG_NAME_POLYGON);
	egueb_dom_string_unref(EGUEB_SVG_NAME_POLYLINE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_RADIAL_GRADIENT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_RECT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_TEXT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_TSPAN);
	egueb_dom_string_unref(EGUEB_SVG_NAME_USE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STOP);
	egueb_dom_string_unref(EGUEB_SVG_NAME_SVG);
	egueb_dom_string_unref(EGUEB_SVG_NAME_VIDEO);
	/* attributes */
	egueb_dom_string_unref(EGUEB_SVG_NAME_CLASS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CLIP_PATH);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CLIP_PATH_UNITS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_COLOR);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CX);
	egueb_dom_string_unref(EGUEB_SVG_NAME_CY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_D);
	egueb_dom_string_unref(EGUEB_SVG_NAME_DX);
	egueb_dom_string_unref(EGUEB_SVG_NAME_DY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_DISPLAY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FILL);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FILL_OPACITY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FILL_RULE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FONT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FONT_SIZE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FONT_STYLE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FONT_WEIGHT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FX);
	egueb_dom_string_unref(EGUEB_SVG_NAME_FY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_GRADIENT_UNITS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_GRADIENT_TRANSFORM);
	egueb_dom_string_unref(EGUEB_SVG_NAME_HEIGHT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ID);
	egueb_dom_string_unref(EGUEB_SVG_NAME_OFFSET);
	egueb_dom_string_unref(EGUEB_SVG_NAME_OPACITY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_OVERFLOW);
	egueb_dom_string_unref(EGUEB_SVG_NAME_PATTERN_UNITS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_PATTERN_TRANSFORM);
	egueb_dom_string_unref(EGUEB_SVG_NAME_POINTS);
	egueb_dom_string_unref(EGUEB_SVG_NAME_R);
	egueb_dom_string_unref(EGUEB_SVG_NAME_RX);
	egueb_dom_string_unref(EGUEB_SVG_NAME_RY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_SPREAD_METHOD);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STOP_COLOR);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STOP_OPACITY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE_DASHARRAY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE_LINECAP);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE_LINEJOIN);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE_OPACITY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STROKE_WIDTH);
	egueb_dom_string_unref(EGUEB_SVG_NAME_STYLE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_TEXT_ANCHOR);
	egueb_dom_string_unref(EGUEB_SVG_NAME_TRANSFORM);
	egueb_dom_string_unref(EGUEB_SVG_NAME_VIEWBOX);
	egueb_dom_string_unref(EGUEB_SVG_NAME_VISIBILITY);
	egueb_dom_string_unref(EGUEB_SVG_NAME_WIDTH);
	egueb_dom_string_unref(EGUEB_SVG_NAME_X);
	egueb_dom_string_unref(EGUEB_SVG_NAME_X1);
	egueb_dom_string_unref(EGUEB_SVG_NAME_X2);
	egueb_dom_string_unref(EGUEB_SVG_NAME_Y);
	egueb_dom_string_unref(EGUEB_SVG_NAME_Y1);
	egueb_dom_string_unref(EGUEB_SVG_NAME_Y2);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ZOOM_AND_PAN);
	/* event attributes */
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONCLICK);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONMOUSEDOWN);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONMOUSEUP);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONMOUSEOVER);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONMOUSEMOVE);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONMOUSEOUT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONFOCUSOUT);
	egueb_dom_string_unref(EGUEB_SVG_NAME_ONFOCUSIN);
}

static Eina_Bool _egueb_svg_dependencies_init(void)
{
	if (!eina_init())
	{
		fprintf(stderr, "Egueb_Svg: Eina init failed");
		return EINA_FALSE;
	}

	if (!enesim_init())
	{
		fprintf(stderr, "Egueb_Svg: Enesim init failed");
		goto shutdown_eina;
	}

	egueb_dom_init();
	egueb_css_init();
	egueb_smil_init();
	return EINA_TRUE;

shutdown_eina:
	eina_shutdown();
	return EINA_FALSE;
}

static void _egueb_svg_dependencies_shutdown(void)
{
	egueb_css_shutdown();
	egueb_smil_shutdown();
	egueb_dom_shutdown();
	enesim_shutdown();
	eina_shutdown();
}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int egueb_svg_log_dom_global = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* elements */
Egueb_Dom_String *EGUEB_SVG_NAME_A;
Egueb_Dom_String *EGUEB_SVG_NAME_ANIMATE_TRANSFORM;
Egueb_Dom_String *EGUEB_SVG_NAME_CIRCLE;
Egueb_Dom_String *EGUEB_SVG_NAME_CLIP_PATH;
Egueb_Dom_String *EGUEB_SVG_NAME_DEFS;
Egueb_Dom_String *EGUEB_SVG_NAME_DESC;
Egueb_Dom_String *EGUEB_SVG_NAME_ELLIPSE;
Egueb_Dom_String *EGUEB_SVG_NAME_G;
Egueb_Dom_String *EGUEB_SVG_NAME_IMAGE;
Egueb_Dom_String *EGUEB_SVG_NAME_LINE;
Egueb_Dom_String *EGUEB_SVG_NAME_LINEAR_GRADIENT;
Egueb_Dom_String *EGUEB_SVG_NAME_PATH;
Egueb_Dom_String *EGUEB_SVG_NAME_PATTERN;
Egueb_Dom_String *EGUEB_SVG_NAME_POLYGON;
Egueb_Dom_String *EGUEB_SVG_NAME_POLYLINE;
Egueb_Dom_String *EGUEB_SVG_NAME_RADIAL_GRADIENT;
Egueb_Dom_String *EGUEB_SVG_NAME_RECT;
Egueb_Dom_String *EGUEB_SVG_NAME_TEXT;
Egueb_Dom_String *EGUEB_SVG_NAME_TSPAN;
Egueb_Dom_String *EGUEB_SVG_NAME_USE;
Egueb_Dom_String *EGUEB_SVG_NAME_STOP;
Egueb_Dom_String *EGUEB_SVG_NAME_SVG;
Egueb_Dom_String *EGUEB_SVG_NAME_VIDEO;
/* attributes */
Egueb_Dom_String *EGUEB_SVG_NAME_CLASS;
Egueb_Dom_String *EGUEB_SVG_NAME_CLIP_PATH;
Egueb_Dom_String *EGUEB_SVG_NAME_CLIP_PATH_UNITS;
Egueb_Dom_String *EGUEB_SVG_NAME_COLOR;
Egueb_Dom_String *EGUEB_SVG_NAME_CX;
Egueb_Dom_String *EGUEB_SVG_NAME_CY;
Egueb_Dom_String *EGUEB_SVG_NAME_D;
Egueb_Dom_String *EGUEB_SVG_NAME_DX;
Egueb_Dom_String *EGUEB_SVG_NAME_DY;
Egueb_Dom_String *EGUEB_SVG_NAME_DISPLAY;
Egueb_Dom_String *EGUEB_SVG_NAME_FILL;
Egueb_Dom_String *EGUEB_SVG_NAME_FILL_OPACITY;
Egueb_Dom_String *EGUEB_SVG_NAME_FILL_RULE;
Egueb_Dom_String *EGUEB_SVG_NAME_FONT;
Egueb_Dom_String *EGUEB_SVG_NAME_FONT_SIZE;
Egueb_Dom_String *EGUEB_SVG_NAME_FONT_STYLE;
Egueb_Dom_String *EGUEB_SVG_NAME_FONT_WEIGHT;
Egueb_Dom_String *EGUEB_SVG_NAME_FX;
Egueb_Dom_String *EGUEB_SVG_NAME_FY;
Egueb_Dom_String *EGUEB_SVG_NAME_GRADIENT_UNITS;
Egueb_Dom_String *EGUEB_SVG_NAME_GRADIENT_TRANSFORM;
Egueb_Dom_String *EGUEB_SVG_NAME_HEIGHT;
Egueb_Dom_String *EGUEB_SVG_NAME_ID;
Egueb_Dom_String *EGUEB_SVG_NAME_OFFSET;
Egueb_Dom_String *EGUEB_SVG_NAME_OPACITY;
Egueb_Dom_String *EGUEB_SVG_NAME_OVERFLOW;
Egueb_Dom_String *EGUEB_SVG_NAME_PATTERN_UNITS;
Egueb_Dom_String *EGUEB_SVG_NAME_PATTERN_TRANSFORM;
Egueb_Dom_String *EGUEB_SVG_NAME_POINTS;
Egueb_Dom_String *EGUEB_SVG_NAME_R;
Egueb_Dom_String *EGUEB_SVG_NAME_RX;
Egueb_Dom_String *EGUEB_SVG_NAME_RY;
Egueb_Dom_String *EGUEB_SVG_NAME_STOP_COLOR;
Egueb_Dom_String *EGUEB_SVG_NAME_STOP_OPACITY;
Egueb_Dom_String *EGUEB_SVG_NAME_SPREAD_METHOD;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE_DASHARRAY;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE_LINECAP;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE_LINEJOIN;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE_OPACITY;
Egueb_Dom_String *EGUEB_SVG_NAME_STROKE_WIDTH;
Egueb_Dom_String *EGUEB_SVG_NAME_STYLE;
Egueb_Dom_String *EGUEB_SVG_NAME_TEXT_ANCHOR;
Egueb_Dom_String *EGUEB_SVG_NAME_TRANSFORM;
Egueb_Dom_String *EGUEB_SVG_NAME_VIEWBOX;
Egueb_Dom_String *EGUEB_SVG_NAME_VISIBILITY;
Egueb_Dom_String *EGUEB_SVG_NAME_WIDTH;
Egueb_Dom_String *EGUEB_SVG_NAME_X;
Egueb_Dom_String *EGUEB_SVG_NAME_X1;
Egueb_Dom_String *EGUEB_SVG_NAME_X2;
Egueb_Dom_String *EGUEB_SVG_NAME_Y;
Egueb_Dom_String *EGUEB_SVG_NAME_Y1;
Egueb_Dom_String *EGUEB_SVG_NAME_Y2;
Egueb_Dom_String *EGUEB_SVG_NAME_ZOOM_AND_PAN;

Egueb_Dom_String *EGUEB_SVG_NAME_ONCLICK;
Egueb_Dom_String *EGUEB_SVG_NAME_ONMOUSEDOWN;
Egueb_Dom_String *EGUEB_SVG_NAME_ONMOUSEUP;
Egueb_Dom_String *EGUEB_SVG_NAME_ONMOUSEOVER;
Egueb_Dom_String *EGUEB_SVG_NAME_ONMOUSEMOVE;
Egueb_Dom_String *EGUEB_SVG_NAME_ONMOUSEOUT;
Egueb_Dom_String *EGUEB_SVG_NAME_ONFOCUSOUT;
Egueb_Dom_String *EGUEB_SVG_NAME_ONFOCUSIN;

/**
 * Initialize the egueb_svg library
 */
EAPI int egueb_svg_init(void)
{
	Egueb_Dom_Implementation *i;

	if (++_egueb_svg_init_count != 1)
		return _egueb_svg_init_count;

	if (!_egueb_svg_dependencies_init())
		return --_egueb_svg_init_count;

	egueb_svg_log_dom_global = eina_log_domain_register("egueb_svg", 0);
	if (egueb_svg_log_dom_global < 0)
	{
		EINA_LOG_ERR("Egueb_Svg: Can not create a general log domain.");
		_egueb_svg_dependencies_shutdown();
		return --_egueb_svg_init_count;
	}
	_egueb_svg_strings_init();
	/* register our own source */
	i = egueb_dom_implementation_new(&_impl_descriptor);
	egueb_dom_registry_implementation_add(i);

	return _egueb_svg_init_count;
}

/**
 * Shutdown the egueb_svg library
 */
EAPI int egueb_svg_shutdown(void)
{
	if (--_egueb_svg_init_count != 0)
		return _egueb_svg_init_count;

	_egueb_svg_strings_shutdown();
	eina_log_domain_unregister(egueb_svg_log_dom_global);
        egueb_svg_log_dom_global = -1;
	_egueb_svg_dependencies_shutdown();

	return _egueb_svg_init_count;
}
