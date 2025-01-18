# GNU LESSER GENERAL PUBLIC LICENSE
# Version 3, 29 June 2007
#
# Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
#
# Everyone is permitted to copy and distribute verbatim copies of this license
# document, but changing it is not allowed.
#
# This version of the GNU Lesser General Public License incorporates the terms and
# conditions of version 3 of the GNU General Public License, supplemented by the
# additional permissions listed below.

cmake_minimum_required(VERSION 3.10)

## Package information.
set(PACKAGE_NAME   "mpaheaderinfo")

# Build without ATL support.
option(SA_MPA_BUILD_WITHOUT_ATL "Build as library, without Windows ATL dependencies." ON)

# Use Address Sanitizer.
option(SA_MPA_ENABLE_ASAN "Build with Address Sanitizer." OFF)

# Compiler id for Apple Clang is now AppleClang.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()

# Setup root directory of the ${PACKAGE_NAME} project.
set(SA_MPA_ROOT_DIR "${CMAKE_SOURCE_DIR}" CACHE STRING
  "Directory root of the ${PACKAGE_NAME} project.")

# C++ standard can be overridden when this is used as a sub-project.
if(NOT CMAKE_CXX_STANDARD)
  # This project requires C++17.
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

# Git is required to build software version via Git.
find_package(Git)

if (NOT DEFINED PACKAGE_VERSION)
  if (NOT Git_FOUND)
    message(FATAL "[common]: Unable to extract Git software version - no Git.")
  endif()

  message(STATUS "[common]: Extracting Git software version.")

  if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    execute_process(COMMAND cmd /c "git rev-list --branches HEAD | find /v " " /c" OUTPUT_VARIABLE NVTS_GIT_VERSION WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  else()
    execute_process(COMMAND bash "-c" "git rev-list --branches HEAD | wc -l | tr -d ' ' | tr -d '\n'" OUTPUT_VARIABLE NVTS_GIT_VERSION WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  set(PACKAGE_VER_PATCH 0)
  math(EXPR PACKAGE_VER_TWEAK ${NVTS_GIT_VERSION})
  message(STATUS
    "[common]: Build version from Git (patch.tweak): ${PACKAGE_VER_PATCH}.${PACKAGE_VER_TWEAK}.")

  set(PACKAGE_VER_MAJOR 2)
  set(PACKAGE_VER_MINOR 4)

  set(PACKAGE_VERSION
    "${PACKAGE_VER_MAJOR}.${PACKAGE_VER_MINOR}.${PACKAGE_VER_PATCH}.${PACKAGE_VER_TWEAK}")
endif()
set(PACKAGE_STRING     "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME    "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT  "https://github.com/Source-Authors/mpaheaderInfo/issues")

# Default build type is Release unless overridden.
# On Windows default is Debug, so need to unify.
# See https://discourse.cmake.org/t/how-to-deal-with-ninja-setting-cmake-build-type-to-debug/281/3
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build.")
set(SA_MPA_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "CMake build type used to build ${PACKAGE_NAME}.")

project(${PACKAGE_NAME}
  VERSION ${PACKAGE_VER_MAJOR}.${PACKAGE_VER_MINOR}.${PACKAGE_VER_PATCH}.${PACKAGE_VER_TWEAK}
  LANGUAGES CXX)

message(STATUS "[common]: CMake version: ${CMAKE_VERSION}.")
message(STATUS "[common]: ${PACKAGE_NAME} version: ${PACKAGE_VERSION}.")

## Common variables.

set(SA_MPA_BINARY_DIR "${CMAKE_BINARY_DIR}" CACHE STRING
  "Directory of the ${PACKAGE_NAME} binaries output.")
set(SA_MPA_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}" CACHE STRING
  "Name of the ${PACKAGE_NAME} build system.")

if(${SA_MPA_SYSTEM_NAME} STREQUAL "Darwin")
  set(SA_MPA_OS_MACOS ON CACHE BOOL "MacOS is build system.")
else()
  set(SA_MPA_OS_MACOS OFF CACHE BOOL "MacOS is NOT build system.")
endif()

if(${SA_MPA_SYSTEM_NAME} STREQUAL "Linux")
  set(SA_MPA_OS_LINUX ON CACHE BOOL "Linux distro is build system.")
else()
  set(SA_MPA_OS_LINUX OFF CACHE BOOL "Linux distro is NOT build system.")
endif()

if(${SA_MPA_SYSTEM_NAME} STREQUAL "Windows")
  set(SA_MPA_OS_WIN ON CACHE BOOL "Windows is build system.")
else()
  set(SA_MPA_OS_WIN OFF CACHE BOOL "Windows is NOT build system.")
endif()

if(SA_MPA_OS_MACOS OR SA_MPA_OS_LINUX)
  set(SA_MPA_OS_POSIX ON CACHE BOOL "POSIX compatible os is build system.")
else()
  set(SA_MPA_OS_POSIX OFF CACHE BOOL "POSIX compatible os is NOT build system.")
endif()

set(SA_MPA_PRODUCT_NAME "${PACKAGE_NAME}" CACHE STRING
  "Name of the ${PACKAGE_NAME} product.")

set(SA_MPA_CMAKE_GENERATOR "${CMAKE_GENERATOR}" CACHE STRING
  "CMake generator used to generate build tree." FORCE)
set(SA_MPA_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING
  "Compiler used to build ${PACKAGE_NAME}.")
set(SA_MPA_CXX_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}" CACHE STRING
  "Compiler id used to build ${PACKAGE_NAME}.")
set(SA_MPA_CXX_COMPILER_FRONTEND_VARIANT "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}"
  CACHE STRING "Compiler frontend used to build ${PACKAGE_NAME}.")
set(SA_MPA_EXPORT_COMPILE_COMMANDS ${CMAKE_EXPORT_COMPILE_COMMANDS} CACHE BOOL
  "Enable/Disable output of compile commands during generation.")

if(SA_MPA_CXX_COMPILER_ID MATCHES "Clang")
  set(SA_MPA_COMPILER_CLANG ON CACHE BOOL "Compiler is Clang.")
else()
  set(SA_MPA_COMPILER_CLANG OFF CACHE BOOL "Compiler is NOT Clang.")
endif()

if(SA_MPA_CXX_COMPILER_ID STREQUAL "GNU")
  set(SA_MPA_COMPILER_GCC ON CACHE BOOL "Compiler is GCC.")
else()
  set(SA_MPA_COMPILER_GCC OFF CACHE BOOL "Compiler is NOT GCC.")
endif()

if(SA_MPA_CXX_COMPILER_ID STREQUAL "MSVC")
  set(SA_MPA_COMPILER_MSVC ON CACHE BOOL "Compiler is MSVC.")
else()
  set(SA_MPA_COMPILER_MSVC OFF CACHE BOOL "Compiler is NOT MSVC.")
endif()

## Common dependencies.

if(Git_FOUND)
  message(STATUS
    "[common]: Git version ${GIT_VERSION_STRING} found at '${GIT_EXECUTABLE}'.")

  # Get the current working branch.
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${SA_MPA_ROOT_DIR}
    OUTPUT_VARIABLE SA_MPA_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Get the latest commit hash.
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
    WORKING_DIRECTORY ${SA_MPA_ROOT_DIR}
    OUTPUT_VARIABLE SA_MPA_GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # Get the subject of the commit.
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" log -1 --format=%s
    WORKING_DIRECTORY "${SA_MPA_ROOT_DIR}"
    OUTPUT_VARIABLE SA_MPA_GIT_COMMIT_SUBJECT
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS
    "[common]: Git using branch '${SA_MPA_GIT_BRANCH}', "
    "commit ${SA_MPA_GIT_COMMIT_HASH} | ${SA_MPA_GIT_COMMIT_SUBJECT}.")
endif()

message(STATUS
  "[common]: ${PACKAGE_NAME} using CMake generator: ${SA_MPA_CMAKE_GENERATOR}.")

# Select correct threading library.
if(NOT SA_MPA_OS_WIN)
  set(CMAKE_THREAD_PREFER_PTHREAD ON)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()
find_package(Threads REQUIRED)
list(APPEND CMAKE_REQUIRED_LIBRARIES Threads::Threads)
if(NOT CMAKE_THREAD_LIBS_INIT)
  set(SA_MPA_THREADS_LIBRARY "system default" CACHE STRING
    "Threads library used to build ${PACKAGE_NAME}.")
else()
  set(SA_MPA_THREADS_LIBRARY "${CMAKE_THREAD_LIBS_INIT}" CACHE STRING
    "Threads library used to build ${PACKAGE_NAME}.")
endif()
message(STATUS
  "[common]: ${PACKAGE_NAME} using threading library: ${SA_MPA_THREADS_LIBRARY}.")

if(XCODE_VERSION)
  message(STATUS
    "[common]: ${PACKAGE_NAME} using XCode version: ${XCODE_VERSION}.")
endif()

message(STATUS "[common]: ${PACKAGE_NAME} cxx compiler: ${SA_MPA_CXX_COMPILER}.")
message(STATUS "[common]: ${PACKAGE_NAME} build type: ${SA_MPA_BUILD_TYPE}.")

if(NOT SA_MPA_BUILD_WITHOUT_ATL)
  set(SRC_PRODUCT_NAME "MPEG Audio Info")

  # Generate common product version info.
  configure_file(
    ${SA_MPA_ROOT_DIR}/build/product_version_config.h.cmake.in
    ${SA_MPA_BINARY_DIR}/build/product_version_config.h
  )

  # Generate app version info.
  configure_file(
    ${SA_MPA_ROOT_DIR}/build/app_version_config.h.cmake.in
    ${SA_MPA_BINARY_DIR}/build/app_version_config.h
  )

  # Generate visual styles manifest info.
  configure_file(
    ${SA_MPA_ROOT_DIR}/build/win/manifests/enable-visual-styles.manifest.cmake.in
    ${SA_MPA_BINARY_DIR}/manifests/enable-visual-styles.manifest
  )
endif ()

# https://github.com/izenecloud/cmake/blob/master/SetCompilerWarningAll.cmake
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Use the highest warning level for Visual Studio.
  set(CMAKE_CXX_WARNING_LEVEL 4)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")

  if(SA_MPA_ENABLE_ASAN)
    # Use AddressSanitizer
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
  endif()

  # Use static runtime.
  foreach(flag_var
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif()
  endforeach()
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Use -Wall for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")

  # Use -Wextra for clang and gcc.
  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")

  # Use -Werror.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

  if(SA_MPA_ENABLE_ASAN)
    # Use AddressSanitizer
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

if(SA_MPA_BUILD_WITHOUT_ATL)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable RTTI.
    string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
  else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable RTTI.
    string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

  add_library(${PACKAGE_NAME} "")
else()
  add_executable(${PACKAGE_NAME} WIN32 "")
endif(SA_MPA_BUILD_WITHOUT_ATL)

target_sources(${PACKAGE_NAME}
  PRIVATE
    stdafx.h
    APETag.cpp
    ID3V1Tag.cpp
    ID3V2Tag.cpp
    LameTag.cpp
    Lyrics3Tag.cpp
    MPAEndOfFileException.cpp
    MPAException.cpp
    MPAFile.cpp
    MPAFileStream.cpp
    MPAFrame.cpp
    MPAHeader.cpp
    MPAStream.cpp
    MusicMatchTag.cpp
    Platform.cpp
    stdafx.cpp
    Tag.cpp
    Tags.cpp
    VBRHeader.cpp
    VBRIHeader.cpp
    XINGHeader.cpp

  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/APETag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/ID3V1Tag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/ID3V2Tag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/LameTag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Lyrics3Tag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAEndOfFileException.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAException.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAFile.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAFileStream.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAFrame.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAHeader.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MPAStream.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/MusicMatchTag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Platform.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Tag.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Tags.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/VBRHeader.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/VBRIHeader.h>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/XINGHeader.h>
)

if(NOT SA_MPA_BUILD_WITHOUT_ATL)
  target_sources(${PACKAGE_NAME}
    PRIVATE
      AboutDlg.cpp
      AboutDlg.h
      base_dlg.cpp
      base_dlg.h
      dpi_wnd_behavior.cpp
      dpi_wnd_behavior.h
      MPEGAudioInfo.cpp
      MPEGAudioInfo.h
      MPEGAudioInfo.rc
      MPEGAudioInfoDlg.cpp
      MPEGAudioInfoDlg.h
      resource.h
      resources/app.ico
      ${SA_MPA_BINARY_DIR}/manifests/enable-visual-styles.manifest
      ${SA_MPA_ROOT_DIR}/build/win/manifests/dpi-aware.manifest
      ${SA_MPA_ROOT_DIR}/build/win/manifests/heap-type.manifest
      ${SA_MPA_ROOT_DIR}/build/win/manifests/supported-os.manifest
      ${SA_MPA_ROOT_DIR}/build/win/manifests/ultra-high-scroll-resolution.manifest
      ${SA_MPA_ROOT_DIR}/build/win/manifests/utf-8-code-page.manifest
      ${SA_MPA_ROOT_DIR}/build/win/resource_scripts/windows_app_base.rc
  )

  target_include_directories(${PACKAGE_NAME}
    PRIVATE
      ${PROJECT_SOURCE_DIR}
      ${SA_MPA_BINARY_DIR}/build
  )

  target_compile_definitions(${PACKAGE_NAME}
    PRIVATE
      SRC_PRODUCT_INTERNAL_NAME_STRING="${PACKAGE_NAME}"
      SRC_PRODUCT_ORIGINAL_NAME_STRING="${PACKAGE_NAME}.exe"
  )
endif(NOT SA_MPA_BUILD_WITHOUT_ATL)

target_include_directories(${PACKAGE_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)

if(SA_MPA_BUILD_WITHOUT_ATL)
  target_compile_definitions(${PACKAGE_NAME}
    PRIVATE
      -DSA_MPA_BUILD_WITHOUT_ATL=1
  )
endif(SA_MPA_BUILD_WITHOUT_ATL)

if(SA_MPA_OS_WIN)
  target_compile_definitions(${PACKAGE_NAME}
    PRIVATE
      _CRT_SECURE_NO_WARNINGS=1
  )
endif()

target_compile_features(${PACKAGE_NAME}
  PRIVATE
    cxx_std_17
)
set_target_properties(${PACKAGE_NAME}
  PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
