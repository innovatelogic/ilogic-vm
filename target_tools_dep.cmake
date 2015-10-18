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

#FILE(TO_CMAKE_PATH ${CMAKE_BINARY_DIR} CMAKE_BIN_DIR)

set(ENABLE_GTEST ON)

proj_thirdparty_add( gtest SOURCE_DIR utils/gtest-1.7.0)

#SET(DEFAULT_PROJECT_FOLDER default_project)

# INSTALL( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BINARY_DIR}/deploy_deps.cmd )")
#IF(NOT EXISTS ${CMAKE_BIN_DIR}/${DEFAULT_PROJECT_ARC})
#ENDIF()