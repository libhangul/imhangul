## Makefile.am for gtk+ hangul modules
#AUTOMAKE_OPTIONS = foreign

SUBDIRS = po

EXTRA_DIST = test.sh gtkrc hanjatable.py imhangul.conf

moduledir = @GTK_IM_MODULE_DIR@

im_hangul_la_SOURCES = \
	gtkimcontexthangul.c 	\
	gtkimcontexthangul.h 	\
	candidatetable.h	\
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
entry_CFLAGS = @GTK_CFLAGS@
entry_LDADD = @GTK_LIBS@

install-data-hook:
	if test -z "$(DESTDIR)" ; then \
		GTK_IM_MODULE_FILE=$(GTK_IM_MODULE_FILE) ; \
		GTK_QUERY_IM_MODULE=`which gtk-query-immodules-2.0` ; \
		GTK_IM_MODULE_FILE_DIR=`dirname $$GTK_IM_MODULE_FILE` ; \
		if test ! -d $$GTK_IM_MODULE_FILE_DIR ; then \
			if test -w `dirname $$GTK_IM_MODULE_FILE_DIR` ; then \
			    $(mkinstalldirs) `dirname $$GTK_IM_MODULE_FILE`; \
			fi; \
		fi; \
		if test -x $$GTK_QUERY_IM_MODULE; then \
		    $$GTK_QUERY_IM_MODULE > $$GTK_IM_MODULE_FILE ; \
		fi; \
	fi

uninstall:
	if test -z "$(DESTDIR)" ; then \
		GTK_IM_MODULE_FILE=$(GTK_IM_MODULE_FILE) ; \
		GTK_QUERY_IM_MODULE=`which gtk-query-immodules-2.0` ; \
		if test -x $$GTK_QUERY_IM_MODULE; then \
		    $$GTK_QUERY_IM_MODULE > $$GTK_IM_MODULE_FILE ; \
		fi; \
	fi
