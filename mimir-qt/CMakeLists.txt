cmake_minimum_required(VERSION 3.28)

if(NOT ENABLE_QT)
  return()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(mimir-qt WIN32 MACOSX_BUNDLE)
target_sources(mimir-qt
  PRIVATE
    src/main.cpp
    src/application.cpp
    src/mainwindow.cpp
)

target_link_libraries(mimir-qt PRIVATE mimir Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_features(mimir-qt PRIVATE cxx_std_26)
target_compile_options(mimir-qt PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS mimir-qt RUNTIME DESTINATION bin)
