# CMake entry point
cmake_minimum_required (VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)
project (Hexagon)

IF( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
ENDIF()
IF( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
ENDIF()
IF( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
ENDIF()

set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} Debug Release RelWithDebInfo MinSizeRel)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES} )
set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel" FORCE)


IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
  set(CMAKE_C_FLAGS                  "${CMAKE_C_FLAGS}                  -Wall -Wextra")
  set(CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG}            -O0 -g3 -pg -fsanitize=address -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE}          -O3 -Werror -DNDEBUG")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO}   -O2 -Werror -g3")
  set(CMAKE_C_FLAGS_MINSIZEREL       "${CMAKE_C_FLAGS_MINSIZEREL}       -Os -Werror -DNDEBUG")

  set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS}                -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG}          -O0 -g3 -pg -fsanitize=address -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE}        -O3 -Werror -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -Werror -g3")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL}     -Os -Werror -DNDEBUG")

  set(CMAKE_LD_FLAGS_DEBUG           "${CMAKE_LD_FLAGS_DEBUG}           -pg -fsanitize=address -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_LD_FLAGS_RELEASE         "${CMAKE_LD_FLAGS_RELEASE}         -fdata-sections -ffunction-sections -Wl,--gc-sections")
  set(CMAKE_LD_FLAGS_RELWITHDEBINFO  "${CMAKE_LD_FLAGS_RELWITHDEBINFO}  -fdata-sections -ffunction-sections -Wl,--gc-sections")
  set(CMAKE_LD_FLAGS_MINSIZEREL      "${CMAKE_LD_FLAGS_MINSIZEREL}      -fdata-sections -ffunction-sections -Wl,--gc-sections")
ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)

set( SOURCES
  src/Hexagon/libhex.cpp
  src/Hexagon/libhex.hpp
  src/main.cpp
  src/Shader.cpp
  src/Shader.hpp
)

# https://cmake.org/cmake/help/v3.0/module/FindOpenGL.html
find_package(OpenGL REQUIRED)
# https://cmake.org/cmake/help/v3.0/module/FindGLEW.html
find_package(GLEW REQUIRED)
# http://www.glfw.org/docs/3.0/build.html#build_link_cmake_pkgconfig
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories( ${PROJECT_SOURCE_DIR}/inc ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD   11)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD_REQUIRED   ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

