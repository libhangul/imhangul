## Makefile.am for gtk+ hangul modules
#AUTOMAKE_OPTIONS = foreign

SUBDIRS = po

EXTRA_DIST = test.sh gtkrc hanjatable.py imhangul.conf

moduledir = @GTK_IM_MODULE_DIR@

im_hangul_la_SOURCES = \
	gtkimcontexthangul.c 	\
	gtkimcontexthangul.h 	\
	gettext.h		\
	imhangul.c

im_hangul_la_CFLAGS = \
	-DIM_HANGUL_LOCALEDIR=\"$(prefix)/share/locale\"	\
	-DG_DISABLE_DEPRECATED                          \
	-DGDK_PIXBUF_DISABLE_DEPRECATED                 \
	-DGDK_DISABLE_DEPRECATED                        \
	-DGTK_DISABLE_DEPRECATED                        \
	$(GTK_CFLAGS)					\
	$(LIBHANGUL_CFLAGS)

im_hangul_la_LDFLAGS = -rpath $(moduledir) -module -avoid-version -no-undefined
im_hangul_la_LIBADD = $(GTK_LIBS) $(LIBHANGUL_LIBS)

module_LTLIBRARIES = im-hangul.la

noinst_PROGRAMS = entry
entry_SOURCES = entry.c
entry_CFLAGS = $(GTK_CFLAGS)
entry_LDADD = $(GTK_LIBS)

install-data-hook:
	if test -z "$(DESTDIR)" ; then \
		GTK_IM_MODULE_FILE=$(GTK_IM_MODULE_FILE) ; \
		GTK_QUERY_IM_MODULE=`which gtk-query-immodules-3.0` ; \
		GTK_IM_MODULE_FILE_DIR=`dirname $$GTK_IM_MODULE_FILE` ; \
		if test ! -d $$GTK_IM_MODULE_FILE_DIR ; then \
			$(mkinstalldirs) $$GTK_IM_MODULE_FILE_DIR; \
		fi; \
		if test -x $$GTK_QUERY_IM_MODULE; then \
			if test -w $$GTK_IM_MODULE_FILE_DIR; then \
				$$GTK_QUERY_IM_MODULE > $$GTK_IM_MODULE_FILE ; \
			fi; \
		fi; \
	fi

uninstall-hook:
	if test -z "$(DESTDIR)" ; then \
		GTK_IM_MODULE_FILE=$(GTK_IM_MODULE_FILE) ; \
		GTK_QUERY_IM_MODULE=`which gtk-query-immodules-3.0` ; \
		if test -x $$GTK_QUERY_IM_MODULE; then \
			$$GTK_QUERY_IM_MODULE > $$GTK_IM_MODULE_FILE ; \
		fi; \
	fi

DISTCHECK_CONFIGURE_FLAGS = \
	--with-gtk-im-module-dir=$${dc_install_base}/lib/gtk-3.0/immodules \
	--with-gtk-im-module-file=$${dc_install_base}/etc/gtk-3.0/gtk.immodules

dist-hook:
	if test -d .git ; then \
		git log --name-status --date=iso > $(distdir)/ChangeLog ; \
	fi
