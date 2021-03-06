project(libkiva)

if(${ENABLE_OPENGL})
  add_definitions(-D ENABLE_OPENGL )
endif()

set(PROCESSOR_ARCHITECTURE "x86_64" CACHE STRING "Set architecture used for version string.")

# Set a default build type if none was specified
if(NOT PROCESSOR_ARCHITECTURE)
  message(STATUS "Setting processor architecture to 'x86_64' as none was specified.")
  set(PROCESSOR_ARCHITECTURE x86_64 CACHE STRING "Choose the processor architecture." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE PROCESSOR_ARCHITECTURE PROPERTY STRINGS "x86_64" "x86")
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(build_architecture macosx-${CMAKE_CXX_COMPILER_ID}-${PROCESSOR_ARCHITECTURE})
else()
  set(build_architecture ${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER_ID}-${PROCESSOR_ARCHITECTURE})
endif()

string(TOLOWER ${build_architecture} build_architecture)

execute_process(COMMAND git describe --tags --long
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                RESULT_VARIABLE git_exit_status
                OUTPUT_VARIABLE kiva_version
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set (kiva_version "${kiva_version}-Debug")
endif()

if(${git_exit_status} MATCHES "0")
  set(VERSION_NUMBER "${kiva_version} (${build_architecture})")
  message("Building Kiva version: ${VERSION_NUMBER}")
else()
  set(VERSION_NUMBER "v?.?.?")
  message("Cannot determine version number.")
endif()

string(TIMESTAMP YEAR %Y)

configure_file (
    "${PROJECT_SOURCE_DIR}/Version.hpp.in"
    "${PROJECT_BINARY_DIR}/Version.hpp"
)

set(kiva_src Algorithms.cpp
             Algorithms.hpp
             BoundaryConditions.hpp
             Domain.cpp
             Domain.hpp
             Foundation.cpp
             Foundation.hpp
             Functions.cpp
             Functions.hpp
             Geometry.cpp
             Geometry.hpp
             Ground.cpp
             Ground.hpp
             GroundOutput.hpp
             Mesher.cpp
             Mesher.hpp
             Version.hpp )

if (${ENABLE_OPENGL})
  set(kiva_src ${kiva_src} PixelCounter.cpp PixelCounter.hpp )
endif ()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libkiva SHARED ${kiva_src})
set_target_properties(libkiva PROPERTIES OUTPUT_NAME kiva)

include(GenerateExportHeader)
generate_export_header(libkiva)

set(kiva_link_flags "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  if(${ENABLE_OPENGL})
    set(kiva_link_flags "${kiva_link_flags} -framework OpenGL -framework Cocoa -framework IOKit" )
  endif()
endif()

if(${ENABLE_OPENMP})
  set(kiva_link_flags "${kiva_link_flags} -fopenmp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${kiva_link_flags}")

set(links lis)

if(${ENABLE_OPENGL})
  set(links ${links} GLEW glfw3 )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(${ENABLE_OPENGL})
    set(links ${links} pthread
                       Xrandr
                       Xxf86vm
                       Xi
                       X11
                       GL )
  endif()
endif()

target_link_libraries(libkiva ${links})
