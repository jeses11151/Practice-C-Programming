# configure.ac

AC_PREREQ(2.61)

AC_INIT([q8_multithread],[1.0],[joshua.chin@netprisma.com])
AM_INIT_AUTOMAKE(-Wall -Werror foreign)
AC_CONFIG_HEADER([config.h])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL                 
AC_PROG_AWK
AM_PROG_AR
AC_PROG_INSTALL 
AC_PROG_MAKE_SET

# Check for pthread library
AC_CHECK_LIB([pthread], [pthread_create])

# Allow cross-compiling
AC_CANONICAL_HOST

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
