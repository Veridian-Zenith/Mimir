#include "window.hh"
#include <mimir/scanner.hh>
#include <mimir/theme.hh>
#include <gtk/gtk.h>

struct _MimirWindow {
    GtkApplicationWindow parent_instance;
    GtkWidget* drop_zone;
    GtkWidget* scan_button;
    GtkWidget* status_label;
    GtkWidget* progress_spinner;
    mimir::scanner* scanner;
};

G_DEFINE_FINAL_TYPE(MimirWindow, mimir_window, GTK_TYPE_APPLICATION_WINDOW)

static void on_drag_data_received(GtkWidget* widget, GdkDragContext* context,
                                  gint x, gint y, GtkSelectionData* data,
                                  guint info, guint time, gpointer user_data) {
    auto* win = MIMIR_WINDOW(user_data);
    gchar** uris = gtk_selection_data_get_uris(data);
    if (!uris) {
        gtk_drag_finish(context, FALSE, FALSE, time);
        return;
    }
    for (gchar** uri = uris; *uri; ++uri) {
        GFile* file = g_file_new_for_uri(*uri);
        gchar* path = g_file_get_path(file);
        if (path) {
            auto text = std::string("Scanning: ") + path;
            gtk_label_set_text(GTK_LABEL(win->status_label), text.c_str());
            gtk_spinner_start(GTK_SPINNER(win->progress_spinner));
            gtk_widget_set_sensitive(win->scan_button, FALSE);
            g_free(path);
        }
        g_object_unref(file);
    }
    g_strfreev(uris);
    gtk_drag_finish(context, TRUE, FALSE, time);
    gtk_spinner_stop(GTK_SPINNER(win->progress_spinner));
    gtk_label_set_text(GTK_LABEL(win->status_label), "Ready");
    gtk_widget_set_sensitive(win->scan_button, TRUE);
}

static void on_scan_clicked(GtkWidget* widget, gpointer user_data) {
    auto* win = MIMIR_WINDOW(user_data);
    gtk_label_set_text(GTK_LABEL(win->status_label),
        "Drag files or folders into the drop zone above");
}

static void mimir_window_class_init(MimirWindowClass* klass) {
    GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class,
        "/com/mimir/Mimir/window.ui");
    gtk_widget_class_bind_template_child(widget_class, MimirWindow, drop_zone);
    gtk_widget_class_bind_template_child(widget_class, MimirWindow, scan_button);
    gtk_widget_class_bind_template_child(widget_class, MimirWindow, status_label);
    gtk_widget_class_bind_template_child(widget_class, MimirWindow, progress_spinner);
}

static void mimir_window_init(MimirWindow* win) {
    gtk_widget_init_template(GTK_WIDGET(win));
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

    auto* css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider,
        "window { background: #000000; }"
        "#drop_zone { background: #0D0D0D; border: 2px dashed #262626; border-radius: 12px; padding: 32px; }"
        "#drop_zone:hover { border-color: #FF1A1A; }"
        "button.suggested-action { background: #FF1A1A; color: #FFFFFF; border-radius: 8px; padding: 12px 24px; }"
        "label#status_label { color: #B3B3B3; }"
    );
    gtk_style_context_add_provider(gtk_widget_get_style_context(GTK_WIDGET(win)),
                                  GTK_STYLE_PROVIDER(css_provider),
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_drag_dest_set(win->drop_zone, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_uri_targets(win->drop_zone);

    g_signal_connect(win->drop_zone, "drag-data-received",
                    G_CALLBACK(on_drag_data_received), win);
    g_signal_connect(win->scan_button, "clicked",
                    G_CALLBACK(on_scan_clicked), win);

    win->scanner = new mimir::scanner();
}

GtkWidget* mimir_window_new() {
    return GTK_WIDGET(g_object_new(mimir_window_get_type(), nullptr));
}