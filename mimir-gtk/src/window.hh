#pragma once

#include <gtk/gtk.h>

#define MIMIR_TYPE_WINDOW (mimir_window_get_type())
G_DECLARE_FINAL_TYPE(MimirWindow, mimir_window, MIMIR, WINDOW, GtkApplicationWindow)

GtkWidget* mimir_window_new();