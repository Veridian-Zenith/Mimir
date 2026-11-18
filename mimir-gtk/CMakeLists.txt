cmake_minimum_required(VERSION 3.28)

if(NOT ENABLE_GTK)
  return()
endif()

pkg_check_modules(GTK4 REQUIRED gtk-4.0 libadwaita-1)
find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED)

add_executable(mimir-gtk WIN32 MACOSX_BUNDLE)

# Compile gresource bundle
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/mimir-gtk-resources.c
  COMMAND ${GLIB_COMPILE_RESOURCES}
    --target=resources.c
    --sourcedir=${CMAKE_SOURCE_DIR}/mimir-gtk/data
    --generate-source
    ${CMAKE_SOURCE_DIR}/mimir-gtk/data/mimir-gtk.gresource.xml
    > ${CMAKE_BINARY_DIR}/mimir-gtk-resources.c
  DEPENDS ${CMAKE_SOURCE_DIR}/mimir-gtk/data/mimir-gtk.gresource.xml
          ${CMAKE_SOURCE_DIR}/mimir-gtk/data/window.ui
  COMMENT "Compiling GResource bundle"
  VERBATIM
)

target_sources(mimir-gtk
  PRIVATE
    src/main.cpp
    src/application.cpp
    src/window.cpp
    ${CMAKE_BINARY_DIR}/mimir-gtk-resources.c
)

target_include_directories(mimir-gtk PRIVATE
  ${GTK4_INCLUDE_DIRS}
  ${LIBADWAITA_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/libmimir/include
)

target_link_libraries(mimir-gtk PRIVATE
  mimir
  GTK4::gtk-4.0
  libadwaita-1
)

target_compile_features(mimir-gtk PRIVATE cxx_std_26)
target_compile_options(mimir-gtk PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS mimir-gtk RUNTIME DESTINATION bin)