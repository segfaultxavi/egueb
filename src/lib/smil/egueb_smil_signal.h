/* Egueb
 * Copyright (C) 2011 - 2013 Jorge Luis Zapata
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

#ifndef _EGUEB_SMIL_SIGNAL_H_
#define _EGUEB_SMIL_SIGNAL_H_

typedef struct _Egueb_Smil_Signal Egueb_Smil_Signal;
typedef void (*Egueb_Smil_Signal_State_Callback)(Egueb_Smil_Signal *s, void *data);

EAPI Egueb_Smil_Signal * egueb_smil_signal_ref(Egueb_Smil_Signal *thiz);
EAPI void egueb_smil_signal_unref(Egueb_Smil_Signal *thiz);
EAPI void egueb_smil_signal_disable(Egueb_Smil_Signal *thiz);
EAPI void egueb_smil_signal_enable(Egueb_Smil_Signal *thiz);
EAPI Eina_Bool egueb_smil_signal_enabled(Egueb_Smil_Signal *thiz);
EAPI void egueb_smil_signal_offset_set(Egueb_Smil_Signal *thiz, Egueb_Smil_Clock inc);
EAPI Egueb_Smil_Timeline * egueb_smil_signal_timeline_get(Egueb_Smil_Signal *thiz);

typedef void (*Egueb_Smil_Signal_Continuous_Process)(Egueb_Dom_Value *a, Egueb_Dom_Value *b, double m, void *data);
EAPI Egueb_Smil_Signal * egueb_smil_signal_continuous_new(
		Egueb_Smil_Signal_Continuous_Process process_cb,
		Egueb_Smil_Signal_State_Callback start_cb,
		Egueb_Smil_Signal_State_Callback stop_cb,
		Egueb_Smil_Signal_State_Callback repeat_cb,
		void *data);
EAPI void egueb_smil_signal_continuous_repeat_set(Egueb_Smil_Signal *s, int times);
EAPI void egueb_smil_signal_continuous_keyframe_simple_add(Egueb_Smil_Signal *s,
		Egueb_Smil_Keyframe_Interpolator_Type type,
		Egueb_Smil_Clock clock,
		Egueb_Dom_Value *value);
EAPI void egueb_smil_signal_continuous_keyframe_quadratic_add(Egueb_Smil_Signal *s,
		Egueb_Smil_Clock clock, double x0, double y0,
		Egueb_Dom_Value *value);

typedef void (*Egueb_Smil_Signal_Discrete_Process)(Egueb_Dom_Value *v, void *data);
EAPI Egueb_Smil_Signal * egueb_smil_signal_discrete_new(
		Egueb_Smil_Signal_Discrete_Process process_cb,
		Egueb_Smil_Signal_State_Callback start_cb,
		Egueb_Smil_Signal_State_Callback stop_cb,
		Egueb_Dom_Value *value,
		void *data);
#endif
