INCLUDES = \
	-I$(top_srcdir) \
	-DG_LOG_DOMAIN=\"xfce4-remotenotify-plugin\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	-DSRC_DIR=\"$(abs_builddir)\" \
	$(PLATFORM_CPPFLAGS)

#
# RemoteNotify plugin
#
plugin_PROGRAMS = \
	xfce4-remotenotify-plugin

plugindir = \
	/usr/lib/xfce4/panel-plugins

xfce4_remotenotify_plugin_SOURCES = \
	xfce4-remotenotify-plugin.c \
	xfce4-remotenotify-plugin.h \
	ssh.c \
	ssh.h \
	parse.c \
	parse.h \
	remote.h \
	driver.h \
	driver.c \
	addhost.c \
	addhost.h \
	displayhosts.c \
	displayhosts.h \
	deletehost.h \
	deletehost.c \
	configure.c \
	configure.h \
	notification.c \
	notification.h

xfce4_remotenotify_plugin_CFLAGS = \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4UI_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(LIBPCRE_CFLAGS) \
	$(LIBSSH2_CFLAGS) \
	$(LIBGCRYPT_CFLAGS) \
	$(LIBGNUTLS_CFLAGS) \
	$(LIBNOTIFY_CFLAGS) \
	$(PLATFORM_CFLAGS)

xfce4_remotenotify_plugin_LDADD = \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4UI_LIBS) \
	$(LIBXFCE4PANEL_LIBS) \
	$(LIBSSH2_LIBS) \
	$(LIBGCRYPT_LIBS) \
	$(LIBGNUTLS_LIBS) \
	$(LIBNOTIFY_LIBS) \
	$(LIBPCRE_LIBS)

#
# Desktop file
#
desktopdir =								\
	$(datadir)/xfce4/panel-plugins

desktop_in_in_files =							\
	remotenotify.desktop.in.in

desktop_in_files =							\
	$(desktop_in_in_files:.desktop.in.in=.desktop.in)

%.desktop.in: %.desktop.in.in
	sed -e "s,\@libexecdir\@,$(libexecdir),g" < $< > $@

desktop_DATA =								\
	$(desktop_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	$(desktop_in_in_files)

CLEANFILES =								\
	$(desktop_in_files)						\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
