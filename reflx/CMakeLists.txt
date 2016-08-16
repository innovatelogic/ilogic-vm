cmake_minimum_required(VERSION 3.0.0)

project(reflx)

add_definitions(-DREFLX_LIB_COMPILE)
add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/reflx/include)

set(LIBRARIES CommonBase)

set(INCLUDES
	include/defexport.h
	include/ObjectDeclaration.h
	include/Property.h
	include/ClassNode.h
	include/ClassNodeType.h
	include/ClassFactory.h
	include/ObjectFactory.h
	include/ClassTree.h
	include/ClassNodeInterface.h
	include/AutoTree.h
	include/reflx.h
)

set(SRC
	src/Property.cpp
	src/ClassNode.cpp
	src/ClassTree.cpp
	src/ClassFactory.cpp
	src/ObjectFactory.cpp
	src/main.cpp
)

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SRC})

target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} )

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
	include_directories(${CMAKE_SOURCE_DIR}/utils/googletest/googletest/include)

	set( PROJECT_NAME ${PROJECT_NAME} )
	set( PROJECT_TEST_NAME unittest_${PROJECT_NAME} )
	string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_U )

	add_executable( ${PROJECT_TEST_NAME}
					ut/ut_appclasstree.cpp
					ut/ut_autotree.cpp
					ut/ut_classtree.cpp
					ut/main.cpp )
	
	target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

	add_test(NAME ${PROJECT_NAME_U}_TEST COMMAND ${PROJECT_TEST_NAME})

	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES FOLDER "UT" )
	set_target_properties(${PROJECT_TEST_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

endif( ${ENABLE_TESTING} )			