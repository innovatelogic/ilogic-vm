cmake_minimum_required(VERSION 2.8)

project(Launcher)

find_package(Boost COMPONENTS signals REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIR})
endif()


include_directories(${CMAKE_SOURCE_DIR}/common/SDK)
include_directories(${CMAKE_SOURCE_DIR}/common/WTL/Include)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet-2.81-rev2613/src)
include_directories(${CMAKE_SOURCE_DIR}/D3DDrv)
include_directories(${CMAKE_SOURCE_DIR}/TinyXML)
include_directories(${CMAKE_SOURCE_DIR}/Foundation)
include_directories(${CMAKE_SOURCE_DIR}/RenderSDK)
include_directories(${CMAKE_SOURCE_DIR}/CoreSDK)

#link_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.9.0/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/FreeType-2.1.4-lib/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet-2.81-rev2613/lib/)

set(BULLET_ARCHIVES BulletCollision_vs2010_debug.lib
					BulletDynamics_vs2010_debug.lib
					BulletSoftBody_vs2010_debug.lib
					ConvexDecomposition_vs2010_debug.lib
					HACD_vs2010_debug.lib
					BulletDynamics_vs2010_debug.lib
					LinearMath_vs2010_debug.lib)

set(LIBRARIES TinyXML Foundation RenderSDK CoreSDK nv_math ${BULLET_ARCHIVES})

set(SOURCE 
	ActionPlayEditor.cpp
	ActionPlayEditor.h
	AssetBrowser.h
	DragDropSource.h
	GameLoop.h
	IAssetBrowserFrame.h
	icon1.ico
	ImageBitmap.cpp
	ImageBitmap.h
	Launcher.cpp
	LauncherStdAfx.h
	Launcher_Actors.cpp
	Licence.txt
	ListVeiwEdit.h
	ListViewCombobox.h
	ListViewEditResource.h
	MainFrame.h
	PaneListView.h
	PanePropertyContainer.h
	PaneTreeContainer.h
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
	Win32CameraManager.cpp
	Win32CameraManager.h
	Win32ImageList.cpp
	Win32ImageList.h
	Win32InputBox.cpp
	Win32InputBox.h
	Win32ItemsTree.h
	Win32ObjectBrowserWidget.cpp
	Win32ObjectBrowserWidget.h
	Win32ObjectToolbox.cpp
	Win32ObjectToolbox.h
	Win32PropertyGrid.cpp
	Win32PropertyGrid.h
	Win32ThreadMonitor.cpp
	Win32ThreadMonitor.h
	WindowManager.cpp
	WindowManager.h
	WTLItemsTree.h
	WTLPlacementWidget.h
	WTLPropertyGrid.h
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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/../common/zlib/win32/zlib.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
