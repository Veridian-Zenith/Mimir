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

find_library(CLAMAV_LIBRARY NAMES clamav PATHS /usr/lib /usr/lib64 /usr/local/lib NO_DEFAULT_PATH)
if(NOT CLAMAV_LIBRARY)
  find_library(CLAMAV_LIBRARY NAMES clamav)
endif()
if(NOT CLAMAV_LIBRARY)
  message(FATAL_ERROR "libclamav not found")
endif()
message(STATUS "Found libclamav: ${CLAMAV_LIBRARY}")
target_link_libraries(mimir PUBLIC ${CLAMAV_LIBRARY})

find_program(CLAMDSCAN clamdscan)
find_program(CLAMSCAN clamscan)
find_program(FRESHCLAM freshclam)

# TODO: find_program checks
