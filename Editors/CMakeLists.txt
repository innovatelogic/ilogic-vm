cmake_minimum_required(VERSION 2.8)

project(Editors)

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DLIB_COMPILE)
add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY)

include_directories(${CMAKE_SOURCE_DIR}/utils/gtest/include)
include_directories(${CMAKE_SOURCE_DIR}/utils/googlemock/include)
include_directories(${CMAKE_SOURCE_DIR}/utils/googlemock/include/gmock)

set(SOURCE ieditor.h 
			editor.cpp
			editor.h
			module.h
			icommand.h
			scene_editor.h
			)

set(SOURCE_MOC moc_editor.cxx)

source_group("Moc Files" FILES ${SOURCE_MOC})

set(LIBRARIES Foundation D3DDriver CoreSDK)

link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)

add_library(${PROJECT_NAME} SHARED ${SOURCE} ${SOURCE_MOC})
	
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${FREETYPE_LIBRARIES})
	
set_target_properties( ${PROJECT_NAME} PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        )

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------					
if (${ENABLE_TESTING})

	include_directories(${CMAKE_SOURCE_DIR}/utils/gtest/include)

	set( PROJECT_NAME ${PROJECT_NAME} )
	set( PROJECT_TEST_NAME UT.${PROJECT_NAME} )
	string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_U )

	add_executable(  ${PROJECT_TEST_NAME}
					ut/ut_editor.cpp
					)
	
	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

	add_test(NAME ${PROJECT_NAME_U}_TEST COMMAND ${PROJECT_TEST_NAME})

	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES FOLDER "UT" )
	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

endif( ${ENABLE_TESTING} )	