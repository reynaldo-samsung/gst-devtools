# configure.ac

AC_INIT([gst-mediainfo], [0.1])
AC_CONFIG_SRCDIR([src/mi.vala])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([check-news])

# Enable silent rules is available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_PROG_CC_STDC
AM_PROG_VALAC([0.7.0])
AC_PROG_INSTALL
AC_PROG_INTLTOOL([0.35])
PKG_PROG_PKG_CONFIG

pkg_modules="gtk+-2.0 >= 2.14.0"
MI_PACKAGES="--pkg gtk+-2.0"
PKG_CHECK_MODULES(MI, [$pkg_modules])
AC_SUBST(MI_CFLAGS)
AC_SUBST(MI_LIBS)
AC_SUBST(MI_PACKAGES)

GETTEXT_PACKAGE=gst-mi
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext Package])
AC_SUBST(GETTEXT_PACKAGE)
AM_GNU_GETTEXT_VERSION([0.12])
AM_GNU_GETTEXT([external])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	po/Makefile.in
	vapi/Makefile
])
AC_OUTPUT
