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
#ifndef _ESVG_MAIN_PRIVATE_H
#define _ESVG_MAIN_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include <Eina.h>
#include <Ecss.h>
#include <Enesim.h>
#include <Egueb_Dom.h>
#include <Egueb_Smil.h>
#include <Etch.h>

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

#define ESVG_LOG_COLOR_DEFAULT EINA_COLOR_ORANGE
/* Whenever a file needs to generate a log, it must declare this first */

#define ESVG_LOG_DEFAULT egueb_svg_log_dom_global

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(ESVG_LOG_DEFAULT, __VA_ARGS__)

#ifdef WARN
# undef WARN
#endif
#define WARN(...) EINA_LOG_DOM_WARN(ESVG_LOG_DEFAULT, __VA_ARGS__)

#ifdef INFO
# undef INFO
#endif
#define INFO(...) EINA_LOG_DOM_INFO(ESVG_LOG_DEFAULT, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(ESVG_LOG_DEFAULT, __VA_ARGS__)

extern int egueb_svg_log_dom_global;

#define EGUEB_SVG_ELEMENT_ATTR_SIMPLE_GET(attr, val)				\
	if (egueb_dom_attr_type_is_set(attr, EGUEB_DOM_ATTR_TYPE_BASE))		\
	{									\
		egueb_dom_attr_get(attr, EGUEB_DOM_ATTR_TYPE_BASE, &val->base);	\
	}									\
	else									\
	{									\
		egueb_dom_attr_get(attr, EGUEB_DOM_ATTR_TYPE_DEFAULT,		\
				&val->base);					\
	}									\
	if (egueb_dom_attr_type_is_set(attr, EGUEB_DOM_ATTR_TYPE_ANIMATED))	\
	{									\
		egueb_dom_attr_get(attr, EGUEB_DOM_ATTR_TYPE_ANIMATED,		\
			&val->anim);						\
	}									\
	else									\
	{									\
		val->anim = val->base;						\
	}

#endif
