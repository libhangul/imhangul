THEMES = \
	SimplyRed \
	MSWindows \
	Mac \
	keyboard \
	Onion \
	KingSejong

THEMES_DIR = @IMHANGUL_STATUS_THEMES_DIR@

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(THEMES_DIR); \
	for dir in $(THEMES) ; do \
		$(mkinstalldirs) $(DESTDIR)$(THEMES_DIR)/$$dir; \
		$(INSTALL_DATA) $(srcdir)/$$dir/none.png $(DESTDIR)$(THEMES_DIR)/$$dir; \
		$(INSTALL_DATA) $(srcdir)/$$dir/hangul.png $(DESTDIR)$(THEMES_DIR)/$$dir; \
		$(INSTALL_DATA) $(srcdir)/$$dir/english.png $(DESTDIR)$(THEMES_DIR)/$$dir; \
		test -f $(srcdir)/$$dir/README && \
			$(INSTALL_DATA) $(srcdir)/$$dir/README $(DESTDIR)$(THEMES_DIR)/$$dir; \
	done

EXTRA_DIST = \
	MSWindows/english.png		\
	MSWindows/hangul.png		\
	MSWindows/none.png		\
	Mac/english.png			\
	Mac/hangul.png			\
	Mac/none.png			\
	SimplyRed/english.png		\
	SimplyRed/hangul.png		\
	SimplyRed/none.png		\
	Onion/english.png		\
	Onion/README			\
	Onion/hangul.png		\
	Onion/none.png			\
	keyboard/english.png		\
	keyboard/README			\
	keyboard/hangul.png		\
	keyboard/none.png
