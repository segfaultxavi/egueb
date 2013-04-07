/* Edom - DOM
 * Copyright (C) 2011 Jorge Luis Zapata
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
#include "egueb_dom_private.h"

#include "egueb_dom_main.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _init_count = 0;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
int edom_log_dom_global = -1;
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void edom_init(void)
{
	if (_init_count) goto done;
	eina_init();
	enesim_init();
	ender_init();
	edom_log_dom_global = eina_log_domain_register("edom", 0);
	if (edom_log_dom_global < 0)
	{
		EINA_LOG_ERR("Edom: Can not create a general log domain.");
		goto shutdown_eina;
	}
done:
	_init_count++;
	return;

shutdown_eina:
	eina_shutdown();
}

EAPI void edom_shutdown(void)
{
	if (_init_count != 1) goto done;
	eina_log_domain_unregister(edom_log_dom_global);
        edom_log_dom_global = -1;
	ender_shutdown();
	enesim_shutdown();
	eina_shutdown();
done:
	_init_count--;
}

/* FIXME once svg is done, use the macro from ender */
EAPI Ender_Namespace * edom_namespace_get(void)
{
	static Ender_Namespace *ns = NULL;

	if (!ns) ns = ender_namespace_new("egueb_dom", 0);
	return ns;
}
