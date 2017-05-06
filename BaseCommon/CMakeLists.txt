cmake_minimum_required(VERSION 3.0.0)

project(CommonBase)

add_definitions(-DCOMMON_LIB_COMPILE)
add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)

set(INCLUDES
include/OEMBase.h
include/TypesBase.h
include/HashBase.h
include/vector2f.h
include/vector.h
include/vector4f.h
include/quat.h
include/matrix.h
include/matrix3.h
include/mathlib.h
include/Bounds2f.h
include/Bounds3f.h
include/IBaseInterface.h
include/StringUtility.h
include/singleton.h
include/BaseTypes.h
include/IObjectAbstract.h
include/ICollisionInterface.h
include/IRenderInterface.h
include/ValueParser.h
include/UtFile.h
include/TArray.h
include/permutations.h
include/xmlparser.h
)

set(SRC
src/empty.cpp
src/vector2f.cpp
src/vector.cpp
src/vector4f.cpp
src/quat.cpp
src/matrix.cpp
src/matrix3.cpp
src/mathlib.cpp
src/StringUtility.cpp
src/UtFile.cpp
src/TArray.cpp
)

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SRC})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

target_link_libraries( ${PROJECT_NAME} 
					   ${TINY_XML_LIBRARY}
)
					
#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})
	include_directories(${CMAKE_SOURCE_DIR}/utils/googletest/googletest/include)

	set( PROJECT_NAME ${PROJECT_NAME} )
	set( PROJECT_TEST_NAME Tests_${PROJECT_NAME} )
	string( TOUPPER ${PROJECT_NAME} PROJECT_NAME_U )

	add_executable( ${PROJECT_TEST_NAME}
					ut/ut_treemap.cpp
					ut/ut_treemap_fixture.h
					ut/ut_matrix_transform.cpp
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
	
	set_target_properties( ${PROJECT_TEST_NAME} PROPERTIES
            OUTPUT_NAME ${PROJECT_TEST_NAME}.test
            DEBUG_POSTFIX ""
        )
endif( ${ENABLE_TESTING} )			