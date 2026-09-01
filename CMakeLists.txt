cmake_minimum_required(VERSION 3.28)
project(Mimir VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_PROJECT_NAME STREQUAL project)
  include(cmake/Presets.cmake)
endif()

# Core library
add_subdirectory(libmimir)

# Frontends (optional)
find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(GTK4 QUIET gtk-4.0)
  if(GTK4_FOUND)
    add_subdirectory(mimir-gtk)
  endif()

  if(Qt6_FOUND)
    add_subdirectory(mimir-qt)
  endif()
else()
  # Manual Qt detection fallback
  find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
  if(Qt6_FOUND)
    add_subdirectory(mimir-qt)
  endif()
endif()
