#include "application.hh"
#include "window.hh"

struct _MimirApp {
    AdwApplication parent_instance;
};

G_DEFINE_FINAL_TYPE(MimirApp, mimir_app, ADW_TYPE_APPLICATION)

static void mimir_app_activate(GApplication* app) {
    auto* win = mimir_window_new();
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));
    gtk_window_present(GTK_WINDOW(win));
}

static void mimir_app_class_init(MimirAppClass* klass) {
    G_APPLICATION_CLASS(klass)->activate = mimir_app_activate;
}

static void mimir_app_init(MimirApp*) {}

MimirApp* mimir_app_new() {
    return MIMIR_APP(g_object_new(mimir_app_get_type(),
                                  "application-id", "com.mimir.Mimir",
                                  "flags", G_APPLICATION_DEFAULT_FLAGS,
                                  nullptr));
}