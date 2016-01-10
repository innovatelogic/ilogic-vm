cmake_minimum_required(VERSION 2.8)

project(Foundation)

add_definitions(-DFOUNDATION_EXPORTS)
add_definitions(-DUNICODE -D_UNICODE)

find_package(Boost COMPONENTS signals REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR})
endif()

include_directories(${COMMON_SDK_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)

set(SOURCE
	platform_specific.h
	_String.cpp
	_String.h
	Actor.cpp
	Actor.h
	Actor_Static.cpp
	AutoTree.h
	BoundingTree.h
	Bounds2f.h
	Bounds3f.h
	ClassFactory.cpp
	ClassFactory.h
	ClassNode.cpp
	ClassNode.h
	ClassNodeInterface.h
	ClassNodeType.h
	ClassTree.cpp
	ClassTree.h
	CriticalSection.cpp
	CriticalSection.h
	DefTypes.h
	EventDesc.cpp
	EventDesc.h
	FoundationSDK.h
	geometry.cpp
	geometry.h
	IBaseInterface.h
	ICollisionInterface.h
	IListener.h
	IRenderInterface.h
	m_attribute.cpp
	m_attribute.h
	m_file.cpp
	m_file.h
	m_streams.cpp
	m_streams.h
	math_decl.h
	math_float.h
	mathlib.cpp
	mathlib.h
	matrix.cpp
	matrix.h
	matrix3.cpp
	matrix3.h
	MemoryAllocator.cpp
	MemoryAllocator.h
	MessageMap.h
	ObjectAbstract.cpp
	ObjectAbstract.h
	ObjectDeclaration.h
	ObjectFactory.cpp
	ObjectFactory.h
	Property.cpp
	Property.h
	quat.cpp
	quat.h
	singleton.h
	StdafxFoundation.h
	Streams.cpp
	Streams.h
	StringUtility.cpp
	StringUtility.h
	TArray.cpp
	TArray.h
	UtContainer.cpp
	UtContainer.h
	UtFile.cpp
	UtFile.h
	UtMemory.h
	ValueParser.h
	vector.cpp
	vector.h
	vector2f.cpp
	vector2f.h
	vector4f.cpp
	vector4f.h
	xmlparser.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} ${TINY_XML_LIBRARY})

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})

	enable_testing()

	include_directories(${CMAKE_SOURCE_DIR}/utils/gtest-1.7.0/include)
	add_executable( UT.${PROJECT_NAME} 
					ut/ut_vector3f.cpp
					ut/main.cpp )
	
	target_link_libraries(UT.${PROJECT_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

	set_target_properties(UT.${PROJECT_NAME} PROPERTIES FOLDER "UT" )

	set_target_properties(UT.${PROJECT_NAME} PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						)

	add_test(foundation_test runUnitTests)
endif( ${ENABLE_TESTING} )					