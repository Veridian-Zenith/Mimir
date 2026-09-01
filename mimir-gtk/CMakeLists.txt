cmake_minimum_required(VERSION 3.28)

if(NOT ENABLE_GTK)
  return()
endif()

pkg_check_modules(GTK4 REQUIRED gtk-4.0 libadwaita-1)

add_executable(mimir-gtk WIN32 MACOSX_BUNDLE)
target_sources(mimir-gtk
  PRIVATE
    src/main.cpp
    src/application.cpp
    src/window.cpp
)

target_link_libraries(mimir-gtk PRIVATE mimir GTK4::gtk-4.0 libadwaita-1)
target_compile_features(mimir-gtk PRIVATE cxx_std_26)
target_compile_options(mimir-gtk PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS mimir-gtk RUNTIME DESTINATION bin)
