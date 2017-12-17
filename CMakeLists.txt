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
add_executable(get_time_cli.cpp ${PROJECT_SOURCE_DIR}/get_time_cli.cpp)
add_executable(send_time_ser.cpp ${PROJECT_SOURCE_DIR}/send_time_ser.cpp)