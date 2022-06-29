cmake_minimum_required(VERSION 3.0.0)
project(opengl-tp VERSION 0.1.0)

include(CTest)
enable_testing()

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

if(${glfw3_FOUND})
    message(STATUS "Found GLFW3")
else()
    message(FATAL_ERROR "GLFW3 not found")
endif()

find_package(GLEW REQUIRED)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO: find a better way to handle release/debug builds
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -g -fno-omit-frame-pointer -fsanitize=address")

include_directories("${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE all_SRCS CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/src/*.hxx"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(opengl-tp ${all_SRCS})

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(
    opengl-tp
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    glfw
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
