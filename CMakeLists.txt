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



set(INCLUDE_NORMAL ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})



add_executable(RaytracingNormal ${SRC_NORMAL})
add_executable(RaytracingNormalASCII ${SRC_NORMAL})
target_compile_definitions(RaytracingNormalASCII PRIVATE "ASCII_ART")

target_include_directories(RaytracingNormal PUBLIC ${INCLUDE_NORMAL})
target_include_directories(RaytracingNormalASCII PUBLIC ${INCLUDE_NORMAL})
set_target_properties(
        RaytracingNormal
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/normal/debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/normal/release"
)
