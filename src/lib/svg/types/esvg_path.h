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
#ifndef _ESVG_PATH_H
#define _ESVG_PATH_H

typedef enum _Egueb_Svg_Element_Path_Command_Type
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
} Egueb_Svg_Element_Path_Command_Type;

typedef struct _Egueb_Svg_Element_Path_Command
{
	Egueb_Svg_Element_Path_Command_Type type;
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
} Egueb_Svg_Element_Path_Command;


typedef void (*Egueb_Svg_Command_Cb)(Egueb_Svg_Element_Path_Command *cmd, void *data);

EAPI Eina_Bool egueb_svg_path_data_string_from(const char *value, Egueb_Svg_Command_Cb cb, void *data);

typedef struct _Egueb_Svg_Path_Seg_List Egueb_Svg_Path_Seg_List;

EAPI Egueb_Svg_Path_Seg_List * egueb_svg_path_seg_list_new(void);
EAPI Egueb_Svg_Path_Seg_List * egueb_svg_path_seg_list_ref(Egueb_Svg_Path_Seg_List *thiz);
EAPI void egueb_svg_path_seg_list_unref(Egueb_Svg_Path_Seg_List *thiz);
EAPI Eina_Bool egueb_svg_path_seg_list_string_from(Egueb_Svg_Path_Seg_List *thiz,
		const char *attr);
EAPI void egueb_svg_path_seg_list_clear(Egueb_Svg_Path_Seg_List *thiz);
EAPI void egueb_svg_path_seg_list_add(Egueb_Svg_Path_Seg_List *thiz, const Egueb_Svg_Element_Path_Command *cmd);
EAPI Eina_Bool egueb_svg_path_seg_list_string_from(Egueb_Svg_Path_Seg_List *thiz,
		const char *attr);
#endif

