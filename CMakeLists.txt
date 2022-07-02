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

find_package(assimp REQUIRED)

find_package(GLEW REQUIRED)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -g -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -g -fno-omit-frame-pointer")

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
    assimp
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
