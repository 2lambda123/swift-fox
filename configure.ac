#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Swift Fox Compiler], [0.79.1], [marcin@ieee.org], [sfc])
AC_CONFIG_SRCDIR([src/sf_driver.c])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AM_INIT_AUTOMAKE


AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit memset mkdir strdup])

#AC_CONFIG_FILES([Makefile
#		doc/Makefile])
#                 doc/manual/Makefile
#                 doc/presentation/Makefile
#                 doc/report/Makefile
#                 doc/tutorial/Makefile
#                 doc/whitepaper/Makefile])
AC_OUTPUT(
	Makefile
	src/Makefile
	doc/Makefile
	examples/Makefile
	man/Makefile)

