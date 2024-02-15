cmake_minimum_required(VERSION 3.27)
project(Raytracing)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC_NORMAL "./src/*.cpp")
file(GLOB SRC_NORMAL_NO_MAIN "./src/*.cpp")
list(FILTER SRC_NORMAL_NO_MAIN EXCLUDE REGEX ".*main.cpp")
file(GLOB SRC_METAL "./src/metal/*.cpp")
list(APPEND SRC_METAL ${SRC_NORMAL_NO_MAIN})
set(INCLUDE_SELF "./include/Raytracing/")

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)
list(APPEND INCLUDE_THIRDPARTIES ${spdlog_SOURCE_DIR}/include)
FetchContent_Declare(
        KawaiiMQ
        GIT_REPOSITORY https://github.com/kagurazaka-ayano/KawaiiMq.git
        GIT_TAG main
)
FetchContent_MakeAvailable(KawaiiMQ)
list(APPEND INCLUDE_SELF ${KawaiiMQ_SOURCE_DIR}/include)
link_libraries(KawaiiMQ)
FetchContent_Declare(
        math
        GIT_REPOSITORY https://github.com/kagurazaka-ayano/AppleMath.git
        GIT_TAG main
)
FetchContent_MakeAvailable(math)
list(APPEND INCLUDE_SELF ${math_SOURCE_DIR}/include)

set(INCLUDE ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})
include_directories(${INCLUDE})
add_executable(RaytracingNormal ${SRC_NORMAL})
add_executable(RaytracingAscii ${SRC_NORMAL})
target_compile_definitions(RaytracingAscii PUBLIC "ASCII_ART")

set_target_properties(
        RaytracingNormal
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/normal/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/normal/release"
)
set_target_properties(
        RaytracingAscii
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/normal/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/normal/release"
)

