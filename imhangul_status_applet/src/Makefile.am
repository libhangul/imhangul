SUBDIRS = themes

INCLUDES = \
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"		\
	-DIMHANGUL_STATUS_THEMES_DIR=\"@IMHANGUL_STATUS_THEMES_DIR@\"

CFLAGS = -Wall -g @IMHANGUL_STATUS_CFLAGS@

libexec_PROGRAMS = imhangul-status-applet

imhangul_status_applet_SOURCES = \
	none.xpm 		\
	english.xpm 		\
	hangul.xpm 		\
	imhangul_status.c

imhangul_status_applet_LDADD = $(IMHANGUL_STATUS_LIBS)

schemadir = @GCONF_SCHEMA_FILE_DIR@
schema_in_files = imhangul_status.schemas.in
schema_DATA = $(schema_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

install-data-local:
	if test -z "$(DESTDIR)" ; then \
		GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA) ; \
	fi


serverdir = $(libdir)/bonobo/servers
server_in_files = GNOME_IMHangulStatusApplet.server.in	
server_DATA = $(server_in_files:.server.in=.server)
$(server_in_files): $(server_in_files:.server.in=.server.in.in)
	sed -e "s|\@LIBEXECDIR\@|$(libexecdir)|" $< > $@
@INTLTOOL_SERVER_RULE@


uidir = $(datadir)/gnome-2.0/ui
ui_DATA = GNOME_IMHangulStatusApplet.xml

EXTRA_DIST = \
	GNOME_IMHangulStatusApplet.server.in.in \
	imhangul_status.schemas.in \
	$(ui_DATA)

