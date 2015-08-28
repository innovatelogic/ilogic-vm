cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0054 OLD)

#include(${CMAKE_CURRENT_SOURCE_DIR}/Build/cmake/macros.cmake)

#---------------------------------------------------------------------------
macro(SET_BIN_OUTPUT_FOLDER proj out)
set_target_properties( ${PROJ} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUT}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT}
					)
endmacro()

#---------------------------------------------------------------------------

add_definitions(-DUNICODE -D_UNICODE)

MESSAGE( STATUS "CMAKE_COMMAND: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:" ${CMAKE_COMMAND} )
#message(STATUS "EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})

#set(MyFoo "MyFoo's value")
#debugValue(MyFoo)

set(COMMON_SDK_DIR ${CMAKE_SOURCE_DIR}/common/SDK)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/build/cmake)

set(PRJOUTDIR ${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRJOUTDIR})

set(CMAKE_DEBUG_POSTFIX _d)

#set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/Lib)
set(BOOST_INCLUDEDIR ${CMAKE_SOURCE_DIR}/common/SDK/)
set(BOOST_LIBRARYDIR ${CMAKE_SOURCE_DIR}/Lib)

#variable_watch(Boost_USE_STATIC_LIBS)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("${G_CMAKE_TARGET}_${G_GENERATOR_TOKEN}_${G_PROCESSOR_ARCH}") 

MESSAGE(STATUS "CMAKE_TARGET:" ${G_CMAKE_TARGET})
MESSAGE(STATUS "GENERATOR_TOKEN:" ${G_GENERATOR_TOKEN})
MESSAGE(STATUS "PROCESSOR_ARCH:" ${G_PROCESSOR_ARCH})
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX:" ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "CMAKE_PREFIX_PATH:" ${CMAKE_PREFIX_PATH})
MESSAGE(STATUS "CMAKE_SOURCE_DIR:" ${CMAKE_SOURCE_DIR})

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

add_subdirectory(utils/gtest-1.7.0)
add_subdirectory(common/TinyXML)
add_subdirectory(Foundation)
add_subdirectory(D3DDrv)
add_subdirectory(RenderSDK)
add_subdirectory(CoreSDK)
add_subdirectory(Launcher)
#add_subdirectory(MaxExporter)