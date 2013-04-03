#ifndef _ESVG_PRIVATE_ELEMENT_H_
#define _ESVG_PRIVATE_ELEMENT_H_

/* FIXME given that we include every header
 * needed on every source, we better dont
 * do this
 */
#include "esvg_types.h"
#include "esvg_attribute_private.h"
#include "esvg_private_attribute_presentation.h"
#include "esvg_context_private.h"
#include "esvg_private_renderable_behaviour.h"

/* element */
typedef enum _Esvg_Element_Setup_Return
{
	ESVG_SETUP_FAILED,
	ESVG_SETUP_OK,
	ESVG_SETUP_ENQUEUE,
	ESVG_SETUP_RETURNS
} Esvg_Element_Setup_Return;

typedef struct _Esvg_Element_Event_Topmost_Changed {
	Ender_Element *previous;
	Ender_Element *current;
	Edom_Tag *child;
} Esvg_Element_Event_Topmost_Changed;

typedef struct _Esvg_Element_Context {
	double dpi_x;
	double dpi_y;
	Esvg_Rect viewbox; /* the current viewbox */
	double font_size; /* the propagated value of the current font size? FIXME here or in the attributes? */
	Enesim_Rectangle bounds; /* the bounds of the object */
	Enesim_Matrix transform; /* the current transformation */
	Esvg_Renderable_Behaviour renderable_behaviour;
} Esvg_Element_Context;

typedef void (*Esvg_Element_Initialize)(Ender_Element *e);
typedef Eina_Bool (*Esvg_Element_Attribute_Set)(Ender_Element *e, const char *key, const char *value);
/* FIXME use this instead of the add/remove */
typedef int * (*Esvg_Element_Attribute_Animated_Fetch)(Edom_Tag *t, const char *name);

typedef Esvg_Element_Setup_Return (*Esvg_Element_Setup)(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *parent_ctx,
		Esvg_Element_Context *ctx,
		Esvg_Attribute_Presentation *attr,
		Enesim_Log **error);
typedef Eina_Bool (*Esvg_Element_Setup_Interceptor)(Edom_Tag *t,
		Edom_Tag *child,
		Esvg_Context *c,
		Enesim_Log **error,
		void *data);
typedef struct _Esvg_Element_Descriptor {
	/* the tag interface */
	Edom_Tag_Child_Add child_add;
	Edom_Tag_Child_Remove child_remove;
	Edom_Tag_Attribute_Get attribute_get;
	Edom_Tag_Cdata_Set cdata_set;
	Edom_Tag_Text_Set text_set;
	Edom_Tag_Text_Get text_get;
	Edom_Tag_Free free;
	/* the element interface */
	Esvg_Element_Attribute_Set attribute_set;
	Esvg_Element_Attribute_Animated_Fetch attribute_animated_fetch;
	Esvg_Element_Initialize initialize;
	Esvg_Element_Setup setup;
} Esvg_Element_Descriptor;

void * esvg_element_data_get(Edom_Tag *t);
Edom_Tag * esvg_element_new(Esvg_Element_Descriptor *descriptor, Esvg_Type type, void *data);
Ender_Element * esvg_element_ender_get(Edom_Tag *t);
void esvg_element_attribute_type_set(Edom_Tag *t, Esvg_Attribute_Type type);
Esvg_Attribute_Type esvg_element_attribute_type_get(Edom_Tag *t);
void esvg_element_attribute_animate_set(Edom_Tag *t, Eina_Bool animate);
Eina_Bool esvg_element_attribute_animate_get(Edom_Tag *t);

void esvg_element_initialize(Ender_Element *e);

void esvg_element_ecss_style_apply(Edom_Tag *t, Ecss_Style *s);

void esvg_element_topmost_set(Edom_Tag *t, Ender_Element *topmost);
void esvg_element_state_compose(Edom_Tag *t, const Esvg_Element_Context *s, Esvg_Element_Context *d);
Eina_Bool esvg_element_changed(Edom_Tag *t);
Eina_Bool esvg_element_has_setup(Edom_Tag *t, Esvg_Context *c);
void esvg_element_context_dump(const Esvg_Element_Context *c);

const Esvg_Element_Context * esvg_element_context_get(Edom_Tag *t);
const Esvg_Attribute_Presentation * esvg_element_attribute_presentation_get(Edom_Tag *t);
Esvg_Element_Setup_Return esvg_element_setup_rel(Edom_Tag *t,
		Esvg_Context *c,
		const Esvg_Element_Context *rel_state,
		const Esvg_Attribute_Presentation *rel_attr,
		Enesim_Log **error);
Eina_Bool esvg_element_attribute_animation_add(Edom_Tag *t, const char *attr, int *index);
void esvg_element_attribute_animation_remove(Edom_Tag *t, const char *attr);
/* internal functions */
Esvg_Type esvg_element_internal_type_get(Edom_Tag *t);
const char * esvg_element_internal_id_get(Edom_Tag *t);
Eina_Bool esvg_is_element_internal(Edom_Tag *t);
void esvg_element_internal_topmost_get(Edom_Tag *t, Ender_Element **e);
Esvg_Element_Setup_Return esvg_element_internal_setup(Edom_Tag *t,
		Esvg_Context *c,
		Enesim_Log **error);
Eina_Bool esvg_element_internal_child_setup(Edom_Tag *t,
		Esvg_Context *c,
		Enesim_Log **error,
		Esvg_Element_Setup_Interceptor pre,
		Esvg_Element_Setup_Interceptor post,
		void *data);

void esvg_element_property_string_set(Ender_Element *e, Ender_Property *p, const char *v);
void esvg_element_property_length_set(Ender_Element *e, Ender_Property *p, const Esvg_Length *v);
void esvg_element_property_paint_set(Ender_Element *e, Ender_Property *p, const Esvg_Paint *v);
void esvg_element_property_clip_path_set(Ender_Element *e, Ender_Property *p, const Esvg_Clip_Path *v);
void esvg_element_property_number_set(Ender_Element *e, Ender_Property *p, const Esvg_Number *v);
void esvg_element_property_enum_set(Ender_Element *e, Ender_Property *p, int v);

void esvg_element_init(void);
void esvg_element_shutdown(void);

#endif
