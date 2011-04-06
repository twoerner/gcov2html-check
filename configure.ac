dnl Copyright (C) 2011  Trevor Woerner

AC_INIT([gcov2html-check], 0.1.1, twoerner@gmail.com, gcov2html-check)
AC_CONFIG_SRCDIR(src/gcov2html-check.c)
AC_CONFIG_AUX_DIR(cfg)
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-bzip2 1.9])
AM_CONFIG_HEADER(cfg/config.h)

SUBDIRS="src doc"

dnl **********************************
dnl checks for programs
dnl **********************************
AC_PROG_CC
AC_PROG_CPP

AC_PROG_MAKE_SET

AC_PROG_INSTALL
AC_PROG_LN_S

AC_CHECK_PROG(doc_html, docbook2html, docbook2html, [true],)
AC_CHECK_PROG(doc_pdf,  docbook2pdf,  docbook2pdf,  [true],)
AC_CHECK_PROG(doc_txt,  docbook2txt,  docbook2txt,  [true],)

dnl **********************************
dnl checks for user features
dnl **********************************
AC_MSG_CHECKING([whether or not to build module documentation])
AC_ARG_ENABLE(docs,
	AS_HELP_STRING([--enable-docs],
		[build module documentation (default NO)]),
	[case "${enableval}" in
		yes) docs=true  ;;
		no)  docs=false ;;
		*) AC_MSG_ERROR(bad value ${enableval} for --enable-docs) ;;
	esac],[docs=false])
AM_CONDITIONAL(BUILDDOCS, test x$docs = xtrue)
AC_MSG_RESULT([$docs])

dnl **********************************
dnl checks for libraries
dnl **********************************

dnl **********************************
dnl checks for header files
dnl **********************************
AC_HEADER_STDC

dnl **********************************
dnl checks for typedefs, structs, and
dnl compiler characteristics
dnl **********************************

dnl **********************************
dnl checks for library functions
dnl **********************************

dnl **********************************
dnl checks for system services
dnl **********************************

dnl **********************************
dnl checks for unix variants
dnl **********************************

dnl **********************************
dnl other stuff
dnl **********************************
DOCBASEDIR="\$(DESTDIR)\$(datadir)/doc/\$(PACKAGE)-\$(VERSION)"
AC_SUBST(DOCBASEDIR)
AC_SUBST(SUBDIRS)

dnl **********************************
dnl output
dnl **********************************
AC_OUTPUT(Makefile
cfg/Makefile
doc/Makefile
src/Makefile)
