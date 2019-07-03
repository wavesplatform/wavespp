# Searches for waves_c library
#
# The following variables are optionally searched for defaults
#  WAVES_C_ROOT_DIR:            Base directory where all WAVES_C components are found
#
# The following are set after configuration is done:
#  WAVES_C_FOUND
#  WAVES_C_INCLUDE_DIRS
#  WAVES_C_LIBRARIES
#  WAVES_C_LIBRARYRARY_DIRS
include(FindPackageHandleStandardArgs)

set(WAVES_C_ROOT_DIR "" CACHE PATH "Folder contains waves_c")

find_path(WAVES_C_INCLUDE_DIR
    NAMES waves/b58.h waves/crypto.h
    PATH_SUFFIXES include)

if(MSVC)
    find_library(WAVES_C_LIBRARY_RELEASE
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}waves_c${CMAKE_STATIC_LIBRARY_SUFFIX} waves_c
        PATHS ${WAVES_C_ROOT_DIR}
        PATH_SUFFIXES Release)

    find_library(WAVES_C_LIBRARY_DEBUG
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}waves_c${CMAKE_STATIC_LIBRARY_SUFFIX} waves_c
        PATHS ${WAVES_C_ROOT_DIR}
        PATH_SUFFIXES Debug)

    set(WAVES_C_LIBRARY optimized ${WAVES_C_LIBRARY_RELEASE} debug ${WAVES_C_LIBRARY_DEBUG})
else()
    find_library(WAVES_C_LIBRARY
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}waves_c${CMAKE_STATIC_LIBRARY_SUFFIX} waves_c
        PATHS ${WAVES_C_ROOT_DIR}
        PATH_SUFFIXES lib/waves lib64/waves)
endif()

find_package_handle_standard_args(WAVES_C DEFAULT_MSG WAVES_C_INCLUDE_DIR WAVES_C_LIBRARY)

if(WAVES_C_FOUND)
  set(WAVES_C_INCLUDE_DIRS ${WAVES_C_INCLUDE_DIR})
  set(WAVES_C_LIBRARIES ${WAVES_C_LIBRARY})
  message(STATUS "Found waves_c    (include: ${WAVES_C_INCLUDE_DIR}, library: ${WAVES_C_LIBRARY})")
  mark_as_advanced(WAVES_C_ROOT_DIR WAVES_C_LIBRARY_RELEASE WAVES_C_LIBRARY_DEBUG
                                 WAVES_C_LIBRARY WAVES_C_INCLUDE_DIR)
endif()
