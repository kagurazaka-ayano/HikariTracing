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
        metal
        GIT_REPOSITORY https://github.com/bkaradzic/metal-cpp.git
        GIT_TAG metal-cpp_macOS14.2_iOS17.2
)
FetchContent_MakeAvailable(metal)
set(METAL ${metal_SOURCE_DIR}/SingleHeader)
list(APPEND METAL "include/metal")
FetchContent_Declare(
        Math
        GIT_REPOSITORY https://github.com/kagurazaka-ayano/AppleMath.git
        GIT_TAG main
)
FetchContent_MakeAvailable(Math)
list(APPEND INCLUDE_SELF ${Math_SOURCE_DIR}/include)
link_libraries(Math)

set(INCLUDE_NORMAL ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})
set(INCLUDE_METAL ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES} ${METAL})


add_executable(RaytracingNormal ${SRC_NORMAL})
add_executable(RaytracingMetal ${SRC_METAL} ${SRC_NORMAL_NO_MAIN})

target_include_directories(RaytracingNormal PUBLIC ${INCLUDE_NORMAL})
target_include_directories(RaytracingMetal PUBLIC ${INCLUDE_METAL})
target_link_libraries(RaytracingMetal
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore"
        )
set_target_properties(
        RaytracingMetal
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/metal/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/metal/release"
)
set_target_properties(
        RaytracingNormal
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/normal/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/normal/release"
)

