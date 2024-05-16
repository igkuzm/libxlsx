AC_INIT(libxlsx, 1.0)
AM_INIT_AUTOMAKE

AC_PROG_CC

LT_INIT
AC_CHECK_HEADER([zip.h],[],[AC_MSG_ERROR([Please install libzip])],[])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT
