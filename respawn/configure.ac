## CopyRight (c) 2009-2010 Mikhail Krivtsov

## Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([respawn], [0.3.4.1], [mk@pisem.net])
AC_CONFIG_SRCDIR([src/respawn.c])
#AC_CONFIG_AUX_DIR([build-aux])
#AM_INIT_AUTOMAKE([1.10.1 -Wall -Werror foreign])
AM_INIT_AUTOMAKE([1.15 -Wall -Werror no-dist-gzip dist-xz subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AC_CHECK_LIB([rt], [clock_gettime])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([clock_gettime gettimeofday localtime_r memset strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
