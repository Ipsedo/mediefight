cmake_minimum_required(VERSION 3.7)
project(mediefight C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(./glm ./src ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")

add_executable(mediefight ${SOURCE_FILES} src/drawable/objmtlvbo.cpp src/drawable/objmtlvbo.h src/utils/string_utils.h src/utils/string_utils.cpp)