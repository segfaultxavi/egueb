/* Egueb_Xlink - XLINK
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
#ifndef _EGUEB_XLINK_H
#define _EGUEB_XLINK_H

#include <Egueb_Dom.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EGUEB_XLINK_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * @ender_depends{egueb_dom}
 */

/**
 * @defgroup Egueb_Dom_Group XLINK
 * @brief XLINK related API
 * @{
 */

#include "egueb_xlink_main.h"
#include "egueb_xlink_attr_href.h"

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*_EGUEB_XLINK_H*/

