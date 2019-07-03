# Searches for coda library
#
# The following variables are optionally searched for defaults
#  CODA_ROOT_DIR:            Base directory where all CODA components are found
#
# The following are set after configuration is done:
#  CODA_FOUND
#  CODA_INCLUDE_DIRS
#  CODA_LIBRARIES
#  CODA_LIBRARYRARY_DIRS
include(FindPackageHandleStandardArgs)

set(CODA_ROOT_DIR "" CACHE PATH "Folder contains coda")

find_path(CODA_INCLUDE_DIR
    NAMES coda/base58.h coda/coda.h coda/error.h coda/string.h
    PATH_SUFFIXES include)

if(MSVC)
    find_library(CODA_LIBRARY_RELEASE
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}coda${CMAKE_STATIC_LIBRARY_SUFFIX} coda
        PATHS ${CODA_ROOT_DIR}
        PATH_SUFFIXES Release)

    find_library(CODA_LIBRARY_DEBUG
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}coda${CMAKE_STATIC_LIBRARY_SUFFIX} coda
        PATHS ${CODA_ROOT_DIR}
        PATH_SUFFIXES Debug)

    set(CODA_LIBRARY optimized ${CODA_LIBRARY_RELEASE} debug ${CODA_LIBRARY_DEBUG})
else()
    find_library(CODA_LIBRARY
        NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}coda${CMAKE_STATIC_LIBRARY_SUFFIX} coda
        PATHS ${CODA_ROOT_DIR}
        PATH_SUFFIXES lib lib64)
endif()

find_package_handle_standard_args(CODA DEFAULT_MSG CODA_INCLUDE_DIR CODA_LIBRARY)

if(CODA_FOUND)
  set(CODA_INCLUDE_DIRS ${CODA_INCLUDE_DIR})
  set(CODA_LIBRARIES ${CODA_LIBRARY})
  message(STATUS "Found coda    (include: ${CODA_INCLUDE_DIR}, library: ${CODA_LIBRARY})")
  mark_as_advanced(CODA_ROOT_DIR CODA_LIBRARY_RELEASE CODA_LIBRARY_DEBUG
                                 CODA_LIBRARY CODA_INCLUDE_DIR)
endif()
