/*#include "coresdkafx.h"
#include "CharacterControl.h"
#include "TriggerCallbackReport.h"

#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxBoxController.h"
#include "NxControllerManager.h"

#if defined(DEBUG) | defined(_DEBUG)
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/NxCharacter.lib")
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/PhysXCooking.lib")
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/PhysXLoader.lib")
#else
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/NxCharacterCHECKED.lib")
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/PhysXCookingCHECKED.lib")
#pragma comment( lib, "../common/SDK/PhysX/v2.8.3/SDKs/lib/Win32/PhysXLoaderCHECKED.lib")
#endif

#pragma warning(disable:4244)

//----------------------------------------------------------------------------------------------
PhysicsEnginePhysX::PhysicsEnginePhysX(CCoreSDK* pSDK)
: pCoreSDK(pSDK)
, bEnableSimulation(true)
, m_pPhisicsSDK(NULL)
, m_pScene(NULL)
, m_pControllerManager(NULL)
, DefaultGravity(0.0f, -9.8f, 0.0f)
, FixedTimestep(1.0f / 60.0f)
, bRenderDebug(false)
{

}

//----------------------------------------------------------------------------------------------
PhysicsEnginePhysX::~PhysicsEnginePhysX()
{
	Release();
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::Initialize()
{
	Lock();

	m_pMyAllocator = new UserAllocator;
	m_pControllerReport = new ControllerHitReport;
	m_pTriggerCallbackReport = new TriggerCallbackReport(pCoreSDK);

	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;

	m_pPhisicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, m_pMyAllocator, 0, NxPhysicsSDKDesc(), &errorCode);

// 	assert(errorCode != NXCE_PHYSX_NOT_FOUND);
// 	assert(errorCode != NXCE_WRONG_VERSION);
// 	assert(errorCode != NXCE_DESCRIPTOR_INVALID);
// 	assert(errorCode != NXCE_CONNECTION_ERROR);
// 	assert(errorCode != NXCE_RESET_ERROR);
// 	assert(errorCode != NXCE_IN_USE_ERROR);
// 	assert(errorCode != NXCE_BUNDLE_ERROR);

	if (m_pPhisicsSDK)
	{
		// Set the physics parameters
		//m_pPhisicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

#ifndef FINAL_RELEASE
// 		if (m_pPhisicsSDK->getFoundationSDK().getRemoteDebugger()){
// 			m_pPhisicsSDK->getFoundationSDK().getRemoteDebugger()->connect("localhost", NX_DBG_DEFAULT_PORT, NX_DBG_EVENTMASK_EVERYTHING);
// 		}
// 
 		// Set the debug visualization parameters
// 		m_pPhisicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.f);
//   		m_pPhisicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.f);
//  		m_pPhisicsSDK->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.f);

		// 	m_pPhisicsSDK->setParameter(NX_VISUALIZE_COLLISION_VNORMALS, 1);
		// 	m_pPhisicsSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);
#endif
		
		NxSceneDesc sceneDesc;
		sceneDesc.simType = NX_SIMULATION_SW;
		sceneDesc.gravity = DefaultGravity;
		sceneDesc.maxTimestep = FixedTimestep;
		sceneDesc.maxIter = 8;
		sceneDesc.timeStepMethod = NX_TIMESTEP_FIXED;

		m_pScene = m_pPhisicsSDK->createScene(sceneDesc);

		if (m_pScene)
		{
			NxMaterial * defaultMaterial = m_pScene->getMaterialFromIndex(0); 
			defaultMaterial->setRestitution(0.5f);
			defaultMaterial->setStaticFriction(0.5f);
			defaultMaterial->setDynamicFriction(0.5f);

			m_pControllerManager = NxCreateControllerManager(m_pMyAllocator);
			assert(m_pControllerManager);

			m_pScene->setTiming(FixedTimestep / 4.f, 8, NX_TIMESTEP_FIXED);
			m_pScene->setUserTriggerReport(m_pTriggerCallbackReport);

			m_pScene->simulate(0.0f);
			m_pScene->flushStream();
		}
	}

	Unlock();
	return true;
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::Release()
{
	if (m_pPhisicsSDK)
	{
		if (m_pControllerManager)
		{
			NxReleaseControllerManager(m_pControllerManager);
			m_pControllerManager = NULL;
		}

		if (m_pScene)
		{
			m_pPhisicsSDK->releaseScene(*m_pScene);
			m_pScene = NULL;
		}

		m_pPhisicsSDK->release();
		m_pPhisicsSDK = NULL;

		SAFE_DELETE(m_pMyAllocator);
		SAFE_DELETE(m_pControllerReport);
		SAFE_DELETE(m_pTriggerCallbackReport);
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::Simulate(float ElapsedTime)
{
	if (bEnableSimulation && m_pPhisicsSDK)
	{
		if (m_pScene)
		{
			NxReal maxTimestep;
			NxTimeStepMethod method;
			NxU32 maxIter;
			NxU32 numSubSteps;

			Lock();

			SimulateCharacters(ElapsedTime);

			m_pScene->simulate(ElapsedTime);
			m_pScene->flushStream();
			
			while (!m_pScene->fetchResults(NX_RIGID_BODY_FINISHED, false))
			{
			}

			m_pScene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
			
			if (numSubSteps)
			{
				m_pControllerManager->updateControllers();

				// post update all dynamic actors
				//PostSimulateUpdate();
			}

			Unlock();
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::SimulateCharacters(float ElapsedTime)
{
	if (m_pPhisicsSDK)
	{
		m_pControllerManager->updateControllers();

		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller)
			{
				// try to find movement data
				TMapMovementDataIterator Iter = MMovementData.find(Controller);

				if (Iter != MMovementData.end())
				{
					NxF32 sharpness = 1.0f;

					NxVec3 disp = DefaultGravity;

					NxVec3 HDisplace(Iter->second.vDisplace.x, Iter->second.vDisplace.y, Iter->second.vDisplace.z);
					HDisplace.normalize();

					disp += HDisplace * Iter->second.GetSpeed();
					
					NxU32 collisionFlags = MoveCharacter(Controller, disp, ElapsedTime, COLLIDABLE_MASK, Iter->second.GetHeight(ElapsedTime, DefaultGravity));
					
					if (collisionFlags & NXCC_COLLISION_DOWN){
						Iter->second.StopJump();
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
unsigned int PhysicsEnginePhysX::MoveCharacter(NxController * Controller,
												const Vector *dispVector,
												float elapsedTime,
												unsigned int collisionGroups, 
												float heightDelta)
{
	NxF32 sharpness = 1.0f;

	NxU32 collisionFlags;

	NxVec3 d = *dispVector * elapsedTime;

	if (heightDelta != 0.0f){
		d.y += heightDelta;
	}

	Controller->move(d, collisionGroups, 0.000001f, collisionFlags, sharpness);

	// for 2D simulation
	//NxExtendedVec3 pos = Controller->getDebugPosition();
	//pos.z = 0;
	//Controller->setPosition(pos);

	return collisionFlags;
	return 0;
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::PostSimulateUpdate()
{
	size_t NumControllers = m_pControllerManager->getNbControllers();

	for (size_t Index = 0; Index < NumControllers; ++Index)
	{
		NxController * Controller = m_pControllerManager->getController((NxU32)Index);

		if (Controller)
		{
			CActor * Actor = static_cast<CActor*> (Controller->getUserData());

			if (Actor)
			{
				NxExtendedVec3 Position = Controller->getPosition();
				Vector NewPosition(Position.x / PHIS_MULTYPLIER, Position.y / PHIS_MULTYPLIER, Position.z / PHIS_MULTYPLIER);

				if (NewPosition == Actor->GetPosition()){
					continue;
				}

				Matrix Mat;
				Mat.t = NewPosition;
				SyncPhysicsInfo.insert(std::make_pair(Actor, Mat));
			}
		}
	}

	// Render all the actors in the scene
	size_t NumActors = m_pScene->getNbActors();
	NxActor** Actors = m_pScene->getActors();

	while (NumActors--)
	{
		NxActor * Actor = *Actors++;

		if (Actor->isDynamic())
		{
			CActor * SourceActor = static_cast<CActor*> (Actor->userData);

			if (SourceActor)
			{
				NxMat34 ActorPose = Actor->getGlobalPose();

				NxVec3 Row_0 = ActorPose.M.getRow(0);
				NxVec3 Row_1 = ActorPose.M.getRow(1);
				NxVec3 Row_2 = ActorPose.M.getRow(2);

				Matrix Mat(Row_0.x, Row_1.x, Row_2.x, 0.f,
						   Row_0.y, Row_1.y, Row_2.y, 0.f,
						   Row_0.z, Row_1.z, Row_2.z, 0.f,
						   ActorPose.t.x / PHIS_MULTYPLIER, ActorPose.t.y / PHIS_MULTYPLIER, ActorPose.t.z / PHIS_MULTYPLIER, 1.f);

   				if (Mat == SourceActor->GetPivot()){
   					continue;
   				}
				SyncPhysicsInfo.insert(std::make_pair(SourceActor, Mat));
			}
		}
	}

	SyncActors();

}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::SyncActors()
{
	pCoreSDK->EnterCommandCS();

	if (SyncPhysicsInfo.size() > 0)
	{
		TMapSyncDataIterator Iter = SyncPhysicsInfo.begin();

		while (Iter != SyncPhysicsInfo.end())
		{

			Iter->first->SetPivot(Iter->second);
			Iter->first->RebuildTransform();

			//pCoreSDK->AddCommand(new CommandSetLocalTransform<CActor, CActor>(&CActor::SetPivot, Iter->first, Iter->second), true);
			//pCoreSDK->AddCommand(new Command<CActor, CActor>(&CActor::RebuildTransform, Iter->first), true);

			++Iter;
		}
		SyncPhysicsInfo.clear();
	}

	pCoreSDK->LeaveCommandCS();
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::DebugRender(CRenderSDK * RenderSDK)
{
	if (bRenderDebug && m_pPhisicsSDK)
	{
		if (m_pScene)
		{
			//Do this after advancing the simulation.    
			const NxDebugRenderable * dbgRenderable = m_pScene->getDebugRenderable();

			if (dbgRenderable)
			{
				NxU32 NbLines = dbgRenderable->getNbLines();
				const NxDebugLine * Lines = dbgRenderable->getLines();

				while(NbLines--) 
				{               
					RenderSDK->DrawLine(Vector(Lines->p0.x, Lines->p0.y, Lines->p0.z), 
										Vector(Lines->p1.x, Lines->p1.y, Lines->p1.z), 0xffffffff);	
					Lines++;            
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::SetPosition(const CActor * Owner)
{
	bool bResult = false;

	Lock();

	// Render all the actors in the scene
	size_t NumActors = m_pScene->getNbActors();
	NxActor** Actors = m_pScene->getActors();

	while (NumActors--)
	{
		NxActor * Actor = *Actors++;

		if (Actor->userData == Owner)
		{
			NxMat34 globalPose;

			Matrix LTM = Owner->GetPivot();

			globalPose.t = NxVec3(LTM.t.x, LTM.t.y, LTM.t.z) * PHIS_MULTYPLIER;

			Actor->setGlobalPose(globalPose);

			bResult = true;
			break;
		}
	}

	Unlock();

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::SetControllerPosition(const CActor * Sender, const Vector *Position)
{
	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)Sender)
			{
				Controller->setPosition(NxExtendedVec3(Position->x * PHIS_MULTYPLIER, Position->y * PHIS_MULTYPLIER, Position->z * PHIS_MULTYPLIER));
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::SetActorPosition(const class CActor *pSender, const Matrix *pWTM)
{
	bool bResult = false;

	if (m_pScene)
	{
		// Render all the actors in the scene
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				NxMat34 globalPose;

				globalPose.M.setRow(0, NxVec3(WTM._11, WTM._21, WTM._31));
				globalPose.M.setRow(1, NxVec3(WTM._12, WTM._22, WTM._32));
				globalPose.M.setRow(2, NxVec3(WTM._13, WTM._23, WTM._33));
				globalPose.t = NxVec3(WTM.t.x * PHIS_MULTYPLIER, WTM.t.y * PHIS_MULTYPLIER, WTM.t.z * PHIS_MULTYPLIER);

				Actor->setGlobalPose(globalPose);
				Actor->setLinearVelocity(NxVec3(0.f, 0.f, 0.f));
				Actor->setAngularVelocity(NxVec3(0.f, 0.f, 0.f));

				bResult = true;
				break;
			}
		}
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::ReleaseObject(const CActor * Owner)
{
	if (m_pScene) // Release actor if present 
	{
		// check for persistent in sync map object and erase if persist
		TMapSyncDataIterator Iter = SyncPhysicsInfo.find(const_cast<CActor*>(Owner));

		if (Iter != SyncPhysicsInfo.end()){
			SyncPhysicsInfo.erase(Iter);
		}
	
		ReleaseController(Owner);

		// Render all the actors in the scene
		NxU32 NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Owner)
			{
 				NxU32 NumShapes = Actor->getNbShapes();
 				NxShape *const * Shapes = Actor->getShapes();
 
 				while (NumShapes--)
 				{
 					NxShape * Shape = *Shapes++;
 					delete Shape->userData;
 				}
  				m_pScene->releaseActor(*Actor);
				break;
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::ReleaseController(const CActor * Sender)
{
	bool bResult = false;

	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)Sender)
			{
				// try to find movement data
				TMapMovementDataIterator IterFind = MMovementData.find(Controller);
				if (IterFind != MMovementData.end())
				{
					MMovementData.erase(IterFind);
					m_pControllerManager->releaseController(*Controller);
					bResult = true;
				}
				break;
			}
		}
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateTriangleMesh(const CActor *pOwner,
											const Vector *pVecArr,
											size_t nSize,
											const unsigned int *pIndexes,
											size_t nIdxSize)
{

	assert(m_pPhisicsSDK && m_pScene);
	
	if (Verts.size() == 0 || Indices.size() == 0){
		return false;
	}
	// release previous actor
	ReleaseObject(Owner);

	Lock();

	// prepare data for physics
	for (std::vector<NxVec3>::iterator Iter = Verts.begin(); Iter != Verts.end(); ++Iter)
	{
		(*Iter).x *= PHIS_MULTYPLIER;
		(*Iter).y *= -PHIS_MULTYPLIER;
		(*Iter).z *= PHIS_MULTYPLIER;
	}

	// allocate new
	NxTriangleMeshDesc * TriangleMeshDesc = new NxTriangleMeshDesc();

	assert(TriangleMeshDesc);

	TriangleMeshDesc->numVertices		  = (NxU32)Verts.size();
	TriangleMeshDesc->pointStrideBytes	  = sizeof(NxVec3);
	TriangleMeshDesc->points			  = &Verts[0];
	TriangleMeshDesc->numTriangles		  = (NxU32)Indices.size() / 3;
	TriangleMeshDesc->flags				  = 0;
	TriangleMeshDesc->triangles			  = &Indices[0];
	TriangleMeshDesc->triangleStrideBytes = 3 * sizeof(NxU32);
	
	MemoryWriteBuffer buf;
	NxTriangleMesh * pMesh;
	
	{
		NxInitCooking();
		bool status = NxCookTriangleMesh(*TriangleMeshDesc, buf);

		if (status){
			pMesh = m_pPhisicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
		}else{
			assert(false);
			pMesh = NULL;
		}
		NxCloseCooking();
	}

	NxTriangleMeshShapeDesc tmsd;
	tmsd.meshData		= pMesh;
	tmsd.userData		= TriangleMeshDesc;
	tmsd.localPose.t	= NxVec3(0.f, 0.f, 0.f);
	tmsd.meshPagingMode = NX_MESH_PAGING_AUTO;
	tmsd.group			= GROUP_COLLIDABLE_NON_PUSHABLE;
	//tmsd.skinWidth		= 0.02f; //SKINWIDTH;

	NxActorDesc actorDesc;
	NxBodyDesc  bodyDesc;

	assert(tmsd.isValid());
	actorDesc.shapes.pushBack(&tmsd);
	actorDesc.body		   = NULL;		// Dynamic triangle mesh don't be supported anymore. So body = NULL
	actorDesc.globalPose.t = NxVec3(0.f, 0.f, 0.f);
	
	if (pMesh)
	{
		assert(actorDesc.isValid());

		NxActor * Actor = m_pScene->createActor(actorDesc);
		Actor->userData = (void*)Owner;
		assert(Actor);
	}
	
	Unlock();
	
	return true;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateBoxController(const CActor * Sender,
										const Vector *Position,
										const Vector *Extends)
{	
	Lock();

	if (m_pControllerManager)
	{
		ReleaseController(Sender);

		NxBoxControllerDesc desc;

		desc.position		= NxExtendedVec3(Position.x * PHIS_MULTYPLIER, Position.y * PHIS_MULTYPLIER, Position.z * PHIS_MULTYPLIER);
		desc.extents		= NxVec3(Extends.x / 2, Extends.y / 2, Extends.z / 2) * PHIS_MULTYPLIER;
		desc.upDirection	= NX_Y;
		desc.slopeLimit		= 0.f;
		desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
		//desc.skinWidth		= 0.02f; //SKINWIDTH;
		desc.stepOffset		= 0.5f;
		desc.userData		= (void*)Sender;
		desc.callback		= m_pControllerReport;
		
		NxController * Controller = m_pControllerManager->createController(m_pScene, desc);
		assert(Controller);

		MovementData Data;
		MMovementData.insert(std::make_pair(Controller, Data));
	}
	Unlock();
	
	return true;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateBox(const CActor * Sender, 
							  const Matrix* pWTM, 
							  const Vector* pExtends, 
							  bool bStatic,
							  float Density,
							  bool DISABLE_GRAVITY,
							  bool FROZEN_POS_X,
							  bool FROZEN_POS_Y,
							  bool FROZEN_POS_Z,
							  bool FROZEN_ROT_X,   
							  bool FROZEN_ROT_Y,   
							  bool FROZEN_ROT_Z,
							  bool FROZEN_POS,
							  bool FROZEN_ROT,
							  bool KINEMATIC)
{
	bool bResult = false;

	Lock();

	if (m_pScene)
	{
		ReleaseObject(Sender);

		NxActorDesc ActorDesc;
		NxBodyDesc	BodyDesc;

		NxBoxShapeDesc BoxShapeDesc;

		Vector HalfExt = Extends * PHIS_MULTYPLIER * 0.5f;

		BoxShapeDesc.dimensions.set(HalfExt.x, HalfExt.y, HalfExt.z);
		ActorDesc.shapes.pushBack(&BoxShapeDesc);

		ActorDesc.body = (!bStatic) ? &BodyDesc : NULL;
		ActorDesc.density = Density;

 		ActorDesc.globalPose.M.setRow(0, NxVec3(WTM._11, WTM._21, WTM._31));
 		ActorDesc.globalPose.M.setRow(1, NxVec3(WTM._12, WTM._22, WTM._32));
 		ActorDesc.globalPose.M.setRow(2, NxVec3(WTM._13, WTM._23, WTM._33));
		ActorDesc.globalPose.t = NxVec3(WTM.t.x, WTM.t.y, WTM.t.z) * PHIS_MULTYPLIER;

		assert(ActorDesc.isValid());

		NxActor * pActor = m_pScene->createActor(ActorDesc);
		assert(pActor);

		SetActorFlags(pActor,
					bStatic,
					DISABLE_GRAVITY,
					FROZEN_POS_X,
					FROZEN_POS_Y,
					FROZEN_POS_Z,
					FROZEN_ROT_X,   
					FROZEN_ROT_Y,   
					FROZEN_ROT_Z,
					FROZEN_POS,
					FROZEN_ROT,
					KINEMATIC);

		pActor->userData = (void*)Sender;

		bResult = true;
	}

	Unlock();

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateSphere(const CActor * Sender, 
							  const Matrix *pWTM, 
							  const float Radius,
							  bool bStatic,
							  float Density,
							  bool DISABLE_GRAVITY,
							  bool FROZEN_POS_X,
							  bool FROZEN_POS_Y,
							  bool FROZEN_POS_Z,
							  bool FROZEN_ROT_X,   
							  bool FROZEN_ROT_Y,   
							  bool FROZEN_ROT_Z,
							  bool FROZEN_POS,
							  bool FROZEN_ROT,
							  bool KINEMATIC)
{
	bool bResult = false;

	Lock();

	if (m_pScene)
	{
		ReleaseObject(Sender);

		NxActorDesc ActorDesc;
		NxBodyDesc	BodyDesc;
		NxSphereShapeDesc SphereShapeDesc;

		SphereShapeDesc.radius = Radius * PHIS_MULTYPLIER;
		ActorDesc.shapes.pushBack(&SphereShapeDesc);

		ActorDesc.body = (!bStatic) ? &BodyDesc : NULL;
		ActorDesc.density = Density;

		ActorDesc.globalPose.M.setRow(0, NxVec3(WTM._11, WTM._21, WTM._31));
		ActorDesc.globalPose.M.setRow(1, NxVec3(WTM._12, WTM._22, WTM._32));
		ActorDesc.globalPose.M.setRow(2, NxVec3(WTM._13, WTM._23, WTM._33));
		ActorDesc.globalPose.t = NxVec3(WTM.t.x, WTM.t.y, WTM.t.z) * PHIS_MULTYPLIER;

		assert(ActorDesc.isValid());

		NxActor * pActor = m_pScene->createActor(ActorDesc);
		assert(pActor);

		SetActorFlags(pActor,
					bStatic,
					DISABLE_GRAVITY,
					FROZEN_POS_X,
					FROZEN_POS_Y,
					FROZEN_POS_Z,
					FROZEN_ROT_X,   
					FROZEN_ROT_Y,   
					FROZEN_ROT_Z,
					FROZEN_POS,
					FROZEN_ROT,
					KINEMATIC);

		pActor->userData = (void*)Sender;

		bResult = true;
	}
	Unlock();

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateBoxTrigger(const TriggerObject *Sender,
									 const Matrix *pWTM, 
									 const Vector *pExtends,
									 TriggerShapeFlag flag)
{
	bool bResult = false;

	Lock();

	if (m_pScene)
	{
		ReleaseObject(Sender);

		NxActorDesc ActorDesc;
		NxBoxShapeDesc BoxShapeDesc;

		Vector HalfExt = Extends * PHIS_MULTYPLIER * 0.5f;

		BoxShapeDesc.dimensions.set(HalfExt.x, HalfExt.y, HalfExt.z);
		
		if (flag & PHYS_TRIGGER_ON_ENTER){
			BoxShapeDesc.shapeFlags |= NX_TRIGGER_ON_ENTER;
		}
		if (flag & NX_TRIGGER_ON_STAY){
			BoxShapeDesc.shapeFlags |= NX_TRIGGER_ON_STAY;
		}
		if (flag & PHYS_TRIGGER_ON_LEAVE){
			BoxShapeDesc.shapeFlags |= NX_TRIGGER_ON_LEAVE;
		}

		ActorDesc.shapes.pushBack(&BoxShapeDesc);
		ActorDesc.body = NULL;

		ActorDesc.globalPose.M.setRow(0, NxVec3(WTM._11, WTM._21, WTM._31));
		ActorDesc.globalPose.M.setRow(1, NxVec3(WTM._12, WTM._22, WTM._32));
		ActorDesc.globalPose.M.setRow(2, NxVec3(WTM._13, WTM._23, WTM._33));
		ActorDesc.globalPose.t = NxVec3(WTM.t.x, WTM.t.y, WTM.t.z) * PHIS_MULTYPLIER;

		assert(ActorDesc.isValid());

		NxActor * pActor = m_pScene->createActor(ActorDesc);
		assert(pActor);

		pActor->userData = (void*)Sender;

		bResult = true;
	}

	Unlock();

	return bResult;
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::SetActorFlags(NxActor * pActor,
								  bool bStatic,
								  bool DISABLE_GRAVITY,
								  bool FROZEN_POS_X,
								  bool FROZEN_POS_Y,
								  bool FROZEN_POS_Z,
								  bool FROZEN_ROT_X,   
								  bool FROZEN_ROT_Y,   
								  bool FROZEN_ROT_Z,
								  bool FROZEN_POS,
								  bool FROZEN_ROT,
								  bool KINEMATIC)
{
	if (!bStatic && DISABLE_GRAVITY){
		pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	}
	if (!bStatic && FROZEN_POS_X){
		pActor->raiseBodyFlag(NX_BF_FROZEN_POS_X);
	}
	if (!bStatic && FROZEN_POS_Y){
		pActor->raiseBodyFlag(NX_BF_FROZEN_POS_Y);
	}
	if (!bStatic && FROZEN_POS_Z){
		pActor->raiseBodyFlag(NX_BF_FROZEN_POS_Z); //2D
	}
	if (!bStatic && FROZEN_ROT_X){
		pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_X); //2D
	}
	if (!bStatic && FROZEN_ROT_Y){
		pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_Y); //2D
	}
	if (!bStatic && FROZEN_ROT_Z){
		pActor->raiseBodyFlag(NX_BF_FROZEN_ROT_Z);
	}
	if (!bStatic && FROZEN_POS){
		pActor->raiseBodyFlag(NX_BF_FROZEN_POS);
	}
	if (!bStatic && FROZEN_ROT){
		pActor->raiseBodyFlag(NX_BF_FROZEN_ROT);
	}
	if (!bStatic && KINEMATIC){
		pActor->raiseBodyFlag(NX_BF_KINEMATIC);
	}
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::SetCharacterMovement(const CActor *pSender, Vector *pDisplace, float Speed )//= 1.f
{
	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)Sender)
			{
				// try to find movement data
				TMapMovementDataIterator Iter = MMovementData.find(Controller);

				if (Iter != MMovementData.end())
				{
					Iter->second.vDisplace = displace;
					Iter->second.CharacterSpeed = Speed;
					return true;
				}
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::SetCharacterMovementSpeed(const CActor *pSender, float Speed)
{
	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)pSender)
			{
				// try to find movement data
				TMapMovementDataIterator Iter = MMovementData.find(Controller);

				if (Iter != MMovementData.end())
				{
					Iter->second.CharacterSpeed = Speed;
					return true;
				}
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CharacterJump(const CActor * Sender, float Height)
{
	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)Sender)
			{
				// try to find movement data
				TMapMovementDataIterator Iter = MMovementData.find(Controller);

				if (Iter != MMovementData.end())
				{
					Iter->second.StartJump(Height);
					return true;
				}
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
float PhysicsEnginePhysX::Helper_GetHeight(const CActor * Sender)
{
	if (m_pControllerManager)
	{
		size_t NumControllers = m_pControllerManager->getNbControllers();

		for (size_t Index = 0; Index < NumControllers; ++Index)
		{
			NxController * Controller = m_pControllerManager->getController((NxU32)Index);

			if (Controller && Controller->getUserData() == (void*)Sender)
			{
				// try to find movement data
				TMapMovementDataIterator Iter = MMovementData.find(Controller);

				if (Iter != MMovementData.end())
				{
					return Iter->second.Height;
				}
			}
		}
	}
	return 0.f;
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::ReleaseNxActor(NxActor *pActor)
{
	if (m_pScene)
	{
		size_t NumShapes = actor->getNbShapes();
		NxShape *const * Shapes = actor->getShapes();

		while (NumShapes--)
		{
			NxShape * Shape = *Shapes++;
			delete Shape->userData;
		}

		m_pScene->releaseActor(*actor);
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addForceAtPos(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, NxVec3(pos.x, pos.y, pos.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addForceAtLocalPos(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, NxVec3(pos.x, pos.y, pos.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddLocalForceAtPos(const class CActor *pSender, 
											const Vector *pForce,
											const Vector *pos,
											EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addLocalForceAtPos(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, NxVec3(pos.x, pos.y, pos.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
	
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddLocalForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode flag)
{
	
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addLocalForceAtLocalPos(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, NxVec3(pos.x, pos.y, pos.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
	
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddForce(const class CActor *pSender, const Vector *pForce, EForceMode flag)
{
	
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addForce(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddLocalForce(const class CActor *pSender, const Vector *pForce, EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addLocalForce(NxVec3(force.x, force.y, force.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddTorque(const class CActor *pSender, const Vector *pTorque, EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addTorque(NxVec3(torque.x, torque.y, torque.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::AddLocalTorque(const class CActor *pSender, const Vector *pTorque, EForceMode flag)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->addLocalTorque(NxVec3(torque.x, torque.y, torque.z) * PHIS_MULTYPLIER, (NxForceMode)flag);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
void PhysicsEnginePhysX::SetLinearVelocity(const class CActor *pSender, const Vector *pValue)
{
	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				Actor->setLinearVelocity(NxVec3(pValue->x, pValue->y, pValue->z) * PHIS_MULTYPLIER);
				break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
Vector* PhysicsEnginePhysX::GetLinearVelocity(const class CActor *pSender) const
{
	Vector OutValue(0.f, 0.f, 0.f);

	if (m_pScene)
	{
		size_t NumActors = m_pScene->getNbActors();
		NxActor** Actors = m_pScene->getActors();

		while (NumActors--)
		{
			NxActor * Actor = *Actors++;

			if (Actor->userData == Sender)
			{
				NxVec3 velocity = Actor->getLinearVelocity();

				OutValue = Vector(velocity.x, velocity.y, velocity.z) / PHIS_MULTYPLIER;
				break;
			}
		}
	}
	return 0; //OutValue;
}
*/