#ifndef _ESVG_ELEMENT_H_
#define _ESVG_ELEMENT_H_

/* core */
EAPI Eina_Bool esvg_is_element(Ender_Element *e);
EAPI Esvg_Type esvg_element_type_get(Ender_Element *e);
EAPI Eina_Bool esvg_element_setup(Ender_Element *e, Enesim_Error **error);
/* attributes */
EAPI void esvg_element_id_set(Ender_Element *e, const char *id);
EAPI void esvg_element_id_get(Ender_Element *e, const char **id);
EAPI void esvg_element_class_set(Ender_Element *e, const char *id);
EAPI void esvg_element_transform_set(Ender_Element *e, const Enesim_Matrix *transform);
EAPI void esvg_element_transform_get(Ender_Element *e, Enesim_Matrix *transform);
EAPI void esvg_element_style_set(Ender_Element *e, const char *style);
EAPI void esvg_element_style_get(Ender_Element *e, const char **style);
EAPI Eina_Bool esvg_element_style_is_set(Ender_Element *e);
EAPI void esvg_element_clip_path_set(Ender_Element *e, const char *id);
EAPI void esvg_element_clip_path_unset(Ender_Element *e);
EAPI void esvg_element_opacity_set(Ender_Element *e, double opacity);
EAPI void esvg_element_opacity_unset(Ender_Element *e);
EAPI void esvg_element_color_set(Ender_Element *e, const Esvg_Color *color);
EAPI void esvg_element_color_unset(Ender_Element *e);
EAPI void esvg_element_fill_set(Ender_Element *e, const Esvg_Paint *fill);
EAPI void esvg_element_fill_unset(Ender_Element *e);
EAPI void esvg_element_fill_opacity_set(Ender_Element *e, double fill_opacity);
EAPI void esvg_element_fill_opacity_unset(Ender_Element *e);
EAPI void esvg_element_fill_rule_set(Ender_Element *e, Esvg_Fill_Rule fill_rule);
EAPI void esvg_element_stroke_set(Ender_Element *e, const Esvg_Paint *stroke);
EAPI void esvg_element_stroke_unset(Ender_Element *e);
EAPI void esvg_element_stroke_width_set(Ender_Element *e, const Esvg_Length *stroke_width);
EAPI void esvg_element_stroke_width_unset(Ender_Element *e);
EAPI void esvg_element_stroke_opacity_set(Ender_Element *e, double stroke_opacity);
EAPI void esvg_element_stroke_opacity_unset(Ender_Element *e);
EAPI void esvg_element_stroke_line_cap_set(Ender_Element *e, Esvg_Stroke_Line_Cap cap);
EAPI void esvg_element_stroke_line_cap_unset(Ender_Element *e);
EAPI void esvg_element_stroke_line_join_set(Ender_Element *e, Esvg_Stroke_Line_Join join);
EAPI void esvg_element_stroke_line_join_unset(Ender_Element *e);
EAPI void esvg_element_visibility_set(Ender_Element *e, Eina_Bool visibility);
EAPI void esvg_element_visibility_unset(Ender_Element *e);
EAPI void esvg_element_stop_opacity_set(Ender_Element *e, double stop_opacity);
EAPI void esvg_element_stop_opacity_get(Ender_Element *e, double *stop_opacity);
EAPI void esvg_element_stop_color_set(Ender_Element *e, Esvg_Color *stop_color);
EAPI void esvg_element_stop_color_get(Ender_Element *e, Esvg_Color *stop_color);

#endif