cmake_minimum_required(VERSION 3.0.0)

#get_filename_component(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Build/cmake" REALPATH)
#include(macros)

include(ExternalProject)
include(CMakeParseArguments)

#---------------------------------------------------------------------------
function( proj_thirdparty_add NAME )
	cmake_parse_arguments( ARGS "" "SOURCE_DIR" "CMAKE_ARGS;DEPENDS" ${ARGN} )
	string(TOUPPER ${NAME} NAMEU)
	ExternalProject_Add( ${NAME}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}
		DOWNLOAD_COMMAND ""
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=${G_INSTALL_DIR}
			#-DCMAKE_MODULE_PATH:PATH=${CMAKE_MODULE_PATH}
			-DG_CMAKE_TARGET=${NAME}
			-DG_CMAKE_TARGET_DIR=${ARGS_SOURCE_DIR}
			-DG_BIN_DIR:PATH=${DG_BIN_DIR}
			#-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
			${ARGS_CMAKE_ARGS}

		DEPENDS ${ARGS_DEPENDS}
		
		#BUILD_COMMAND 	${CMAKE_COMMAND} --build . --config Debug #${CMAKE_BUILD_TYPE}
		#	COMMAND 	${CMAKE_COMMAND} --build . --config Release
		#INSTALL_COMMAND	${CMAKE_COMMAND} --build . --target INSTALL --config Debug
		#	COMMAND 	${CMAKE_COMMAND} --build . --target INSTALL --config Release
		)
	ExternalProject_Add_Step( ${NAME} forceconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "Force configure of ${NAME}"
		DEPENDEES update
		DEPENDERS configure
		ALWAYS 1
		)
endfunction()
#---------------------------------------------------------------------------

# external project download and build (no install for gtest)
ExternalProject_Add(gtest
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils/gtest-1.7.0
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_DEBUG_POSTFIX=_d
               -Dgtest_force_shared_crt=ON
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )

#---------------------------------------------------------------------------
#zlib	
ExternalProject_Add(zlib
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/zlib-master
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../bin
			   #-DCMAKE_DEBUG_POSTFIX=_d
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )	

#---------------------------------------------------------------------------
#glew	
ExternalProject_Add(glew
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/SDK/glew-1.13.0/build/cmake
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../bin
			   #-DCMAKE_DEBUG_POSTFIX=_d
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )

#---------------------------------------------------------------------------
#bullet	
ExternalProject_Add(bullet
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/SDK/bullet/bullet3-2.83.6
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../bin
			   #-DCMAKE_DEBUG_POSTFIX=_d
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )

#---------------------------------------------------------------------------
#tinyxml	
ExternalProject_Add(tinyxml2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/tinyxml2
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_DEBUG_POSTFIX=_d
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )

#---------------------------------------------------------------------------
#freetype2	
ExternalProject_Add(freetype2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common/freetype-2.6.2
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../Libs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../Libs
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${CMAKE_BINARY_DIR}/../bin
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${CMAKE_BINARY_DIR}/../bin
			   #-DCMAKE_DEBUG_POSTFIX=_d
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )
	
# INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BINARY_DIR}/deploy_deps.cmd )")
#IF(NOT EXISTS ${CMAKE_BIN_DIR}/${DEFAULT_PROJECT_ARC})
#ENDIF()