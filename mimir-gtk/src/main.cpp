#include <gio/gio.h>
#include "application.hh"

extern "C" GResource* mimir_gtk_get_resource();

int main(int argc, char* argv[]) {
    g_resources_register(mimir_gtk_get_resource());
    return g_application_run(G_APPLICATION(mimir_app_new()), argc, argv);
}