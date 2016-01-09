cmake_minimum_required(VERSION 3.0.0)

project(Editors)


include(CheckCXXCompilerFlag)

enable_language(C)
enable_language(CXX)

add_definitions(-DEDITOR_EXPORTS)
add_definitions(-DUNICODE -D_UNICODE)

set(SOURCE ieditor.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)
					
if (${ENABLE_TESTING})

	enable_testing()
endif( ${ENABLE_TESTING} )	