/*
 * Conversation input characters count plugin.
 * Based on the Markerline plugin.
 *
 * Copyright (C) 2007 Dossy Shiobara <dossy@panoptic.com>
 * Copyright (C) 2011 Konrad Gräfe <konradgraefe@aol.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 */

#include "config.h"

#include "internal.h"

#ifndef PURPLE_PLUGINS
#define PURPLE_PLUGINS
#endif

#include <gdk/gdk.h>
#include <glib.h>
#include <gtk/gtk.h>

#include <gtkconv.h>
#include <gtkimhtml.h>
#include <gtkplugin.h>
#include <version.h>

static void
changed_text_cb(GtkTextBuffer *textbuffer, gpointer user_data)
{
	PidginConversation *gtkconv = (PidginConversation *)user_data;
	GtkWidget *box, *counter = NULL;
	gchar count[20];

	g_return_if_fail(gtkconv != NULL);

	g_snprintf(count, sizeof(count) - 1, "%u", gtk_text_buffer_get_char_count(textbuffer));

	box = gtkconv->toolbar;
	counter = g_object_get_data(G_OBJECT(box), PLUGIN_ID "-counter");
	if (counter)
		gtk_label_set_text(GTK_LABEL(counter), count);
}

static void
detach_from_gtkconv(PidginConversation *gtkconv, gpointer null)
{
	GtkWidget *box, *counter = NULL, *sep = NULL;

	g_signal_handlers_disconnect_by_func(G_OBJECT(gtkconv->entry_buffer),
			(GFunc)changed_text_cb, gtkconv);

	box = gtkconv->toolbar;
	counter = g_object_get_data(G_OBJECT(box), PLUGIN_ID "-counter");
	if (counter)
		gtk_container_remove(GTK_CONTAINER(box), counter);
	sep = g_object_get_data(G_OBJECT(box), PLUGIN_ID "-sep");
	if (sep)
		gtk_container_remove(GTK_CONTAINER(box), sep);

	gtk_widget_queue_draw(pidgin_conv_get_window(gtkconv)->window);
}

static void
attach_to_gtkconv(PidginConversation *gtkconv, gpointer null)
{
	GtkWidget *box, *sep, *counter;

	box = gtkconv->toolbar;
	counter = g_object_get_data(G_OBJECT(box), PLUGIN_ID "-counter");
	g_return_if_fail(counter == NULL);

	counter = gtk_label_new(NULL);
	gtk_widget_set_name(counter, "convcharcount_label");
	gtk_label_set_text(GTK_LABEL(counter), "0");
	gtk_box_pack_end(GTK_BOX(box), counter, FALSE, FALSE, 0);
	gtk_widget_show_all(counter);

	g_object_set_data(G_OBJECT(box), PLUGIN_ID "-counter", counter);

	sep = gtk_vseparator_new();
	gtk_box_pack_end(GTK_BOX(box), sep, FALSE, FALSE, 0);
	gtk_widget_show_all(sep);

	g_object_set_data(G_OBJECT(box), PLUGIN_ID "-sep", sep);

	/* connect signals, etc. */
	g_signal_connect(G_OBJECT(gtkconv->entry_buffer), "changed",
			G_CALLBACK(changed_text_cb), gtkconv);

	gtk_widget_queue_draw(pidgin_conv_get_window(gtkconv)->window);
}

static void
detach_from_pidgin_window(PidginWindow *win, gpointer null)
{
	g_list_foreach(pidgin_conv_window_get_gtkconvs(win), (GFunc)detach_from_gtkconv, NULL);
}

static void
attach_to_pidgin_window(PidginWindow *win, gpointer null)
{
	g_list_foreach(pidgin_conv_window_get_gtkconvs(win), (GFunc)attach_to_gtkconv, NULL);
}

static void
detach_from_all_windows()
{
	g_list_foreach(pidgin_conv_windows_get_list(), (GFunc)detach_from_pidgin_window, NULL);
}

static void
attach_to_all_windows()
{
	g_list_foreach(pidgin_conv_windows_get_list(), (GFunc)attach_to_pidgin_window, NULL);
}

static void
conv_created_cb(PurpleConversation *conv, gpointer null)
{
	PidginConversation *gtkconv = PIDGIN_CONVERSATION(conv);

	g_return_if_fail(gtkconv != NULL);

	attach_to_gtkconv(gtkconv, NULL);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	attach_to_all_windows();

	purple_signal_connect(purple_conversations_get_handle(),
			"conversation-created",
			plugin, PURPLE_CALLBACK(conv_created_cb), NULL);

	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	detach_from_all_windows();

	return TRUE;
}

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,			/**< type           */
	PIDGIN_PLUGIN_TYPE,			/**< ui_requirement */
	0,					/**< flags          */
	NULL,					/**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,		/**< priority       */

	PLUGIN_ID,				/**< id             */
	NULL,					/**< name           */
	PLUGIN_VERSION,				/**< version        */
	NULL,					/**  summary        */
				
	NULL,					/**  description    */
	PLUGIN_AUTHOR,				/**< author         */
	PLUGIN_WEBSITE,				/**< homepage       */

	plugin_load,				/**< load           */
	plugin_unload,				/**< unload         */
	NULL,					/**< destroy        */

	NULL,					/**< ui_info        */
	NULL,					/**< extra_info     */
	NULL,					/**< prefs_info     */
	NULL,					/**< actions        */
	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
	const char *str = "Character Counting";
	gchar *plugins_locale_dir;

#ifdef ENABLE_NLS
	plugins_locale_dir = g_build_filename(purple_user_dir(), "locale", NULL);

	bindtextdomain(GETTEXT_PACKAGE, plugins_locale_dir);
	if(str == _(str)) {
		bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	}
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");

	g_free(plugins_locale_dir);
#endif /* ENABLE_NLS */

	info.name        = _("Character Counting");
	info.summary     = _("Display current number of characters in a conversation's input widget.");
	info.description = _("Display current number of characters in a conversation's input widget.");
}

PURPLE_INIT_PLUGIN(PLUGIN_STATIC_NAME, init_plugin, info)

