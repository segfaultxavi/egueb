#ifndef _ESVG_ELEMENT_GRADIENT_H_
#define _ESVG_ELEMENT_GRADIENT_H_

EAPI Eina_Bool esvg_is_gradient(Ender_Element *e);
EAPI void esvg_element_gradient_units_set(Ender_Element *e, Esvg_Element_Gradient_Units units);
EAPI void esvg_element_gradient_units_get(Ender_Element *e, Esvg_Element_Gradient_Units *units);
EAPI Eina_Bool esvg_element_gradient_units_is_set(Ender_Element *e);
EAPI void esvg_element_gradient_transform_set(Ender_Element *e, const Enesim_Matrix *transform);
EAPI void esvg_element_gradient_transform_get(Ender_Element *e, Enesim_Matrix *transform);
EAPI Eina_Bool esvg_element_gradient_transform_is_set(Ender_Element *e);
EAPI void esvg_element_gradient_spread_method_set(Ender_Element *e, Esvg_Spread_Method spread_method);
EAPI void esvg_element_gradient_spread_method_get(Ender_Element *e, Esvg_Spread_Method *spread_method);
EAPI void esvg_element_gradient_href_set(Ender_Element *e, const char *href);
EAPI void esvg_element_gradient_href_get(Ender_Element *e, const char **href);

#endif
