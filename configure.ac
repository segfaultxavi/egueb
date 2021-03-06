# get rid of that stupid cache mechanism
rm -f config.cache

ENS_VERSION([0], [0], [23], [1])

AC_INIT([egueb], [v_ver], [egueb-devel@googlegroups.com])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_USE_SYSTEM_EXTENSIONS

## Add parameters for aclocal (keep in sync with Makefile.am)
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4 -I common/m4")

# compiler
AC_PROG_CC
AC_PROG_CXX

AM_INIT_AUTOMAKE([foreign 1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_PREREQ([2.2])
LT_INIT([win32-dll])

version_info="lt_cur:lt_rev:lt_age"
AC_SUBST([version_info])
AC_DEFINE_UNQUOTED([VERSION_MAJOR], [v_maj], [Major version])
AC_DEFINE_UNQUOTED([VERSION_MINOR], [v_min], [Minor version])
AC_DEFINE_UNQUOTED([VERSION_MICRO], [v_mic], [Micro version])

VMAJ=v_maj
AC_SUBST([VMAJ])

### Needed information

EINA_VERSION="1.0.99"
ENESIM_VERSION="0.0.18"
ENDER_VERSION="0.0.8"

AC_CANONICAL_HOST

egueb_dom_requirements="enesim >= ${ENESIM_VERSION} ender >= ${ENDER_VERSION} eina >= ${EINA_VERSION}"
egueb_dom_requirements_private=""
egueb_css_requirements="eina >= ${EINA_VERSION} ${egueb_dom_requirements}"
egueb_css_requirements_private=""
egueb_smil_requirements="${egueb_dom_requirements}"
egueb_svg_requirements="${egueb_dom_requirements}"
egueb_xlink_requirements="${egueb_dom_requirements}"

AC_SUBST([egueb_css_requirements])
AC_SUBST([egueb_css_requirements_private])
AC_SUBST([egueb_dom_requirements])
AC_SUBST([egueb_dom_requirements_private])
AC_SUBST([egueb_smil_requirements])
AC_SUBST([egueb_svg_requirements])
AC_SUBST([egueb_xlink_requirements])

### Additional options to configure
AC_ARG_ENABLE([egueb_dom],
   [AS_HELP_STRING([--enable-egueb_dom], [enable Edom compilation.])],
   [if test "x${enableval}" = "xyes"; then
       want_egueb_dom="yes"
    else
       want_egueb_dom="no"
    fi
   ],
   [want_egueb_dom="yes"])

AM_CONDITIONAL([WANT_EGUEB_DOM], [test "x${want_egueb_dom}" = "xyes"])

AC_ARG_ENABLE([egueb_css],
   [AS_HELP_STRING([--enable-egueb_css], [enable Ecss compilation.])],
   [if test "x${enableval}" = "xyes"; then
       want_egueb_css="yes"
    else
       want_egueb_css="no"
    fi
   ],
   [want_egueb_css="yes"])

AM_CONDITIONAL([WANT_EGUEB_CSS], [test "x${want_egueb_css}" = "xyes"])

AC_ARG_ENABLE([egueb_xlink],
   [AS_HELP_STRING([--enable-egueb_xlink], [enable Exlink compilation.])],
   [if test "x${enableval}" = "xyes"; then
       want_egueb_xlink="yes"
    else
       want_egueb_xlink="no"
    fi
   ],
   [want_egueb_xlink="yes"])

AM_CONDITIONAL([WANT_EGUEB_XLINK], [test "x${want_egueb_xlink}" = "xyes"])

AC_ARG_ENABLE([egueb_smil],
   [AS_HELP_STRING([--enable-egueb_smil], [enable Esmil compilation.])],
   [if test "x${enableval}" = "xyes"; then
       want_egueb_smil="yes"
    else
       want_egueb_smil="no"
    fi
   ],
   [want_egueb_smil="yes"])

AM_CONDITIONAL([WANT_EGUEB_SMIL], [test "x${want_egueb_smil}" = "xyes"])

AC_ARG_ENABLE([egueb_svg],
   [AS_HELP_STRING([--enable-egueb_svg], [enable Esvg compilation.])],
   [if test "x${enableval}" = "xyes"; then
       want_egueb_svg="yes"
    else
       want_egueb_svg="no"
    fi
   ],
   [want_egueb_svg="yes"])

AM_CONDITIONAL([WANT_EGUEB_SVG], [test "x${want_egueb_svg}" = "xyes"])


### Checks for programs

# pkg-config
PKG_PROG_PKG_CONFIG

# Check whether pkg-config supports Requires.private
if ! $PKG_CONFIG --atleast-pkgconfig-version 0.22; then
   AC_MSG_ERROR([pkg-config >= 0.22 is required])
fi

ENESIM_MODULE_IMAGE_DIR=`${PKG_CONFIG} --variable=moduleimagedir enesim`
AC_SUBST([ENESIM_MODULE_IMAGE_DIR])

# flex (egueb_css dep)
if test "x${want_egueb_css}" = "xyes" ; then
   AM_PROG_LEX
   if test "x$ac_cv_prog_LEX" = "x"; then
      AC_MSG_WARN([Cannot find flex, Ecss compilation disabled])
      want_egueb_css="no"
   fi
fi

# yacc (egueb_css dep)

if test "x${want_egueb_css}" = "xyes" ; then
   AC_PROG_YACC
   if test "x$ac_cv_prog_YACC" = "x"; then
      AC_MSG_WARN([Cannot find bison, Ecss compilation disabled])
      want_egueb_css="no"
   fi
fi

# doxygen program for documentation building
ENS_CHECK_DOXYGEN([build_doc="yes"], [build_doc="no"])

### Checks for libraries
if test "x${want_egueb_dom}" = "xyes" ; then
   PKG_CHECK_MODULES([EGUEB_DOM], [${egueb_dom_requirements} ${egueb_dom_requirements_private}], [want_egueb_dom="yes"], [want_egueb_dom="no"])
   # Check the eina version
   PKG_CHECK_EXISTS([eina >= 1.8], [AC_DEFINE([EINA_POST_1_8_0], [1], [Eina supports doctype child])])
fi

if test "x${want_egueb_css}" = "xyes" && test "x${want_egueb_dom}" = "xyes" ; then
   PKG_CHECK_MODULES([EGUEB_CSS], [${egueb_css_requirements}], [want_egueb_css="yes"], [want_egueb_css="no"])
fi

if test "x${want_egueb_xlink}" = "xyes" && test "x${want_egueb_dom}" = "xyes" ; then
   PKG_CHECK_MODULES([EGUEB_XLINK], [${egueb_xlink_requirements}], [want_egueb_xlink="yes"], [want_egueb_xlink="no"])
fi

if test "x${want_egueb_smil}" = "xyes" && test "x${want_egueb_dom}" = "xyes" ; then
   PKG_CHECK_MODULES([EGUEB_SMIL], [${egueb_smil_requirements}], [want_egueb_smil="yes"], [want_egueb_smil="no"])
fi

if test "x${want_egueb_svg}" = "xyes" && test "x${want_egueb_css}" = "xyes" && test "x${want_egueb_dom}" = "xyes" && test "x${want_egueb_smil}" = "xyes" ; then
   PKG_CHECK_MODULES([EGUEB_SVG], [${egueb_svg_requirements}], [want_egueb_svg="yes"], [want_egueb_svg="no"])
fi

# For Dom test
AM_CONDITIONAL(BUILD_EGUEB_DOM, [test "x${want_egueb_dom}" = "xyes"])
AM_CONDITIONAL(BUILD_EGUEB_CSS, [test "x${want_egueb_css}" = "xyes"])
AM_CONDITIONAL(BUILD_EGUEB_XLINK, [test "x${want_egueb_xlink}" = "xyes"])
AM_CONDITIONAL(BUILD_EGUEB_SMIL, [test "x${want_egueb_smil}" = "xyes"])
AM_CONDITIONAL(BUILD_EGUEB_SVG, [test "x${want_egueb_svg}" = "xyes"])

### Checks for header files


### Checks for types


### Checks for structures


### Checks for compiler characteristics

AM_PROG_CC_C_O
AC_C_BIGENDIAN
ENS_CHECK_COMPILER_FLAGS([EGUEB], [-Wall])

EGUEB_DOM_CFLAGS="${EGUEB_DOM_CFLAGS} ${EGUEB_CFLAGS}"
EGUEB_CSS_CFLAGS="${EGUEB_CSS_CFLAGS} ${EGUEB_CFLAGS}"
EGUEB_XLINK_CFLAGS="${EGUEB_XLINK_CFLAGS} ${EGUEB_CFLAGS}"
EGUEB_SMIL_CFLAGS="${EGUEB_SMIL_CFLAGS} ${EGUEB_CFLAGS}"
EGUEB_SVG_CFLAGS="${EGUEB_SVG_CFLAGS} ${EGUEB_CFLAGS}"

### Checks for linker characteristics

### Checks for library functions

# Check where the ender data should be installed
PKG_CHECK_EXISTS([ender], [have_ender="yes"], [have_ender="no"])
if test "x${have_ender}" = "xyes"; then
   ENDER_DATADIR=$($PKG_CONFIG --variable=descriptionsdir ender)
   AC_SUBST([ENDER_DATADIR])
fi

# Unit tests, coverage and benchmarking
ENS_CHECK_COVERAGE(yes, [enable_coverage="yes"], [enable_coverage="no"])

## Make the debug preprocessor configurable

AC_CONFIG_FILES([
Makefile
common/doc/Doxyfile_common
])

AC_OUTPUT

#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo
echo "Configuration Options Summary:"
echo
echo "DOM support.................................: ${want_egueb_dom}"
echo "CSS support.................................: ${want_egueb_css}"
echo "XLINK support...............................: ${want_egueb_xlink}"
echo "SMIL support................................: ${want_egueb_smil}"
echo "SVG support.................................: ${want_egueb_svg}"
echo
echo "Installation Path...........................: ${prefix}"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo
