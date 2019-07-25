AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I./include

lib_LTLIBRARIES = libxstdtsl.la
libxstdtsl_la_SOURCES = src/system.cpp src/xstdtsl_mutex.cpp
libxstdtsl_la_CFLAGS = --std=c++17 -pthread
libxstdtsl_la_LDFLAGS = -version-info 0:0:0
check_PROGRAMS = xstdtsl_mutex_test_exe xstdtsl_vector_test_exe xstdtsl_binary_tree_test_exe xstdtsl_avl_tree_test_exe xstdtsl_rb_tree_test_exe xstdtsl_map_test_exe
xstdtsl_mutex_test_exe_SOURCES = src/xstdtsl_mutex_test.cpp src/xstdtsl_mutex_test_common.cpp
xstdtsl_mutex_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_mutex_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_vector_test_exe_SOURCES = src/xstdtsl_vector_test.cpp src/xstdtsl_vector_test_common.cpp
xstdtsl_vector_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_vector_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_binary_tree_test_exe_SOURCES = src/xstdtsl_binary_tree_test.cpp src/xstdtsl_binary_tree_test_common.cpp
xstdtsl_binary_tree_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_binary_tree_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_avl_tree_test_exe_SOURCES = src/xstdtsl_avl_tree_test.cpp src/xstdtsl_avl_tree_test_common.cpp
xstdtsl_avl_tree_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_avl_tree_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_rb_tree_test_exe_SOURCES = src/xstdtsl_rb_tree_test.cpp 
xstdtsl_rb_tree_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_rb_tree_test_exe_LDFLAGS = -lpthread -lxstdtsl
xstdtsl_map_test_exe_SOURCES = src/xstdtsl_map_test.cpp 
xstdtsl_map_test_exe_CFLAGS = --std=c++17 -pthread
xstdtsl_map_test_exe_LDFLAGS = -lpthread -lxstdtsl

include_HEADERS = include/xstdtsl_mutex include/xstdtsl_system_C.h include/xstdtsl_mutex_C.h include/xstdtsl_safe_vector  include/xstdtsl_safe_binary_tree   include/xstdtsl_safe_avl_tree    include/xstdtsl_safe_rb_tree include/xstdtsl_safe_map
noinst_HEADERS = include/xstdtsl_mutex_internal.hpp include/xstdtsl_vector_test.hpp include/xstdtsl_mutex_test.hpp include/xstdtsl_binary_tree_test.hpp include/xstdtsl_avl_tree_test.hpp include/xstdtsl_rb_tree_test.hpp

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc
TESTS = xstdtsl_mutex_test_exe xstdtsl_vector_test_exe xstdtsl_binary_tree_test_exe xstdtsl_avl_tree_test_exe xstdtsl_rb_tree_test_exe xstdtsl_map_test_exe
