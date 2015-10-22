cmake_minimum_required(VERSION 2.8)

project(Foundation)

add_definitions(-DFOUNDATION_EXPORTS)
add_definitions(-DUNICODE -D_UNICODE)

find_package(Boost COMPONENTS signals REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR})
endif()

set(LIBRARIES TinyXML)

include_directories(${COMMON_SDK_DIR})

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
	Mutex.h
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
target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} ${Boost_LIBRARIES})

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})

	enable_testing()
	
	find_package(gtest REQUIRED)
	
	include_directories(${GTEST_INCLUDE_DIRS})

	

	add_executable( ut.${PROJECT_NAME} 
					ut/ut_vector3f.cpp
					ut/main.cpp )
					
	#set(GTEST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/../utils/gtest-1.7.0)
	
	target_link_libraries(ut.${PROJECT_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES}) #${GTEST_BOTH_LIBRARIES

	set_target_properties(ut.${PROJECT_NAME} PROPERTIES FOLDER "UT" )

	set_target_properties( ut.${PROJECT_NAME} PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						)
endif( ${ENABLE_TESTING} )					