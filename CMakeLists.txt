cmake_minimum_required(VERSION 3.9)

set(PROJECT_VERSION_STRING 1.0.1.1)
set(PROJECT_VERSION_NUMBER 0x01000101)
project(wavespp
    VERSION ${PROJECT_VERSION_STRING}
    DESCRIPTION "C++ library to work with waves transactions"
    LANGUAGES C CXX)
set(include_out_dir "${CMAKE_CURRENT_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${include_out_dir}")
configure_file("src/version.h.in" "${include_out_dir}/version.h")
project(wavespp VERSION)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local "${CMAKE_SOURCE_DIR}/CMake/modules")

include_directories("src")
file(GLOB WAVESPP_SRC "src/*.cpp" "src/tx/*.cpp")
file(GLOB WAVESPP_HEADERS "src/*.hpp" "src/*.h" "src/*.hpp" "src/tx/*.hpp")
set(WAVESPP_HEADERS ${WAVESPP_HEADERS} "${include_out_dir}/version.h")
include_directories("${include_out_dir}")

find_package(WAVES_C CONFIG REQUIRED)
include_directories(${WAVES_C_INCLUDE_DIRS})

find_package(CODA CONFIG REQUIRED)
include_directories(${CODA_INCLUDE_DIRS})

add_library(wavespp STATIC ${WAVESPP_SRC})
target_link_libraries(wavespp ${WAVES_C_LIBRARIES} ${CODA_LIBRARIES})
set_target_properties(wavespp PROPERTIES PUBLIC_HEADER "${WAVESPP_HEADERS}")

add_library(wavespp_shared SHARED ${WAVESPP_SRC})
set_target_properties(wavespp_shared PROPERTIES
    OUTPUT_NAME wavespp
    PUBLIC_HEADER "${WAVESPP_HEADERS}")
target_link_libraries(wavespp_shared ${WAVES_C_LIBRARIES} ${CODA_LIBRARIES})

install(TARGETS wavespp wavespp_shared
        ARCHIVE DESTINATION lib/wavespp
        LIBRARY DESTINATION lib/wavespp
        PUBLIC_HEADER DESTINATION include/wavespp
)

if(NOT CMAKE_DOC_DIR)
    set(CMAKE_DOC_DIR share/doc/waves)
endif(NOT CMAKE_DOC_DIR)
if(NOT CMAKE_CONFIG_DIR)
    set(CMAKE_CONFIG_DIR etc/waves)
endif(NOT CMAKE_CONFIG_DIR)

install(FILES LICENSE README.md
    DESTINATION "${CMAKE_DOC_DIR}"
    COMPONENT config)

# {{{ CPack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_STRING})
set(CPACK_GENERATOR RPM DEB TGZ ZIP)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "support@wavesplatform.com")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_VENDOR "Waves Platform")

set(CPACK_RPM_PACKAGE_RELOCATABLE ON)
set(CPACK_RPM_PACKAGE_AUTOREQ "no")
set(CPACK_RPM_PACKAGE_AUTOREQPROV "no")
set(CPACK_RPM_PACKAGE_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_RPM_PACKAGE_VERSION "${PROJECT_VERSION_STRING}")
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
set(CPACK_RPM_PACKAGE_LICENSE MIT)
set(CPACK_RPM_PACKAGE_GROUP Development/Tools)
set(CPACK_RPM_PACKAGE_VENDOR "${CPACK_PACKAGE_VENDOR}")

set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
# }}}

# vim: fdm=marker
