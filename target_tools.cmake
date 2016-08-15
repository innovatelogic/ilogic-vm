cmake_minimum_required(VERSION 3.0.0)

SET(GBOOST_SIGNALS_NO_DEPRECATION_WARNING ON)

set(ENABLE_TESTING ON)

set(ENABLE_GTEST ON)
SET(GTEST_LIBRARIES optimized ${CMAKE_BINARY_DIR}/../Libs/gtest.lib debug ${CMAKE_BINARY_DIR}/../Libs/gtest_d.lib)
SET(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} optimized ${CMAKE_BINARY_DIR}/../Libs/gtest_main.lib 
											debug ${CMAKE_BINARY_DIR}/../Libs/gtest_main_d.lib)

SET(TINY_XML_LIBRARY optimized ${CMAKE_BINARY_DIR}/../Libs/tinyxml2.lib debug ${CMAKE_BINARY_DIR}/../Libs/tinyxml2_d.lib)

SET(GMOCK_LIBRARY optimized ${CMAKE_BINARY_DIR}/../Libs/gmock.lib debug ${CMAKE_BINARY_DIR}/../Libs/gmock_d.lib)
SET(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARY}
					optimized ${CMAKE_BINARY_DIR}/../Libs/gmock_main.lib debug ${CMAKE_BINARY_DIR}/../Libs/gmock_main_d.lib)

if(ENABLE_TESTING)
#find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)
	enable_testing()
endif()

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/prebuild)
set(BOOST_INCLUDEDIR ${CMAKE_SOURCE_DIR}/common/SDK/)
set(BOOST_LIBRARYDIR ${CMAKE_SOURCE_DIR}/prebuild)

add_subdirectory(BaseCommon)
add_subdirectory(reflx)
add_subdirectory(Foundation)
add_subdirectory(D3DDrv)
add_subdirectory(RenderSDK)
add_subdirectory(Editors)
add_subdirectory(CoreSDK)
add_subdirectory(Launcher)
#add_subdirectory(MaxExporter)