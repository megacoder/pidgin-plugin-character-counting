#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(pidgin-character-counting, 2.3)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/character-counting.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_DEFINE_UNQUOTED(PLUGIN_VERSION, ["2.3"], [Define the plugin version to be used])

AC_DEFINE_UNQUOTED(PLUGIN_WEBSITE, ["http://freakazoid.teamblind.de/2011/01/28/pidgin-character-counting/"], [Define the plugin website to be used])

AC_DEFINE_UNQUOTED(PLUGIN_ID, ["gtk-freakazoid-character-counting"], [Define the plugin website to be used])

AC_DEFINE_UNQUOTED(PLUGIN_STATIC_NAME, ["character-counting"], [Define the plugin website to be used])
AC_DEFINE_UNQUOTED(PLUGIN_PREFS_PREFIX, ["/plugins/gtk/character-counting"], [Define the plugin website to be used])

AC_DEFINE_UNQUOTED(PLUGIN_AUTHOR, ["Konrad Gräfe <konradgraefe@aol.com>"], [Define the plugin author to be used])


# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INTLTOOL
GETTEXT_PACKAGE=pidgin-character-counting
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Define the gettext package to be used])
ALL_LINGUAS="de"
AM_GLIB_GNU_GETTEXT

# Checks for libraries.
PKG_CHECK_MODULES(pidgin, [pidgin >= 2.5.0], , [AC_MSG_ERROR(pidgin >= 2.5.0 required! Please make sure that pidgin development files are installed.)])
LIBS="$LIBS $pidgin_LIBS"
CFLAGS="$CFLAGS $pidgin_CFLAGS -Wall"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 po/Makefile.in])
AC_OUTPUT

