#设置cmake 版本要求
cmake_minimum_required(VERSION 3.5.1)

# 项目名称
PROJECT(webtcp)

# 版本号
set(WEBTCP_VERSION_MAJOR 0)
set(WEBTCP_VERSION_MINOR 1)


# 配置文件引用cmake变量
configure_file(
        "${PROJECT_SOURCE_DIR}/webtcp.h.in"
        "${PROJECT_SOURCE_DIR}/webtcp.h"
)


# 设置源文件和输出目录
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


# 添加源代码目录
include_directories("${PROJECT_SOURCE_DIR}")


# 设置一些编译选项
#option()

set(SOURCE_FILES get_time_cli.cpp)

# 生成可执行文件
add_executable(get_time_cli ${PROJECT_SOURCE_DIR}/get_time_cli.cpp)
add_executable(send_time_ser ${PROJECT_SOURCE_DIR}/send_time_ser.cpp)
add_executable(str_echo_ser ${PROJECT_SOURCE_DIR}/str_echo_ser.cpp)
add_executable(str_echo_cli ${PROJECT_SOURCE_DIR}/str_echo_cli.cpp)
add_executable(select_ser ${PROJECT_SOURCE_DIR}/select_ser.cpp)
add_executable(poll_ser ${PROJECT_SOURCE_DIR}/poll_ser.cpp)
add_executable(dg_echo_ser ${PROJECT_SOURCE_DIR}/dg_echo_ser.cpp)
add_executable(sctp_ser ${PROJECT_SOURCE_DIR}/sctp_ser.cpp )
add_executable(sctp_cli ${PROJECT_SOURCE_DIR}/sctp_cli.cpp )
add_executable(dg_cli ${PROJECT_SOURCE_DIR}/dg_cli.cpp)
add_executable(un_local_ser ${PROJECT_SOURCE_DIR}/un_local_ser.cpp)
add_executable(un_local_cli ${PROJECT_SOURCE_DIR}/un_local_cli.cpp)
add_executable(sig ${PROJECT_SOURCE_DIR}/sig.cpp)
add_executable(sig_cli ${PROJECT_SOURCE_DIR}/sig_cli.cpp ${PROJECT_SOURCE_DIR}/sig.cpp)
add_executable(sig_ser ${PROJECT_SOURCE_DIR}/sig_ser.cpp ${PROJECT_SOURCE_DIR}/sig.cpp)

include_directories("${PROJECT_SOURCE_DIR}/sctp_lib")
add_subdirectory (sctp_lib)
set (EXTRA_LIBS ${EXTRA_LIBS} sctp_lib)

target_link_libraries(sctp_ser ${EXTRA_LIBS})
target_link_libraries(sctp_cli ${EXTRA_LIBS})