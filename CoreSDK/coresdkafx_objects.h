#ifndef __coresdkafx_objects_h__
#define __coresdkafx_objects_h__

#ifdef _WIN32
#pragma once
#endif

#include "ActorAllocator.h"

#ifndef __idrawinterface_h__
#include "IDrawInterface.h"
#endif

#ifndef __idrawinterfaceui_h__
#include "IDrawInterfaceUI.h"
#endif

#ifndef __iupdateinterface_h__
#include "IUpdateInterface.h"
#endif

#ifndef __iinputinterface_h__
#include "IInputInterface.h"
#endif

#ifndef __inputcontrollerimpl_h__
#include "InputControllerImpl.h"
#endif

#ifndef __inputcontrolleredimpl_h__
#include "InputControllerEdImpl.h"
#endif

#ifndef __viewportinterface_h__
#include "ViewportInterface.h"
#endif

#ifndef __ientitycontroller_h__
#include "IEntityController.h"
#endif

#ifndef __iuiinputcontroller_h__
#include "IUIInputController.h"
#endif

#include "Explorer.h"
#include "Explorer2D.h"
#include "Explorer3D.h"
#include "ModelViewer.h"

#include "SkyDome.h"
#include "src/Camera.h"
#include "PawnCueBall.h"
#include "DummyObject.h"
//#include "LevelActor.h"
#include "LevelPlaneActor.h"
#include "Scene.h"
#include "TriggerObject.h"
#include "RestrictedVolume.h"
#include "UVModificator.h"
#include "SplineCollision2DModificator.h"
#include "SplineMovementController.h"
#include "CollisionController.h"
#include "BoxCharacterController.h"
#include "BoxCollisionController.h"
#include "SphereCollisionController.h"
#include "PlaneCollisionController.h"
#include "ActorCollisionController.h"
#include "RestrictedVolumeCollisionController.h"
#include "MeshCollisionComponent.h"
#include "SplinePolygon.h"

#include "BaseModificator.h"
#include "BaseModificatorUI.h"
#include "PolygonImage.h"

#endif//__coresdkafx_objects_h__