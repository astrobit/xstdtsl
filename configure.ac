# Release version
m4_define([package_name],[xstdtsl])
m4_define([package_name_long],[xstdtsl])
m4_define([package_version_major],[0])
m4_define([package_version_minor],[1])
m4_define([package_version_micro],[0])
m4_define([package_repository],[https://github.com/astrobit/stdts])
m4_define([package_auth_email],[bwmulligan@astronaos.com])

# API version
m4_define([api_version],package_version_major)

AC_PREREQ([2.69])
AC_INIT(package_name,package_version_major.package_version_minor.package_version_micro,package_auth_email,package_name_long,package_repository)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([include/xstdtsl_mutex_internal.hpp])

# LT_PREREQ(ver?)
LT_INIT

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

#MOD_PROG_ENVMOD
#MOD_INSTALLDIR

AM_INIT_AUTOMAKE

#AC_PROG_RANLIB

AC_LANG(C++)

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_testcancel])

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

#AX_CXX_COMPILE_STDCXX(11,noext,mandatory) #use c++11 standard, required in a variety of places

#AX_PTHREAD #needed for anything using EngineMk1
#include <libxml/parser.h>
#include <libxml/tree.h>
#AC_CHECK_HEADER(/usr/include/libxml2/libxml/parser.h, AC_SUBST([xml2_CFLAGS],-I/usr/include/libxml2), AC_MSG_ERROR([could not find xml2 parser.h]))
#AC_CHECK_LIB(xml2, htmlParseFile,AC_SUBST([xml2_LIBS],-lxml2) , AC_MSG_ERROR([could not find libxml2 with html parsing]))
#PKG_CHECK_MODULES([xml2],[libxml-2.0],[],[AC_MSG_ERROR([" Could not find libxml2 development package"])])


AC_SUBST([PACKAGE_VERSION_MAJOR],package_version_major)
AC_SUBST([PACKAGE_VERSION_MINOR],package_version_minor)
AC_SUBST([PACKAGE_VERSION_MICRO],package_version_micro)
AC_SUBST([PACKAGE_REPOSITORY],package_repository)
AC_SUBST([PACKAGE_AUTH_EMAIL],package_auth_email)
AC_SUBST([API_VERSION],api_version)

AC_SUBST([LIB_NAME],[xstdtsl])
AC_SUBST([LIB_DESCRIPTION],["thread-safe and library safe extensions to the standard template library"])


AC_CONFIG_FILES([Makefile $PACKAGE_NAME-$API_VERSION.pc:lib.pc.in],[],[PACKAGE_NAME='$PACKAGE_NAME' API_VERSION='$API_VERSION'])

AC_OUTPUT


