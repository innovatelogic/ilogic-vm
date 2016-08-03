cmake_minimum_required(VERSION 2.8)

project(Launcher)

find_package(Boost COMPONENTS signals REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR})
endif()

set(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet3-2.83.6/src/)


include_directories(${CMAKE_SOURCE_DIR}/common/SDK)
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common/WTL/Include)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet-2.81-rev2613/src)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)
include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/D3DDrv)
include_directories(${CMAKE_SOURCE_DIR}/Foundation)
include_directories(${CMAKE_SOURCE_DIR}/RenderSDK)
include_directories(${CMAKE_SOURCE_DIR}/CoreSDK)
include_directories(${CMAKE_SOURCE_DIR}/CoreSDK/editors)
include_directories(${CMAKE_SOURCE_DIR}/Editors)
include_directories(${CMAKE_SOURCE_DIR}/Launcher/src)


#link_directories(${CMAKE_SOURCE_DIR}/common/SDK/FreeType-2.1.4-lib/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)


SET(BULLET_LIBRARY ${CMAKE_BINARY_DIR}/../Libs)

SET(BULLET_LIBRARIES debug ${CMAKE_BINARY_DIR}/../Libs/Bullet2FileLoader_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet2FileLoader.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/Bullet3Collision_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet3Collision.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/Bullet3Common_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet3Common.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/Bullet3Dynamics_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet3Dynamics.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/Bullet3Geometry_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet3Geometry.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/Bullet3OpenCL_clew_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/Bullet3OpenCL_clew.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletCollision_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletCollision.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletDynamics_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletDynamics.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletFileLoader_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletFileLoader.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletSoftBody_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletSoftBody.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletWorldImporter_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletWorldImporter.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/BulletXmlWorldImporter_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/BulletXmlWorldImporter.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/ConvexDecomposition_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/ConvexDecomposition.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/HACD_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/HACD.lib)
SET(BULLET_LIBRARIES ${GLEW_LIBRARIES} debug ${CMAKE_BINARY_DIR}/../Libs/LinearMath_Debug.lib optimized ${CMAKE_BINARY_DIR}/../Libs/LinearMath.lib)


set(LIBRARIES	CommonBase
				Foundation 
				RenderSDK 
				CoreSDK 
				Editors
				nv_math 
				${BULLET_LIBRARIES} 
				${TINY_XML_LIBRARY} 
				${FREETYPE_LIBRARIES})

set(SOURCE 
	src/AssetBrowser.h
	src/DragDropSource.h
	src/GameLoop.h
	src/IAssetBrowserFrame.h
	icon1.ico
	src/ImageBitmap.cpp
	src/ImageBitmap.h
	src/Launcher.cpp
	LauncherStdAfx.h
	Licence.txt
	ListVeiwEdit.h
	ListViewCombobox.h
	ListViewEditResource.h
	MainFrame.h
	MainFrame.ipp
	PaneListView.h
	PanePropertyContainer.h
	pane_property_container.ipp
	PaneTreeContainer.h
	PaneTreeContainer.ipp
	PaneTreeView.h
	PaneViewAssetContainer.h
	PaneViewContrainer.h
	resource.h
	ThreadManager.cpp
	ThreadManager.h
	ThreadManagerSerial.cpp
	ThreadManagerSerial.h
	ToolbarControl.cpp
	ToolbarControl.h
	ToolbarHelper.h
	Win32ImageList.cpp
	Win32ImageList.h
	Win32InputBox.cpp
	Win32InputBox.h
	Win32ItemsTree.h
	Win32ObjectBrowserWidget.ipp
	Win32ObjectBrowserWidget.h
	Win32ObjectToolbox.cpp
	Win32ObjectToolbox.h
	Win32ThreadMonitor.cpp
	Win32ThreadMonitor.h
	src/WTLItemsTree.h
	src/WTLPlacementWidget.h
	src/WTLPropertyGrid.h
	src/wtlpropertygrid.ipp
	src/MSelectTree.h
	src/iproperty_reactor.h
	src/property_reactor.h
	src/property_reactor.ipp
	src/splash_window.h
	src/splash_window.cpp
	)

IF(WIN32)
  SET(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
ENDIF(WIN32)
	
add_executable(${PROJECT_NAME} WIN32 ${SOURCE} resource.rc)

target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${Boost_LIBRARIES})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#       "${PROJECT_SOURCE_DIR}/../common/zlib/win32/zlib.dll"      # <--this is in-file
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")	

# Find user and system name
#SET(SYSTEM_NAME $ENV{USERDOMAIN} CACHE STRING SystemName)
#SET(USER_NAME $ENV{USERNAME} CACHE STRING UserName)

# Configure the template file
#SET(USER_FILE ${_projectName}.vcproj.${SYSTEM_NAME}.${USER_NAME}.user)
#SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
#CONFIGURE_FILE(UserTemplate.user ${USER_FILE} @ONLY)