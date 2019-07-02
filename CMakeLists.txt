cmake_minimum_required(VERSION 3.0)
project(wavespp)

include_directories("src")
file(GLOB WAVESPP_SRC "src/*.cpp" "src/tx/*.cpp")

LINK_DIRECTORIES("/usr/local/lib/waves/")

add_library(wavespp STATIC ${WAVESPP_SRC})
target_link_libraries(wavespp waves_c coda)

add_library(wavespp_shared SHARED ${WAVESPP_SRC})
set_target_properties(wavespp_shared PROPERTIES OUTPUT_NAME wavespp)
target_link_libraries(wavespp_shared waves_c coda)

install(TARGETS wavespp wavespp_shared
        ARCHIVE DESTINATION lib/wavespp
        LIBRARY DESTINATION lib/wavespp
        PUBLIC_HEADER DESTINATION include/wavespp
)
