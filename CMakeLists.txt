cmake_minimum_required(VERSION 3.7)
project(mediefight)

set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(./glm ./src ./src/utils ./src/drawable ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

set(SOURCE_FILES src/drawable/modelvbo.h src/drawable/modelvbo.cpp src/utils/shader.h src/utils/shader.cpp src/main.cpp src/level/level.h)

add_executable(mediefight ${SOURCE_FILES})