lib_LTLIBRARIES += src/lib/smil/libegueb_smil.la

installed_smilheadersdir = $(pkgincludedir)-@VMAJ@/smil
dist_installed_smilheaders_DATA = \
src/lib/smil/Egueb_Smil.h \
src/lib/smil/egueb_smil_accumulate.h \
src/lib/smil/egueb_smil_additive.h \
src/lib/smil/egueb_smil_animation.h \
src/lib/smil/egueb_smil_clock.h \
src/lib/smil/egueb_smil_duration.h \
src/lib/smil/egueb_smil_event.h \
src/lib/smil/egueb_smil_fill.h \
src/lib/smil/egueb_smil_attr_duration.h \
src/lib/smil/egueb_smil_attr_fill.h \
src/lib/smil/egueb_smil_attr_timing_list.h \
src/lib/smil/egueb_smil_set.h \
src/lib/smil/egueb_smil_timing.h \
src/lib/smil/egueb_smil_timing_list.h

src_lib_smil_libegueb_smil_la_SOURCES = \
src/lib/smil/egueb_smil_accumulate.c \
src/lib/smil/egueb_smil_additive.c \
src/lib/smil/egueb_smil_animation.c \
src/lib/smil/egueb_smil_clock.c \
src/lib/smil/egueb_smil_duration.c \
src/lib/smil/egueb_smil_event.c \
src/lib/smil/egueb_smil_fill.c \
src/lib/smil/egueb_smil_main.c \
src/lib/smil/egueb_smil_attr_duration.c \
src/lib/smil/egueb_smil_attr_fill.c \
src/lib/smil/egueb_smil_attr_timing_list.c \
src/lib/smil/egueb_smil_set.c \
src/lib/smil/egueb_smil_timing.c \
src/lib/smil/egueb_smil_timing_list.c

src_lib_smil_libegueb_smil_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib/smil \
-I$(top_srcdir)/src/lib/dom \
-DEGUEB_SMIL_BUILD @EDOM_CFLAGS@

src_lib_smil_libegueb_smil_la_LIBADD = @EDOM_LIBS@ -letch

src_lib_smil_libegueb_smil_la_LDFLAGS = -no-undefined -version-info @version_info@

