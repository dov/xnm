AUTOMAKE_OPTIONS = gnu 
SUBDIRS = src 

#include aminclude.am

libxnmdocdir = ${prefix}/doc/libxnm
libxnmdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = autogen.sh $(DX_CONFIG) $(libxnmdoc_DATA) doxygen-doc/html

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxnm.pc

