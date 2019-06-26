AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I./include

lib_LTLIBRARIES = libxstdtsl.la
libxstdtsl_la_SOURCES = src/system.cpp src/xstdtsl_mutex.cpp
libxstdtsl_la_CFLAGS = --std=c++17 -pthread
libxstdtsl_la_LDFLAGS = -version-info 0:0:0
check_PROGRAMS = xstdtsl_mutex_test_exe xstdtsl_vector_test_exe
xstdtsl_mutex_test_exe_SOURCES = src/xstdtsl_mutex_test.cpp src/xstdtsl_mutex_test_common.cpp
xstdtsl_mutex_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_mutex_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_vector_test_exe_SOURCES = src/xstdtsl_vector_test.cpp src/xstdtsl_vector_test_common.cpp
xstdtsl_vector_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_vector_test_exe_LDFLAGS = -lpthread -lxstdtsl

include_HEADERS = include/xstdtsl_mutex include/xstdtsl_system_C.h include/xstdtsl_mutex_C.h include/xstdtsl_vector
noinst_HEADERS = include/xstdtsl_mutex_internal.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc
TESTS = xstdtsl_mutex_test_exe
