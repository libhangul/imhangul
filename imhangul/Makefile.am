## Makefile.am for gtk+ hangul modules
#AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = test.sh gtkrc gen.py

INCLUDES = -DGTK_LOCALEDIR=\"$(prefix)/share/locale\"
LDADDS = @GTK_DEP_LIBS@

moduledir = $(libdir)/gtk-2.0/$(GTK_BINARY_VERSION)/immodules

GTK_IMMODULES= $(DESTDIR)$(sysconfdir)/gtk-2.0/gtk.immodules

im_hangul2_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul2_la_SOURCES = imhangul2.c imhangul.h hanjatable.h
im_hangul2_la_LIBADD = $(LDADDS)

im_hangul3_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul3_la_SOURCES = imhangul3.c imhangul.h hanjatable.h
im_hangul3_la_LIBADD = $(LDADDS)

CFLAGS = @GTK_CFLAGS@ -g -Wall

module_LTLIBRARIES = im-hangul2.la im-hangul3.la

noinst_PROGRAMS = entry
entry_SOURCES = entry.c
entry_LDADD = @GTK_LIBS@

install-data-local:
	gtk-query-immodules-2.0 > $(GTK_IMMODULES)

uninstall:
	gtk-query-immodules-2.0 > $(GTK_IMMODULES)

