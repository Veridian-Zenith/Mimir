#pragma once

#include <adwaita.h>

#define MIMIR_TYPE_APP (mimir_app_get_type())
G_DECLARE_FINAL_TYPE(MimirApp, mimir_app, MIMIR, APP, AdwApplication)

MimirApp* mimir_app_new();