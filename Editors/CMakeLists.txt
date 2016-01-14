cmake_minimum_required(VERSION 2.8)

project(Editors)

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DLIB_COMPILE)

set(SOURCE ieditor.h 
			editor.cpp
			editor.h
			module.h
			icommand.h
			scene_editor.h
			)
			
set(LIBRARIES Foundation D3DDriver CoreSDK)

#link_directories(${CMAKE_SOURCE_DIR}/common/SDK/FreeType-2.1.4-lib/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
	
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${FREETYPE_LIBRARIES})
	
set_target_properties( ${PROJECT_NAME} PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        )
					
if (${ENABLE_TESTING})
	enable_testing()
endif( ${ENABLE_TESTING} )	