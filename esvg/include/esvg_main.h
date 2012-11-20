#ifndef _ESVG_MAIN_H_
#define _ESVG_MAIN_H_

/*
 * @defgroup Ender_Main_Group Main
 * @{
 */
EAPI int esvg_init(void);
EAPI int esvg_shutdown(void);

extern Ender_Property *EDOM_TOPMOST;
extern Ender_Property *EDOM_CHILD;
extern Ender_Property *EDOM_PARENT;
extern Ender_Property *EDOM_CDATA;
extern Ender_Property *EDOM_TEXT;

/**
 * @}
 */


#endif
