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
include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)

set(SOURCE
	platform_specific.h
	Actor.cpp
	Actor.h
	Actor_Static.cpp
	BoundingTree.h
	CriticalSection.cpp
	CriticalSection.h
	EventDesc.cpp
	EventDesc.h
	FoundationSDK.h
	geometry.cpp
	geometry.h
	IListener.h
	m_attribute.cpp
	m_attribute.h
	m_file.cpp
	m_file.h
	m_streams.cpp
	m_streams.h
	math_float.h
	MemoryAllocator.cpp
	MemoryAllocator.h
	MessageMap.h
	ObjectAbstract.cpp
	ObjectAbstract.h
	StdafxFoundation.h
	Streams.cpp
	Streams.h
	UtContainer.cpp
	UtContainer.h
	UtMemory.h
	xmlparser.h
	IEventManager.h
)

set(SOURCE_API

)

source_group("API" FILES ${SOURCE_API})

set(LIBRARIES CommonBase)

add_library(${PROJECT_NAME} SHARED ${SOURCE} ${SOURCE_API})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

target_link_libraries( ${PROJECT_NAME} 
					   ${LIBRARIES}
					   ${Boost_LIBRARIES}
					   ${TINY_XML_LIBRARY}
 )

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})

	enable_testing()

	include_directories(${CMAKE_SOURCE_DIR}/utils/googletest/googletest/include)
	add_executable( unittest_${PROJECT_NAME} 
					ut/ut_vector3f.cpp
					ut/main.cpp )
	
	target_link_libraries(unittest_${PROJECT_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

	set_target_properties(unittest_${PROJECT_NAME} PROPERTIES FOLDER "UT" )

	set_target_properties(unittest_${PROJECT_NAME} PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						)

	add_test(foundation_test runUnitTests)
endif( ${ENABLE_TESTING} )					