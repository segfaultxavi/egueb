if BUILD_EGUEB_SVG

enesim_image_svg_LTLIBRARIES = src/modules/enesim_image/enesim_image_svg.la
enesim_image_svgdir = @ENESIM_MODULE_DIR@

src_modules_enesim_image_enesim_image_svg_la_SOURCES = \
src/modules/enesim_image/enesim_image_svg.c

src_modules_enesim_image_enesim_image_svg_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib/css \
-I$(top_srcdir)/src/lib/dom \
-I$(top_srcdir)/src/lib/svg \
-DEGUEB_SVG_BUILD \
@EGUEB_SVG_CFLAGS@

src_modules_enesim_image_enesim_image_svg_la_LIBADD = \
$(top_builddir)/src/lib/dom/libegueb_dom.la \
$(top_builddir)/src/lib/svg/libegueb_svg.la \
@EGUEB_SVG_LIBS@

src_modules_enesim_image_enesim_image_svg_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_enesim_image_enesim_image_svg_la_LIBTOOLFLAGS = --tag=disable-static

endif
