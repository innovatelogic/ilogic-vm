cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0054 OLD)

include(ExternalProject)

#get_filename_component(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/build/cmake" REALPATH)
#include(macros)
#include(${CMAKE_CURRENT_SOURCE_DIR}/Build/cmake/macros.cmake)

include(CheckCXXCompilerFlag)

enable_language(C)
enable_language(CXX)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

SET(CMAKE_CONFIGURATION_TYPES Debug;Release
    CACHE STRING "Semicolon separated list of supported configuration types"
    FORCE)

add_definitions(-DUNICODE -D_UNICODE)

set(COMMON_SDK_DIR ${CMAKE_SOURCE_DIR}/common/SDK)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/build/cmake)

set(PRJOUTDIR ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJOUTDIR})

if (NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX _d)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#set(Boost_DEBUG ON)
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)
#set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/Lib)
#set(BOOST_INCLUDEDIR ${CMAKE_SOURCE_DIR}/common/SDK/)
#set(BOOST_LIBRARYDIR ${CMAKE_SOURCE_DIR}/Lib)

project("${G_CMAKE_TARGET}_${G_GENERATOR_TOKEN}_${G_PROCESSOR_ARCH}") 

if (MSVC)
  #enable full optimization instead of the cmake default /O2
  string( REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" )

 # Enable intrinsic functions, favor speed and string pooling function level linking.
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /Ot /GF /Gy" )

  # Eliminate unreferenced functions and data and enable COMDAT folding
  set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /DEBUG" )
  set( CMAKE_SHARED_LINKER_FLAGS_RELEASE  "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /DEBUG" )
       
  # Fix for errors in Visual Studio 2012
  # "c1xx : fatal error C1027: Inconsistent values for /Ym between creation
  # and use of precompiled header"
  # http://www.ogre3d.org/forums/viewtopic.php?f=2&t=60015
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm282" )

  add_definitions(     	
        -Zi # Always generate debug information.
        -MP # Enable parallel builds
    	-WX # Enable warnings as errors
        /d2Zi+ # Put local variables and inline functions into the PDB
    )
    add_definitions(-D_VARIADIC_MAX=10)
endif()

set( CMAKE_PREFIX_PATH ${G_INSTALL_DIR} )
set( G_BIN_DIR ${CMAKE_INSTALL_PREFIX} )

MESSAGE( STATUS "CMAKE_COMMAND: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:" ${CMAKE_COMMAND} )
message( STATUS "CMAKE_SOURCE_DIR:" ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_TARGET:" ${G_CMAKE_TARGET})
MESSAGE(STATUS "GENERATOR_TOKEN:" ${G_GENERATOR_TOKEN})
MESSAGE(STATUS "PROCESSOR_ARCH:" ${G_PROCESSOR_ARCH})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX:" ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "CMAKE_PREFIX_PATH:" ${CMAKE_PREFIX_PATH})
message( STATUS "G_BIN_DIR: " ${G_BIN_DIR})

set(TARGET_FILENAME target_${G_CMAKE_TARGET}.cmake)

find_file(TARGET_INCLUDE
	${TARGET_FILENAME}
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH)
	
if (TARGET_INCLUDE)
    message( STATUS "Found target include cmake: " ${TARGET_FILENAME} )
    include( ${TARGET_INCLUDE} )
else()
	#message(FATAL_ERROR "No target file:" ${TARGET_FILENAME})
endif()

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")