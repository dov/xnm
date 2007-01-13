## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(PACKAGE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libxnm-1.0.la
libxnm_1_0_la_LIBADD =		\
	$(GLIB_LIBS)			

libxnm_1_0_la_SOURCES =      \
        xnm-parser.y         \
        xnm-struct.c         \
        xnm-retrieve.c         

noinst_PROGRAMS = test-libxnm xnm-parse-and-query

test_libxnm_LDADD = libxnm-1.0.la $(GLIB_LIBS)
xnm_parse_and_query_LDADD = libxnm-1.0.la $(GLIB_LIBS)
