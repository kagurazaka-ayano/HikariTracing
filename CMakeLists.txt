cmake_minimum_required(VERSION 3.27)
project(OneWeekend)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SRC "./src/*.cpp")
set(INCLUDE_SELF "./include/OneWeekend/")

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
)
FetchContent_MakeAvailable(spdlog)
list(APPEND INCLUDE_THIRDPARTIES ${spdlog_SOURCE_DIR}/include)
FetchContent_Declare(
        KawaiiMQ
        URL https://github.com/kagurazaka-ayano/KawaiiMq/releases/latest/download/KawaiiMQ-src.zip
)
FetchContent_MakeAvailable(KawaiiMQ)
list(APPEND INCLUDE_THIRDPARTIES ${KawaiiMQ_SOURCE_DIR}/include)

set(INCLUDE ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})

include_directories(${INCLUDE})

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
