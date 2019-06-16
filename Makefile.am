AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I./include

lib_LTLIBRARIES = libxstdtsl.la
libxstdtsl_la_SOURCES = src/xstdtsl_mutex.cpp
libxstdtsl_la_CFLAGS = --std=c++17 -pthread
libxstdtsl_la_LDFLAGS = -version-info 0:0:0
check_PROGRAMS = xstdtsl_mutex_test
xstdtsl_mutex_test_SOURCES = src/xstdtsl_mutex_test.cpp
xstdtsl_mutex_test_CFLAGS = --std=c++17 -pthread
xstdtsl_mutex_test_LDFLAGS = -lpthread -lxstdtsl

include_HEADERS = include/xstdtsl_mutex

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc

