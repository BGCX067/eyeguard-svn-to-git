## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = eyeguard.ui \
	icon16.png \
	icon32.png \
	icon64.png \
	icon128.png


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(EYEGUARD_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = eyeguard

eyeguard_SOURCES = \
	main.c \
	eyeguard.h \
	EgSetting.c \
	EgSetting.h \
	EgAbout.c

eyeguard_LDFLAGS = \
	-Wl,--export-dynamic

eyeguard_LDADD = $(EYEGUARD_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

