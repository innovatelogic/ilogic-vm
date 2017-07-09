cmake_minimum_required(VERSION 2.8)

project(CoreSDK)

add_definitions(-DUNICODE -D_UNICODE)

set(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet3-2.83.6/src/)

include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common/SDK)
include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/reflx/include)
include_directories(${CMAKE_SOURCE_DIR}/D3DDrv)
include_directories(${CMAKE_SOURCE_DIR}/common/tinyxml2)
include_directories(${CMAKE_SOURCE_DIR}/Foundation)
include_directories(${CMAKE_SOURCE_DIR}/RenderSDK)
include_directories(${CMAKE_SOURCE_DIR}/Editors)

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

set(LIBRARIES CommonBase
			  reflx
			  Foundation
			  RenderSDK
			  nv_math
			  ${BULLET_LIBRARIES}
			  ${TINY_XML_LIBRARY}
			  ${FREETYPE_LIBRARIES})

set(SOURCE 
	transform_traits.cpp
	transform_traits.h
	transform_history_traits.cpp
	transform_history_traits.h
	ActorAllocator.cpp
	ActorAllocator.h
	ActorAllocator_Static.cpp
	CameraManager.cpp
	CameraManager.h
	CoreSDK.cpp
	CoreSDK.h
	coresdkafx.h
	coresdkafx_components.h
	coresdkafx_manager.h
	coresdkafx_objects.h
	coresdkafx_ui.h
	Explorer.cpp
	Explorer.h
	Explorer2D.cpp
	Explorer2D.h
	Explorer3D.cpp
	Explorer3D.h
	EventManager.h
	EventManager.cpp
	game_types.h
	IDrawInterface.cpp
	IDrawInterface.h
	IDrawInterfaceUI.cpp
	IDrawInterfaceUI.h
	IEntityController.cpp
	IEntityController.h
	IInputController.h
	IInputInterface.cpp
	IInputInterface.h
	IEventInterface.h
	IEventInterface.cpp
	InputControllerEdImpl.cpp
	InputControllerEdImpl.h
	InputControllerImpl.cpp
	InputControllerImpl.h
	InputManager.cpp
	InputManager.h
	IUIInputController.h
	IUIInputIController.cpp
	IUpdateInterface.cpp
	IUpdateInterface.h
	Label.cpp
	Label.h
	LabelButton.cpp
	LabelButton.h
	LevelPlaneActor.cpp
	LevelPlaneActor.h
	ModelViewer.cpp
	ModelViewer.h
	object_export.h
	Registry.cpp
	Registry.h
	RemoteControlManager.cpp
	RemoteControlManager.h
	Scene.cpp
	Scene.h
	SceneManager.cpp
	SceneManager.h
	SkyDome.cpp
	SkyDome.h
	UIImage.cpp
	UIImage.h
	UIScene.cpp
	UIScene.h
	UISceneManager.cpp
	UISceneManager.h
	UIScreenObject.cpp
	UIScreenObject.h
	UIScreenObject_Static.cpp
	UpdateManager.cpp
	UpdateManager.h
	ViewportInterface.ipp
	ViewportInterface.h
	ViewportManager.cpp
	ViewportManager.h
	UIViewPivotControl.h
	UIViewPivotControl.cpp
	UIImagePivotControl.h
	UIImagePivotControl.cpp
	)

set(SOURCE_API
	include/Base.h
	include/BaseActor.h
	include/PxCamera.h
	include/PxLevelActor.h
)

set(SRC_SOURCE
	src/NpActorTemplate.h
	src/Camera.cpp
	src/Camera.h
	src/LevelActor.cpp
	src/LevelActor.h
)

source_group("API" FILES ${SOURCE_API})
	
add_library(${PROJECT_NAME} SHARED ${SOURCE} ${SOURCE_API} ${SRC_SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

set_target_properties( ${PROJECT_NAME} PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        )