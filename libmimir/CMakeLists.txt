add_library(mimir STATIC)
target_sources(mimir
  PRIVATE
    src/scanner.cpp
    src/signature.cpp
    src/settings.cpp
    src/theme.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
    include/mimir/scanner.hh
    include/mimir/signature.hh
    include/mimir/settings.hh
    include/mimir/theme.hh
)

target_compile_features(mimir PUBLIC cxx_std_26)
target_compile_options(mimir PRIVATE -Wall -Wextra -Wpedantic)

find_library(CLAMAV clamav)
if(NOT CLAMAV_FOUND)
  message(FATAL_ERROR "libclamav not found")
endif()
target_link_options(mimir PUBLIC -lclamav)

find_program(CLAMDSCAN clamdscan)
find_program(CLAMSCAN clamscan)
find_program(FRESHCLAM freshclam)

# TODO: find_program checks
