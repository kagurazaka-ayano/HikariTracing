cmake_minimum_required(VERSION 3.27)
project(OneWeekend)

set(CMAKE_CXX_STANDARD 23)


file(GLOB SRC "./src/*.cpp")
set(INCLUDE_SELF "./include/OneWeekend/")
set(INCLUDE_THIRDPARTIES "./thirdparties/include")
set(INCLUDE ${INCLUDE_SELF} ${INCLUDE_THIRDPARTIES})

set(LIB_DARWIN_DIR "./thirdparties/lib/Darwin")

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
include_directories(${INCLUDE})
link_directories(${LIB_DARWIN_DIR})
link_libraries(${LIB_DARWIN_DIR}/*.a)
