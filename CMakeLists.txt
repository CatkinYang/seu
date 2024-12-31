cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ##############################################################################
set(PROJECT_NAME "SEU")
project(${PROJECT_NAME})
message(STATUS "PROJECT_NAME: " ${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to `Debug` as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# ##############################################################################

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt"
     PATH_TO_CMAKELISTS_TXT)

if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
  message(
    FATAL_ERROR
      "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake ..\" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those."
  )
endif()

# ##############################################################################

set(SEU_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(SEU_CLANG_SEARCH_PATH
    "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin"
    "/usr/local/opt/llvm@14/bin" "/opt/homebrew/opt/llvm@14/bin/")

# ##############################################################################
# CTest
enable_testing()

# ##############################################################################
# COMPILER SETUP
# ##############################################################################
if(NOT DEFINED SEU_SANITIZER)
  set(SEU_SANITIZER address)
endif()

message("Build mode: ${CMAKE_BUILD_TYPE}")
message("${SEU_SANITIZER} sanitizer will be enabled in debug mode.")

# Compiler flags. set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall
# -Wextra -Werror") set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}
# -Wno-unused-parameter -Wno-attributes")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb
# -fsanitize=${SEU_SANITIZER} -fno-omit-frame-pointer
# -fno-optimize-sibling-calls" )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG}  -Wno-unused-parameter -Wno-attributes")

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=${SEU_SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls"
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# Output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(SEU_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(SEU_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
set(SEU_THIRD_PARTY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party)

include_directories("/opt/homebrew/include/eigen3")

include_directories(${SEU_SRC_INCLUDE_DIR} ${SEU_TEST_INCLUDE_DIR}
                    ${SEU_THIRD_PARTY_INCLUDE_DIR})
include_directories(BEFORE src) # This is needed for gtest.

function(disable_target_warnings NAME)
  target_compile_options(${NAME} PRIVATE "-w")
endfunction()

set(CMAKE_MODULE_PATH "${SEU_BUILD_SUPPORT_DIR}/cmake;${CMAKE_MODULE_PATH}")
find_package(yaml-cpp REQUIRED)
find_package(GUROBI)

include_directories(${GUROBI_INCLUDE_DIRS})
include_directories(${YAML_CPP_INCLUDE_DIR})

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
