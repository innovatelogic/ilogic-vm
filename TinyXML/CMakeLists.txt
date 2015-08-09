cmake_minimum_required(VERSION 2.8)

project(TinyXML)

add_definitions(-DTINYXML_EXPORT)
add_definitions(-DUNICODE -D_UNICODE)

set(SOURCE 
	tinystr.cpp
	tinystr.h
	tinyxml.cpp
	tinyxml.h
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	)
	
add_library(${PROJECT_NAME} SHARED ${SOURCE})

#SET_BIN_OUTPUT_FOLDER(${PROJECT_NAME} ${G_BIN_DIR})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)