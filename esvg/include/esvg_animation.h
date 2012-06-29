#ifndef _ESVG_ANIMATION_H_
#define _ESVG_ANIMATION_H_

EAPI void esvg_animation_attribute_name_set(Ender_Element *e, const char *name);
EAPI void esvg_animation_attribute_name_get(Ender_Element *e, const char **name);
EAPI void esvg_animation_attribute_type_set(Ender_Element *e, Esvg_Attribute_Type type);
EAPI void esvg_animation_attribute_type_get(Ender_Element *e, Esvg_Attribute_Type *type);
EAPI void esvg_animation_dur_set(Ender_Element *e, Esvg_Duration *v);
EAPI void esvg_animation_dur_get(Ender_Element *e, Esvg_Duration *v);
EAPI void esvg_animation_additive_set(Ender_Element *e, Esvg_Additive *additive);
EAPI void esvg_animation_additive_get(Ender_Element *e, Esvg_Additive *additive);
EAPI void esvg_animation_accumulate_set(Ender_Element *e, Esvg_Accumulate *accumulate);
EAPI void esvg_animation_accumulate_get(Ender_Element *e, Esvg_Accumulate *accumulate);

#endif

