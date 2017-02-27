cmake_minimum_required(VERSION 2.8)

project(Editors)

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DLIB_COMPILE)
add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY)

include_directories(${COMMON_SDK_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
include_directories(${CMAKE_SOURCE_DIR}/utils/googletest/googletest/include)
include_directories(${CMAKE_SOURCE_DIR}/utils/googletest/googlemock/include)
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)
include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/reflx/include)
include_directories(${CMAKE_SOURCE_DIR}/D3DDrv)
include_directories(${CMAKE_SOURCE_DIR}/Foundation)
include_directories(${CMAKE_SOURCE_DIR}/RenderSDK)
include_directories(${CMAKE_SOURCE_DIR}/CoreSDK)
include_directories(${CMAKE_SOURCE_DIR}/CoreSDK/editors)

set(SOURCE ieditor.h 
			editor.cpp
			editor.h
			module.h
			icommand.h
			icommand_buffer.h
			command_buffer.h
			command_buffer.cpp
			command_base.cpp
			command_base.h
			selection_container.h
			)

set(SOURCE_EDITORS 
	scene_editor.h
	scene_editor.cpp
	scene_editor_main.h
	scene_editor_main.cpp)

set(LIBRARIES	CommonBase
				reflx
				Foundation
				D3DDriver
				RenderSDK
				CoreSDK
				)

source_group("Editors" FILES ${SOURCE_EDITORS})

#TODO: add optional linking
set(LIBRARIES ${LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)

add_library(${PROJECT_NAME} SHARED ${SOURCE} ${SOURCE_EDITORS})
	
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

	enable_testing()

	set( PROJECT_NAME ${PROJECT_NAME} )
	set( PROJECT_TEST_NAME unittest_${PROJECT_NAME} )
	string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_U )

	set(SOURCE_MOC moc/moc_editor.cxx
		moc/moc_command_buffer.cxx
		moc/moc_command_base.cxx
	)

	set(SOURCE_UT 
	ut/ut_editor.cpp
	ut/ut_selection_component.cpp
	ut/ut_property_reactor.cpp
	)

	source_group("Moc Files" FILES ${SOURCE_MOC})

	add_executable( ${PROJECT_TEST_NAME}
					${SOURCE_MOC}
					${SOURCE_UT}
					)
	
	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})

	add_definitions(-DUSE_MOCK)

	add_test(NAME ${PROJECT_NAME_U}_TEST COMMAND ${PROJECT_TEST_NAME})

	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES FOLDER "UT")
	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

	set_target_properties( ${PROJECT_TEST_NAME} PROPERTIES
            OUTPUT_NAME ${PROJECT_TEST_NAME}.test
            DEBUG_POSTFIX ""
        )

endif( ${ENABLE_TESTING} )	