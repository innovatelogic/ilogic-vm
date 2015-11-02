cmake_minimum_required(VERSION 3.0.0)

set(ENABLE_TESTING ON)

set(ENABLE_GTEST ON)
SET(GTEST_LIBRARIES optimized ${CMAKE_BINARY_DIR}/../Libs/gtest.lib debug ${CMAKE_BINARY_DIR}/../Libs/gtest_d.lib)
SET(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} optimized ${CMAKE_BINARY_DIR}/../Libs/gtest_main.lib debug ${CMAKE_BINARY_DIR}/../Libs/gtest_main_d.lib)

#if(ENABLE_TESTING)
#	find_package(GTest REQUIRED)
	enable_testing()
#endif()

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ROOT ${CMAKE_SOURCE_DIR}/Lib)
set(BOOST_INCLUDEDIR ${CMAKE_SOURCE_DIR}/common/SDK/)
set(BOOST_LIBRARYDIR ${CMAKE_SOURCE_DIR}/Lib)

add_subdirectory(common/TinyXML)
add_subdirectory(Foundation)
add_subdirectory(D3DDrv)
add_subdirectory(RenderSDK)
add_subdirectory(CoreSDK)
add_subdirectory(Launcher)
#add_subdirectory(MaxExporter)