## Makefile.am for gtk+ hangul modules
#AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = test.sh gtkrc hanjatable.py

INCLUDES = -DGTK_LOCALEDIR=\"@GTK_PREFIX@/share/locale\"
LDADDS = @GTK_LIBS@

# now we do not use @GTK_BINARY_VERSION@ string
# moduledir = @GTK_PREFIX@/lib/gtk-2.0/@GTK_BINARY_VERSION@/immodules
moduledir = @GTK_PREFIX@/lib/gtk-2.0/immodules

GTK_IMMODULES= $(sysconfdir)/gtk-2.0/gtk.immodules

im_hangul2_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul2_la_SOURCES = imhangul2.c imhangul.h hanjatable.h
im_hangul2_la_LIBADD = $(LDADDS)

im_hangul3f_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul3f_la_SOURCES = imhangul3f.c imhangul.h hanjatable.h imhangul3.h
im_hangul3f_la_LIBADD = $(LDADDS)

im_hangul3s_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul3s_la_SOURCES = imhangul3s.c imhangul.h hanjatable.h imhangul3.h
im_hangul3s_la_LIBADD = $(LDADDS)

im_hangul39_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul39_la_SOURCES = imhangul39.c imhangul.h hanjatable.h imhangul3.h
im_hangul39_la_LIBADD = $(LDADDS)

im_hangul3y_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul3y_la_SOURCES = imhangul3y.c imhangul.h imhangul3.h hanjatable.h
im_hangul3y_la_LIBADD = $(LDADDS)

im_hangul32_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version
im_hangul32_la_SOURCES = imhangul32.c imhangul.h imhangul3.h hanjatable.h
im_hangul32_la_LIBADD = $(LDADDS)

CFLAGS = @GTK_CFLAGS@ -g -Wall

module_LTLIBRARIES = \
	im-hangul2.la \
	im-hangul3f.la \
	im-hangul39.la \
	im-hangul3s.la \
	im-hangul3y.la \
	im-hangul32.la

noinst_PROGRAMS = entry
entry_SOURCES = entry.c
entry_LDADD = @GTK_LIBS@

install-data-local:
	if test -z "$(DESTDIR)" ; then \
		gtk-query-immodules-2.0 > $(DESTDIR)/$(GTK_IMMODULES); \
	fi

uninstall:
	if test -z "$(DESTDIR)" ; then \
		gtk-query-immodules-2.0 > $(DESTDIR)/$(GTK_IMMODULES); \
	fi
