cmake_minimum_required(VERSION 3.27)
project(Raytracing)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC "./src/*.cpp")
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
list(APPEND INCLUDE_THIRDPARTIES ${KawaiiMQ_SOURCE_DIR}/include)
link_libraries(KawaiiMQ)

set(INCLUDE ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})

include_directories(${INCLUDE})

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
