cmake_minimum_required(VERSION 3.0.0)

project(CommonBase)

add_definitions(-DUNICODE -D_UNICODE)

set(INCLUDES
include/OEMBase.h
include/TypesBase.h
include/HashBase.h
)

set(SRC
src/empty.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDES} ${SRC})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)