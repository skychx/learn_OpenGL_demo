# cmake的版本号
cmake_minimum_required(VERSION 3.17)
# project 的名字
project(learn_OpenGL)
# C++ version
set(CMAKE_CXX_STANDARD 14)

# 检查环境变量
if (NOT DEFINED ENV{GLFW_HOME})
    message(FATAL_ERROR "found no env named GLFW_HOME")
endif()
if (NOT DEFINED ENV{GLAD_HOME})
    message(FATAL_ERROR "found no env named GLAD_HOME")
endif()
if (NOT DEFINED ENV{GLM_HOME})
    message(FATAL_ERROR "found no env named GLM_HOME")
endif()
if (NOT DEFINED ENV{ASSIMP_HOME})
    message(FATAL_ERROR "found no env named ASSIMP_HOME ")
endif()

# 暂存环境变量
set(GLFW_HOME $ENV{GLFW_HOME})
set(GLAD_HOME $ENV{GLAD_HOME})
set(GLM_HOME $ENV{GLM_HOME})
set(ASSIMP_HOME $ENV{ASSIMP_HOME})

# 设置头文件目录
include_directories("${GLFW_HOME}/include")
include_directories("${GLAD_HOME}/include")
include_directories("${GLM_HOME}/include")
include_directories("${ASSIMP_HOME}/include")

# 添加 GLFW3 预编译库
add_library(glfw SHARED IMPORTED)
SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib/libglfw.3.3.dylib")

add_library(assimp SHARED IMPORTED)
SET_TARGET_PROPERTIES(assimp PROPERTIES IMPORTED_LOCATION "${ASSIMP_HOME}/lib/libassimp.dylib")

# 编译 GLAD 库
add_library(glad SHARED "${GLAD_HOME}/src/glad.c")

# 创建可执行文件
add_executable(learn_OpenGL main.cpp shader_s.h camera.h mesh.h shader.h model.h shader_m.h stb_image.cpp)

# # 链接 GLFW GLAD OpenGL
target_link_libraries(learn_OpenGL glfw glad assimp "-framework OpenGL")