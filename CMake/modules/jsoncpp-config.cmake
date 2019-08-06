# Searches for JSONCPP library.
#
# The following variables are optionally searched for defaults
#  JSONCPP_ROOT_DIR: Base directory where all JSONCPP components are found
#
# The following are set after configuration is done:
#  JSONCPP_FOUND
#  JSONCPP_INCLUDE_DIRS
#  JSONCPP_LIBRARIES
#  JSONCPP_LIBRARYRARY_DIRS
include(FindPackageHandleStandardArgs)

set(JSONCPP_ROOT_DIR "" CACHE PATH "Folder contains jsoncpp")

find_path(JSONCPP_INCLUDE_DIR
    NAMES jsoncpp/json/json.h
    PATH_SUFFIXES include)

if(MSVC)
    find_library(JSONCPP_LIBRARY_RELEASE jsoncpp
        PATHS ${JSONCPP_ROOT_DIR}
        PATH_SUFFIXES Release)

    find_library(JSONCPP_LIBRARY_DEBUG jsoncpp
        PATHS ${JSONCPP_ROOT_DIR}
        PATH_SUFFIXES Debug)

    set(JSONCPP_LIBRARY optimized ${JSONCPP_LIBRARY_RELEASE} debug ${JSONCPP_LIBRARY_DEBUG})
else()
    find_library(JSONCPP_LIBRARY jsoncpp
        PATHS ${JSONCPP_ROOT_DIR}
        PATH_SUFFIXES lib lib64)
endif()

find_package_handle_standard_args(JSONCPP DEFAULT_MSG JSONCPP_INCLUDE_DIR JSONCPP_LIBRARY)

if(JSONCPP_FOUND)
  set(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})
  set(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
  message(STATUS "Found jsoncpp    (include: ${JSONCPP_INCLUDE_DIR}, library: ${JSONCPP_LIBRARY})")
  mark_as_advanced(JSONCPP_ROOT_DIR JSONCPP_LIBRARY_RELEASE JSONCPP_LIBRARY_DEBUG
      JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)
endif()
