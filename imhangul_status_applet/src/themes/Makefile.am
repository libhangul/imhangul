THEMES = \
	SimplyRed \
	MSWindows \
	Mac \
	keyboard

THEMES_DIR = @IMHANGUL_STATUS_THEMES_DIR@

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(THEMES_DIR); \
	for dir in $(THEMES) ; do \
		$(mkinstalldirs) $(DESTDIR)$(THEMES_DIR)/$$dir; \
		for i in `find $(srcdir)/$$dir` ; do \
			$(INSTALL_DATA) $$i $(DESTDIR)$(THEMES_DIR)/$$dir; \
		done \
	done

EXTRA_DIST = \
	$(THEMES)
