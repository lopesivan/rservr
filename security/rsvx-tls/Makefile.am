AM_LDFLAGS=-version-info 3:0:3
plugindir=$(PLUGIN_PATH)
ALL_INCLUDES=$(STANDARD_INCLUDES) -I$(srcdir)/../.. -I$(srcdir)/../../include/rservr -I$(srcdir)/../../include
AM_CPPFLAGS=-DENABLE_VIRT_ARRAY_ALL $(ALL_INCLUDES)
plugin_LTLIBRARIES=librsvx-tls.la
librsvx_tls_la_SOURCES=rsvx-tls.cpp
librsvx_tls_la_LIBADD=-lgnutls -lgcrypt

if LINK_EXTRA
  librsvx_tls_la_LIBADD+= -lgnutls-extra
endif

librsvx_tls_la_LDFLAGS=-shared
