## Makefile.am for gtk+ hangul modules
#AUTOMAKE_OPTIONS = foreign

SUBDIRS = po

EXTRA_DIST = test.sh gtkrc hanjatable.py

# now we use @GTK_BINARY_VERSION@ string
# moduledir = @GTK_PREFIX@/lib/gtk-2.0/@GTK_BINARY_VERSION@/immodules
moduledir = @GTK_PREFIX@/lib/gtk-2.0/immodules

GTK_IMMODULES = $(sysconfdir)/gtk-2.0/gtk.immodules

im_hangul_la_SOURCES = \
	gtkimcontexthangul.c 	\
	gtkimcontexthangul.h 	\
	hangulcandidate.h	\
	gettext.h		\
	imhangul.c

im_hangul_la_CFLAGS = \
	-DIM_HANGUL_LOCALEDIR=\"$(prefix)/share/locale\"	\
	-DG_DISABLE_DEPRECATED                          \
	-DGDK_PIXBUF_DISABLE_DEPRECATED                 \
	-DGDK_DISABLE_DEPRECATED                        \
	-DGTK_DISABLE_DEPRECATED                        \
	@GTK_CFLAGS@

im_hangul_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version -no-undefined
im_hangul_la_LIBADD = @GTK_LIBS@

module_LTLIBRARIES = im-hangul.la

noinst_PROGRAMS = entry
entry_SOURCES = entry.c
entry_CFLAGS = -Wall @GTK_CFLAGS@
entry_LDADD = @GTK_LIBS@

install-data-hook:
	if test -z "$(DESTDIR)" ; then \
		$(mkinstalldirs) $(sysconfdir)/gtk-2.0; \
		gtk-query-immodules-2.0 > $(DESTDIR)$(GTK_IMMODULES); \
	fi

uninstall:
	if test -z "$(DESTDIR)" ; then \
		gtk-query-immodules-2.0 > $(DESTDIR)$(GTK_IMMODULES); \
	fi
