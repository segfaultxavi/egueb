#ifndef _ESVG_PRIVATE_STOP_H_
#define _ESVG_PRIVATE_STOP_H_

void esvg_element_stop_init(void);
void esvg_element_stop_shutdown(void);
Enesim_Renderer_Gradient_Stop * esvg_element_stop_gradient_stop_get(Edom_Tag *t);

#endif