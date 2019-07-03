cmake_minimum_required(VERSION 3.9)

set(PROJECT_VERSION_STRING 1.0.0.5)
set(PROJECT_VERSION_NUMBER 0x01000005)
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
