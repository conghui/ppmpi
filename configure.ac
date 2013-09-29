# Process this file with autoconf to produce a configure script.

# Prelude.
AC_PREREQ([2.59])
AC_INIT([parallel programming with mpi], [0.1], [heconghui@gmail.com])


# place to put some extra build scripts installed
AC_CONFIG_AUX_DIR([build-aux])

# fairly severe build strictness
# change foreign to gnu or gnits to comply with gnu standards
AM_INIT_AUTOMAKE([-Wall -Werror foreign 1.9.6])

# Checks for programs.
AC_PROG_CC([mpicc gcc])

if test -z $CXXFLAGS; then
  CXXFLAGS='-g -pipe -O0 -Wall -Wextra -Weffc++ -std=c++0x'
fi
AC_PROG_CXX([mpic++ g++])
AC_PROG_LIBTOOL


# Output files
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/chap03/Makefile
                 src/chap04/Makefile
                 src/chap05/Makefile
                 src/chap06/Makefile
                 src/chap07/Makefile
                 solution/Makefile
                 solution/chap03/Makefile
                 solution/chap04/Makefile
                 ])

AC_OUTPUT
