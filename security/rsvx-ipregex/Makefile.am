AM_LDFLAGS=-version-info 3:0:3
plugindir=$(PLUGIN_PATH)
ALL_INCLUDES=$(STANDARD_INCLUDES) -I$(srcdir)/../.. -I$(srcdir)/../../include/rservr -I$(srcdir)/../../include
AM_CPPFLAGS=-DENABLE_VIRT_ARRAY_ALL $(ALL_INCLUDES)
plugin_LTLIBRARIES=librsvx-ipregex.la
librsvx_ipregex_la_SOURCES=rsvx-ipregex.cpp
librsvx_ipregex_la_LIBADD=
librsvx_ipregex_la_LDFLAGS=-shared
