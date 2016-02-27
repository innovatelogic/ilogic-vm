cmake_minimum_required(VERSION 2.8)

project(CoreSDK)

add_definitions(-DUNICODE -D_UNICODE)

set(BULLET_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/SDK/bullet/bullet3-2.83.6/src/)

include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK)
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
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
			  Foundation
			  RenderSDK
			  nv_math
			  Editors
			  ${BULLET_LIBRARIES}
			  ${TINY_XML_LIBRARY}
			  ${FREETYPE_LIBRARIES})

set(SOURCE 
	ActionBase.cpp
	ActionBase.h
	ActionEventUI.cpp
	ActionEventUI.h
	ActionHandler.cpp
	ActionHandler.h
	ActionLoadLevel.cpp
	ActionLoadLevel.h
	ActionTrigger.cpp
	ActionTrigger.h
	ActorAllocator.cpp
	ActorAllocator.h
	ActorAllocator_Static.cpp
	ActorCollisionController.cpp
	ActorCollisionController.h
	ActorCollisionKinematicController.cpp
	ActorCollisionKinematicController.h
	AnimationSequerence.cpp
	AnimationSequerence.h
	BaseModificator.cpp
	BaseModificator.h
	BaseModificatorUI.cpp
	BaseModificatorUI.h
	BaseSplineAgregator.cpp
	BaseSplineAgregator.h
	BezierSplineAgregator_Brush.cpp
	BezierSplineAgregator_Brush.h
	Bezier_SplineAgregator.cpp
	Bezier_SplineAgregator.h
	BoxCharacterController.cpp
	BoxCharacterController.h
	BoxCollisionController.cpp
	BoxCollisionController.h
	BoxCollisionController_Brush.cpp
	BoxCollisionController_Brush.h
	Brush_AbstractInterface.cpp
	Brush_AbstractInterface.h
	Brush_LevelActor.cpp
	Brush_LevelActor.h
	Camera.cpp
	Camera.h
	CameraActor.cpp
	CameraActor.h
	CameraEditor.cpp
	CameraEditor.h
	CameraManager.cpp
	CameraManager.h
	CanvasFrame.cpp
	CanvasFrame.h
	CharacterControl.cpp
	CharacterControl.h
	CollisionController.cpp
	CollisionController.h
	ContactSensorCallbackBt.cpp
	ContactSensorCallbackBt.h
	CoreSDK.cpp
	CoreSDK.h
	coresdkafx.h
	coresdkafx_actions.h
	coresdkafx_components.h
	coresdkafx_manager.h
	coresdkafx_objects.h
	coresdkafx_ui.h
	CPawn.cpp
	CPawn.h
	DirectionalLight.cpp
	DirectionalLight.h
	DummyObject.cpp
	DummyObject.h
	Explorer.cpp
	Explorer.h
	Explorer2D.cpp
	Explorer2D.h
	Explorer3D.cpp
	Explorer3D.h
	GameInfoManager.cpp
	GameInfoManager.h
	GameRealmInfo.cpp
	GameRealmInfo.h
	GameStaticInfo.cpp
	GameStaticInfo.h
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
	ImageProjection.cpp
	ImageProjection.h
	InputControllerEdImpl.cpp
	InputControllerEdImpl.h
	InputControllerImpl.cpp
	InputControllerImpl.h
	InputManager.cpp
	InputManager.h
	IPhysicsEngine.h
	IUIInputController.h
	IUIInputIController.cpp
	IUpdateInterface.cpp
	IUpdateInterface.h
	Label.cpp
	Label.h
	LabelButton.cpp
	LabelButton.h
	LabelButton_Brush.cpp
	LabelButton_Brush.h
	LevelActor.cpp
	LevelActor.h
	LevelPlaneActor.cpp
	LevelPlaneActor.h
	LevitateVehicle.cpp
	LevitateVehicle.h
	MeshCollisionComponent.cpp
	MeshCollisionComponent.h
	ModelViewer.cpp
	ModelViewer.h
	MovementData.cpp
	MovementData.h
	object_export.h
	PawnCueBall.cpp
	PawnCueBall.h
	PhysDebugDrawerGL.cpp
	PhysDebugDrawerGL.h
	physicsafx.h
	PhysicsEngineBullet.cpp
	PhysicsEngineBullet.h
	PhysNode.cpp
	PhysNode.h
	PlaneCollisionController.cpp
	PlaneCollisionController.h
	Point2D.cpp
	Point2D.h
	Point2DBezierKey.cpp
	Point2DBezierKey.h
	PointBezier2D.cpp
	PointBezier2D.h
	PointLight.cpp
	PointLight.h
	PolygonImage.cpp
	PolygonImage.h
	PolygonMesh.cpp
	PolygonMesh.h
	project.h
	ProjectedLight.cpp
	ProjectedLight.h
	Registry.cpp
	Registry.h
	RemoteControlManager.cpp
	RemoteControlManager.h
	RestrictedVolume.cpp
	RestrictedVolume.h
	RestrictedVolumeCollisionController.cpp
	RestrictedVolumeCollisionController.h
	Scene.cpp
	Scene.h
	SceneGameArcanoid.cpp
	SceneGameArcanoid.h
	SceneManager.cpp
	SceneManager.h
	SceneSelectLevel.cpp
	SceneSelectLevel.h
	ScriptDriver.cpp
	ScriptDriver.h
	SkyDome.cpp
	SkyDome.h
	SphereCollisionController.cpp
	SphereCollisionController.h
	SphereCollisionController_Brush.cpp
	SphereCollisionController_Brush.h
	SphereController.cpp
	SphereController.h
	SplineAgregator.cpp
	SplineAgregator.h
	SplineAgregator_Brush.cpp
	SplineAgregator_Brush.h
	SplineAgregator_PointInserter_Brush.cpp
	SplineAgregator_PointInserter_Brush.h
	SplineCollision2DModificator.cpp
	SplineCollision2DModificator.h
	SplineCollision2DModificator_Brush.cpp
	SplineCollision2DModificator_Brush.h
	SplineMovementController.cpp
	SplineMovementController.h
	SplinePolygon.cpp
	SplinePolygon.h
	TriggerCallbackReport.cpp
	TriggerCallbackReport.h
	TriggerObject.cpp
	TriggerObject.h
	UIFilter.cpp
	UIFilter.h
	UIFilter_Brush.cpp
	UIFilter_Brush.h
	UIImage.cpp
	UIImage.h
	UIImagePivotControl.cpp
	UIImagePivotControl.h
	UIImage_Brush.cpp
	UIImage_Brush.h
	UIScene.cpp
	UIScene.h
	UISceneBaseHud.cpp
	UISceneBaseHud.h
	UISceneLoadingScreen.cpp
	UISceneLoadingScreen.h
	UISceneLogin.cpp
	UISceneLogin.h
	UISceneManager.cpp
	UISceneManager.h
	UIScenePauseMenu.cpp
	UIScenePauseMenu.h
	UIScreenObject.cpp
	UIScreenObject.h
	UIScreenObject_Static.cpp
	UISprite.cpp
	UISprite.h
	UISprite_Brush.cpp
	UISprite_Brush.h
	UIViewPivotControl.cpp
	UIViewPivotControl.h
	UpdateManager.cpp
	UpdateManager.h
	UVModificator.cpp
	UVModificator.h
	UVModificator_Brush.cpp
	UVModificator_Brush.h
	ValueParser.h
	ViewportInterface.cpp
	ViewportInterface.h
	ViewportManager.cpp
	ViewportManager.h
	)
	
add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})