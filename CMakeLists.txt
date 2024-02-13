cmake_minimum_required(VERSION 3.27)
project(Raytracing)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC_NORMAL "./src/*.cpp")
file(GLOB SRC_METAL "./src/metal/*.cpp")
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
# metal, only for metal version
FetchContent_Declare(
        metal
        GIT_REPOSITORY https://github.com/bkaradzic/metal-cpp.git
        GIT_TAG metal-cpp_macOS14.2_iOS17.2
)
FetchContent_MakeAvailable(metal)
set(METAL ${metal_SOURCE_DIR})
list(APPEND METAL include/metal)


set(INCLUDE_NORMAL ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})
set(INCLUDE_METAL ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES} ${METAL})


add_executable(RaytracingNormal ${SRC_NORMAL})
add_executable(RaytracingNormalASCII ${SRC_NORMAL})
target_compile_definitions(RaytracingNormalASCII PRIVATE "ASCII_ART")
add_executable(RaytracingMetal ${SRC_METAL})

target_include_directories(RaytracingNormal PUBLIC ${INCLUDE_NORMAL})
target_include_directories(RaytracingNormalASCII PUBLIC ${INCLUDE_NORMAL})
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
