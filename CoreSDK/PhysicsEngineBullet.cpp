#include "coresdkafx.h"
#include <btBulletDynamicsCommon.h>

#ifdef PHYS_ENABLED

//----------------------------------------------------------------------------------------------
CPhysicsEngineBullet::CPhysicsEngineBullet(CCoreSDK *pSDK)
: m_pBroadPhase(0)
, m_pDispatcher(0)
, m_pConstraintSolver(0)
, m_pCollisionConfiguration(0)
, m_pDynamicsWorld(0)
, m_bRenderDebug(false)
, m_pCoreSDK(pSDK)
{
	 //m_pCollisionShapes = new 
}

//----------------------------------------------------------------------------------------------
CPhysicsEngineBullet::~CPhysicsEngineBullet()
{

}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::Initialize()
{
	m_pBroadPhase = new btDbvtBroadphase();
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	m_pConstraintSolver = new btSequentialImpulseConstraintSolver();
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadPhase, m_pConstraintSolver, m_pCollisionConfiguration);
	
	m_pDebugDrawer = new CPhysDebugDrawerGL(m_pCoreSDK);
	
	m_pDebugDrawer->setDebugMode(1);

	m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);

	m_pDynamicsWorld->setGravity(btVector3(0.f, -10.f, 0.f));

	return true;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::Release()
{
	delete m_pDynamicsWorld;
	delete m_pConstraintSolver;
	delete m_pBroadPhase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
	delete m_pDebugDrawer;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::Simulate(float fElapsedTime)
{
	//Lock();

	//simple dynamics world doesn't handle fixed-time-stepping
	//float deltaTime = fElapsedTime / 1000.f;
	
	if (m_pDynamicsWorld)
	{
		m_pDynamicsWorld->stepSimulation(fElapsedTime, 7);
	}

	//Unlock();
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SimulateCharacters(float fElapsedTime)
{
}
	
//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::PostSimulateUpdate()
{
	CViewportManager *pViewMgr = m_pCoreSDK->GetViewportManager();

	btCollisionObjectArray &refArray = m_pDynamicsWorld->getCollisionObjectArray();

	size_t Num = refArray.size();

	for (size_t Index = 0; Index < Num; ++Index)
	{
		btCollisionObject *pColObj = refArray[Index];

		if (pColObj->isStaticObject()){
			continue;
		}

		const btTransform &worldTransform = pColObj->getWorldTransform();
		const btCollisionShape *pShape = pColObj->getCollisionShape();

		const btVector3 &wtmRow0 = worldTransform.getBasis().getRow(0);
		const btVector3 &wtmRow1 = worldTransform.getBasis().getRow(1);
		const btVector3 &wtmRow2 = worldTransform.getBasis().getRow(2);

		Matrix WTM(wtmRow0[0], wtmRow1[0], wtmRow2[0], 0.f,
					wtmRow0[1], wtmRow1[1], wtmRow2[1], 0.f,
					wtmRow0[2], wtmRow1[2], wtmRow2[2], 0.f,
					worldTransform.getOrigin().getX(),
					worldTransform.getOrigin().getY(),
					worldTransform.getOrigin().getZ(),
					1.f);
		
		if (pShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			const btCompoundShape *pCompoundShape = static_cast<const btCompoundShape*>(pShape);

			for (int Index2 = pCompoundShape->getNumChildShapes() - 1; Index2 >= 0; --Index2)
			{
				btTransform childTrans = pCompoundShape->getChildTransform(Index2);

				const btCollisionShape *pColShape = pCompoundShape->getChildShape(Index2);
				
				if (pColShape)
				{
					if (void *pUserData = pColShape->getUserPointer())
					{
						const btVector3 &ltmRow0 = childTrans.getBasis().getRow(0);
						const btVector3 &ltmRow1 = childTrans.getBasis().getRow(1);
						const btVector3 &ltmRow2 = childTrans.getBasis().getRow(2);

						Matrix LTM(ltmRow0[0], ltmRow1[0], ltmRow2[0], 0.f,
									ltmRow0[1], ltmRow1[1], ltmRow2[1], 0.f,
									ltmRow0[2], ltmRow1[2], ltmRow2[2], 0.f,
									childTrans.getOrigin().getX(),
									childTrans.getOrigin().getY(),
									childTrans.getOrigin().getZ(),
									1.f);

						m_SyncData.insert(std::make_pair(static_cast<CActor*> (pUserData), WTM * LTM));

						break;
					}
				}
			}
		}
		else
		{
			if (void *pUserData = pShape->getUserPointer())
			{
				m_SyncData.insert(std::make_pair(static_cast<CActor*> (pUserData), WTM));
			}
		}
	}

	SyncActors();

	ProcessContacts();
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::DebugRender(class CRenderSDK *pRenderSDK)
{
	if (m_bRenderDebug && m_pDynamicsWorld)
	{
		m_pDynamicsWorld->debugDrawWorld();
	}
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetPosition(const class CActor *pSender)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetControllerPosition(const class CActor *pSender, const Vector *pPosition)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetActorPosition(const CActor *pSender, const Matrix *pWTM)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetActorPositionNode(const SPhysNode *pPhysNode, const Matrix *pWTM)
{
	assert(pPhysNode && pPhysNode->pRigidBody);

	btTransform startTransform;
	startTransform.setIdentity();

	{
		btMatrix3x3 btBasis((*pWTM)[0], (*pWTM)[4], (*pWTM)[8],
							(*pWTM)[1], (*pWTM)[5], (*pWTM)[9],
							(*pWTM)[2], (*pWTM)[6], (*pWTM)[10]);
		startTransform.setBasis(btBasis);
	}

	startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

	pPhysNode->pRigidBody->setWorldTransform(startTransform);

	return true;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::ReleaseObject(const class CActor *pSender)
{	
	bool bRemove = false;

	btCollisionObjectArray &refArray = m_pDynamicsWorld->getCollisionObjectArray();

	size_t Num = refArray.size();

	for (size_t Index = 0; Index < Num; ++Index)
	{
		btCollisionObject *pColObj = refArray[Index];

		btCollisionShape *pShape = pColObj->getCollisionShape();

		if (pShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			btCompoundShape *pCompoundShape = static_cast<btCompoundShape*>(pShape);

			for (int Index2 = pCompoundShape->getNumChildShapes() - 1; Index2 >= 0; --Index2)
			{
				btTransform childTrans = pCompoundShape->getChildTransform(Index2);

				btCollisionShape *pColShape = pCompoundShape->getChildShape(Index2);

				if (pSender == pColShape->getUserPointer())
				{
					m_CollisionShapes.remove(pColShape);
					delete pColShape;
					bRemove = true;
				}
			}

			m_CollisionShapes.remove(pShape);
			delete pShape;
		}
		else
		{
			if (pSender == pShape->getUserPointer())
			{
				m_CollisionShapes.remove(pShape);
				delete pShape;
				bRemove = true;
			}
		}

		if (bRemove)
		{
			btRigidBody *pBody = btRigidBody::upcast(pColObj);

			if (pBody && pBody->getMotionState()){
				delete pBody->getMotionState();
			}

			m_pDynamicsWorld->removeCollisionObject(pColObj);
			delete pColObj;

			break;
		}
	}
	return bRemove;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::ReleaseObject(const SPhysNode *pSender)
{
	bool bResult = false;

	if (pSender)
	{
		for (TListPhysNode::iterator Iter = m_ListPhysNodes.begin(); Iter != m_ListPhysNodes.end(); ++Iter)
		{
			if ((*Iter)->pRigidBody == pSender->pRigidBody && (*Iter)->pActor == pSender->pActor)
			{
				m_ListPhysNodes.remove(*Iter);
				break;
			}
		}

		if (pSender->pRigidBody)
		{
			btCollisionShape *pShape = pSender->pRigidBody->getCollisionShape();

			if (pShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
			{
				btCompoundShape *pCompoundShape = static_cast<btCompoundShape*>(pShape);

				for (int Index2 = pCompoundShape->getNumChildShapes() - 1; Index2 >= 0; --Index2)
				{
					btTransform childTrans = pCompoundShape->getChildTransform(Index2);
					btCollisionShape *pColShape = pCompoundShape->getChildShape(Index2);

					m_CollisionShapes.remove(pColShape);
					delete pColShape;
				}

				m_CollisionShapes.remove(pShape);
				delete pShape;
			}
			else
			{
				m_CollisionShapes.remove(pShape);
				delete pShape;
			}

			if (pSender->pRigidBody && pSender->pRigidBody->getMotionState()){
				delete pSender->pRigidBody->getMotionState();
			}

			m_pDynamicsWorld->removeCollisionObject(pSender->pRigidBody);
			delete pSender->pRigidBody;
		}

		delete pSender;
	
		bResult = true;
	}	
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::ReleaseController(const class CActor *pSender)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreateTriangleMesh(const class CActor *pOwner,
						const Vector *pVecArr,
						size_t nSize,
						const unsigned int *pIndexes,
						size_t nIdxSize)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreateBoxController(const class CActor *pOwner, const Vector *pPosition, const Vector *pExtends)
{
	return false;
}
	
//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreateBox(const class CActor *pSender,
							const Matrix *pWTM, 
							const Vector *pExtends,
							bool bStatic,
							float density,
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
	btBoxShape *pColShape = new btBoxShape(btVector3((*pExtends).x * 0.5f, (*pExtends).y * 0.5f, (*pExtends).z * 0.5f));

	m_CollisionShapes.push_back(pColShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar fMass((*pExtends).x * (*pExtends).y * (*pExtends).z * density);

	if (fMass == 0.f || bStatic){
		fMass = 0.f;
	}

	btVector3 fLocalInertia(0.f, 0.f, 0.f);
	if (fMass != 0.f){
		pColShape->calculateLocalInertia(fMass, fLocalInertia);
	}

	startTransform.setOrigin(btVector3(btScalar((*pWTM).t.x), btScalar((*pWTM).t.y), btScalar((*pWTM).t.z)));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(fMass, myMotionState, pColShape, fLocalInertia);

	btRigidBody* pRigidBody = new btRigidBody(rbInfo);

	pRigidBody->setActivationState(DISABLE_DEACTIVATION);

	pRigidBody->setLinearFactor(btVector3(FROZEN_POS_X ? 0.f : 1.f, FROZEN_POS_Y ? 0.f : 1.f, FROZEN_POS_Z ? 0.f : 1.f));

	pRigidBody->setAngularFactor(btVector3(FROZEN_ROT_X ? 0.f : 1.f, FROZEN_ROT_Y ? 0.f : 1.f, FROZEN_ROT_Z ? 0.f : 1.f));

	pRigidBody->setUserPointer((void*)pSender);

	m_pDynamicsWorld->addRigidBody(pRigidBody);

	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreateSphere(const class CActor *pSender, 
								const Matrix *pWTM, 
								const float fRad,
								bool bStatic,
								float density,
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
	btCollisionShape *pColShape = new btSphereShape((btScalar)fRad);

	m_CollisionShapes.push_back(pColShape);

	btScalar fMass(1.3333f * PI_CONST * (fRad * fRad * fRad) * density);

	if (fMass == 0.f || bStatic){
		fMass = 0.f;
	}

	bool isDynamic = (fMass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic) {
		pColShape->calculateLocalInertia(fMass, localInertia);
	}

	// use first shape as basic for actor
	pColShape->setUserPointer((void*)pSender);

	btRigidBody *pBody = new btRigidBody(fMass, 0, pColShape, localInertia);	

	btTransform startTransform;
	startTransform.setIdentity();

	{
		btMatrix3x3 btBasis((*pWTM)[0], (*pWTM)[4], (*pWTM)[8],
			(*pWTM)[1], (*pWTM)[5], (*pWTM)[9],
			(*pWTM)[2], (*pWTM)[6], (*pWTM)[10]);

		startTransform.setBasis(btBasis);
	}

	startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

	pBody->setWorldTransform(startTransform);

	pBody->setActivationState(DISABLE_DEACTIVATION);

	pBody->setLinearFactor(btVector3(FROZEN_POS_X ? 0.f : 1.f, FROZEN_POS_Y ? 0.f : 1.f, FROZEN_POS_Z ? 0.f : 1.f));

	pBody->setAngularFactor(btVector3(FROZEN_ROT_X ? 0.f : 1.f, FROZEN_ROT_Y ? 0.f : 1.f, FROZEN_ROT_Z ? 0.f : 1.f));

	m_pDynamicsWorld->addRigidBody(pBody);

	if (DISABLE_GRAVITY && !bStatic)
	{
		btVector3 vZero(0.f, 0.f, 0.f);
		pBody->setGravity(vZero);
	}

	return true;
}

//----------------------------------------------------------------------------------------------
SPhysNode* CPhysicsEngineBullet::CreateSphere(const CActor *pSender, const Matrix *pWTM, const SSphereBodyDesc &Desc)
{
	btCollisionShape *pColShape = new btSphereShape((btScalar)Desc.fRadius);

	m_CollisionShapes.push_back(pColShape);

	btScalar fMass(1.3333f * PI_CONST * (Desc.fRadius * Desc.fRadius * Desc.fRadius) * Desc.fDensity);

	if (fMass == 0.f || Desc.bStatic){
		fMass = 0.f;
	}

	bool isDynamic = (fMass != 0.f);

	btVector3 localInertia(0.f, 0.f, 0.f);
	if (isDynamic) {
		pColShape->calculateLocalInertia(fMass, localInertia);
	}

	// use first shape as basic for actor
	pColShape->setUserPointer((void*)pSender);

	btRigidBody *pBody = new btRigidBody(fMass, 0, pColShape, localInertia);	

	btTransform startTransform;
	startTransform.setIdentity();

	{
		btMatrix3x3 btBasis((*pWTM)[0], (*pWTM)[4], (*pWTM)[8],
							(*pWTM)[1], (*pWTM)[5], (*pWTM)[9],
							(*pWTM)[2], (*pWTM)[6], (*pWTM)[10]);

		startTransform.setBasis(btBasis);
	}

	startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

	pBody->setWorldTransform(startTransform);

	pBody->setActivationState(DISABLE_DEACTIVATION);

	pBody->setLinearFactor(btVector3(Desc.FROZEN_POS_X ? 0.f : 1.f, Desc.FROZEN_POS_Y ? 0.f : 1.f, Desc.FROZEN_POS_Z ? 0.f : 1.f));

	pBody->setAngularFactor(btVector3(Desc.FROZEN_ROT_X ? 0.f : 1.f, Desc.FROZEN_ROT_Y ? 0.f : 1.f, Desc.FROZEN_ROT_Z ? 0.f : 1.f));
		
	m_pDynamicsWorld->addRigidBody(pBody);

	if (Desc.DISABLE_GRAVITY && !Desc.bStatic)
	{
		btVector3 vZero(0.f, 0.f, 0.f);
		pBody->setGravity(vZero);
	}

	SPhysNode *pNode = new SPhysNode(const_cast<CActor*>(pSender), pBody, Desc.pCallback);
	m_ListPhysNodes.push_back(pNode);

	return pNode;
}

//----------------------------------------------------------------------------------------------
struct SBTObjInfo 
{
	btCollisionShape	*pShape;
	Matrix				LTM;
	float				fMass;
};

//----------------------------------------------------------------------------------------------
SPhysNode* CPhysicsEngineBullet::CreateActorFromFile(const char *filename,
												const CActor *pSender,
												const Matrix *pWTM,
												bool bStatic,
												float density,
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

	std::string sFilename;
	if (FindFile(filename, &sFilename))
	{
		std::vector<SBTObjInfo> localShapesInfos;

		XML_PARSE_FILE(sFilename)
		{
			XML_CHECK_NODE("col")
			{
				Matrix BaseLTM;
				{
					XML_DEF_ATTRIBUTES_NODE(LTM);
										
					CStringUtility<float> ParseBaseLTM(LTM, ";");

					if (ParseBaseLTM.m_vector.size() == 16)
					{
						BaseLTM._11 = ParseBaseLTM.m_vector[0];
						BaseLTM._12 = ParseBaseLTM.m_vector[1];
						BaseLTM._13 = ParseBaseLTM.m_vector[2];
						BaseLTM._14 = ParseBaseLTM.m_vector[3];
						BaseLTM._21 = ParseBaseLTM.m_vector[4];
						BaseLTM._22 = ParseBaseLTM.m_vector[5];
						BaseLTM._23 = ParseBaseLTM.m_vector[6];
						BaseLTM._24 = ParseBaseLTM.m_vector[7];
						BaseLTM._31 = ParseBaseLTM.m_vector[8];
						BaseLTM._32 = ParseBaseLTM.m_vector[9];
						BaseLTM._33 = ParseBaseLTM.m_vector[10];
						BaseLTM._34 = ParseBaseLTM.m_vector[11];
						BaseLTM._41 = ParseBaseLTM.m_vector[12];
						BaseLTM._42 = ParseBaseLTM.m_vector[13];
						BaseLTM._43 = ParseBaseLTM.m_vector[14];
						BaseLTM._44 = ParseBaseLTM.m_vector[15];
					}
				}

				XML_FOR_EACH_TREE(XML_CURRENT_NODE)
				{
					XML_CHECK_NODE("shape")
					{
						XML_DEF_ATTRIBUTES_NODE(type);
						XML_DEF_ATTRIBUTES_NODE(LTM);

						Matrix fLTM;

						CStringUtility<float> ParseLTM(LTM, ";");

						if (ParseLTM.m_vector.size() == 16)
						{
							fLTM._11 = ParseLTM.m_vector[0];
							fLTM._12 = ParseLTM.m_vector[1];
							fLTM._13 = ParseLTM.m_vector[2];
							fLTM._14 = ParseLTM.m_vector[3];
							fLTM._21 = ParseLTM.m_vector[4];
							fLTM._22 = ParseLTM.m_vector[5];
							fLTM._23 = ParseLTM.m_vector[6];
							fLTM._24 = ParseLTM.m_vector[7];
							fLTM._31 = ParseLTM.m_vector[8];
							fLTM._32 = ParseLTM.m_vector[9];
							fLTM._33 = ParseLTM.m_vector[10];
							fLTM._34 = ParseLTM.m_vector[11];
							fLTM._41 = ParseLTM.m_vector[12];
							fLTM._42 = ParseLTM.m_vector[13];
							fLTM._43 = ParseLTM.m_vector[14];
							fLTM._44 = ParseLTM.m_vector[15];
						}
						else { // Warning
						}

						SBTObjInfo ObjInfo;

						if (!stricmp(type.c_str(), "sphere"))
						{
							XML_DEF_ATTRIBUTES_NODE(Rad);

							CStringUtility<float> ParseRad(Rad, ";");
							if (ParseRad.m_vector.size() == 1)
							{
								float fRad = ParseRad.m_vector[0];
								if (fRad > 0.f)
								{
									ObjInfo.pShape = new btSphereShape((btScalar)fRad);
									ObjInfo.LTM = fLTM;
									ObjInfo.fMass = 1.3333f * PI_CONST * (fRad * fRad * fRad) * density;
								}
							}
						}
						else if (!stricmp(type.c_str(), "box"))
						{
							XML_DEF_ATTRIBUTES_NODE(Ext);

							CStringUtility<float> ParseExt(Ext, ";");
						
							btVector3 btExt(0.f, 0.f, 0.f);
							if (ParseExt.m_vector.size() == 3)
							{
								btExt[0] = ParseExt.m_vector[0];
								btExt[1] = ParseExt.m_vector[1];
								btExt[2] = ParseExt.m_vector[2];
							}else {
								// Warning
								continue;
							} 

							if (!btExt.isZero())
							{
								ObjInfo.pShape = new btBoxShape(btExt * 0.5f);
								ObjInfo.LTM = fLTM;
								ObjInfo.fMass = btExt[0] * btExt[1] * btExt[2] * density;
							}
						}
						else if (!stricmp(type.c_str(), "cylinder"))
						{}
						else if (!stricmp(type.c_str(), "capsule"))
						{}
						else
						{
							// TODO: Warning found nothing
						}

						localShapesInfos.push_back(ObjInfo);
					}
				}
			}


			// process deserialized data
			if (localShapesInfos.size() == 1) 
			{
				std::vector<SBTObjInfo>::iterator Iter = localShapesInfos.begin();

				btTransform localTrans;
				localTrans.setIdentity();

				float fTotalMass = localShapesInfos[0].fMass;

				if (fTotalMass == 0.f || bStatic){
					fTotalMass = 0.f;
				}

				btVector3 fLocalInertia(0.f, 0.f, 0.f);
				if (fTotalMass != 0.f){
					localShapesInfos[0].pShape->calculateLocalInertia(fTotalMass, fLocalInertia);
				}

				// use first shape as basic for actor
				localShapesInfos[0].pShape->setUserPointer((void*)pSender);

				btRigidBody *pBody = new btRigidBody(fTotalMass, 0, localShapesInfos[0].pShape, fLocalInertia);	
					
				btTransform startTransform;
				startTransform.setIdentity();

				{
					btMatrix3x3 btBasis((*Iter).LTM[0], (*Iter).LTM[4], (*Iter).LTM[8],
											(*Iter).LTM[1], (*Iter).LTM[5], (*Iter).LTM[9],
											(*Iter).LTM[2], (*Iter).LTM[6], (*Iter).LTM[10]);

					startTransform.setBasis(btBasis);
				}

				startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

				pBody->setWorldTransform(startTransform);

				pBody->setActivationState(DISABLE_DEACTIVATION);

				pBody->setLinearFactor(btVector3(FROZEN_POS_X ? 0.f : 1.f, FROZEN_POS_Y ? 0.f : 1.f, FROZEN_POS_Z ? 0.f : 1.f));

				pBody->setAngularFactor(btVector3(FROZEN_ROT_X ? 0.f : 1.f, FROZEN_ROT_Y ? 0.f : 1.f, FROZEN_ROT_Z ? 0.f : 1.f));
					
				m_pDynamicsWorld->addRigidBody(pBody);

				SPhysNode *pNode = new SPhysNode(const_cast<CActor*>(pSender), pBody);
				m_ListPhysNodes.push_back(pNode);

				return pNode;
			}
			else if (localShapesInfos.size() > 1) // compound
			{
				btCompoundShape *pCompound = new btCompoundShape();

				btTransform localTrans;
				localTrans.setIdentity();

				localShapesInfos[0].pShape->setUserPointer((void*)pSender);
					
				//localTrans effectively shifts the center of mass with respect to the chassis
				//localTrans.setOrigin(btVector3(0,1,0));

				float fTotalMass = 0.f;

				for (std::vector<SBTObjInfo>::iterator Iter = localShapesInfos.begin(); Iter != localShapesInfos.end(); ++Iter)
				{
					btTransform btLTM;
					btLTM.setIdentity();
					btMatrix3x3 btBasis((*Iter).LTM[0], (*Iter).LTM[4], (*Iter).LTM[8],
										(*Iter).LTM[1], (*Iter).LTM[5], (*Iter).LTM[9],
										(*Iter).LTM[2], (*Iter).LTM[6], (*Iter).LTM[10]);

					btLTM.setBasis(btBasis);
					btLTM.setOrigin(btVector3((*Iter).LTM[12], (*Iter).LTM[13], (*Iter).LTM[14]));

					pCompound->addChildShape(btLTM, (*Iter).pShape);
					fTotalMass += (*Iter).fMass;
				}

				if (fTotalMass == 0.f || bStatic){
					fTotalMass = 0.f;
				}

				btVector3 fLocalInertia(0.f, 0.f, 0.f);
				if (fTotalMass != 0.f){
					pCompound->calculateLocalInertia(fTotalMass, fLocalInertia);
				}

				btRigidBody *pBody = new btRigidBody(fTotalMass, 0, pCompound, fLocalInertia);	
					
				btTransform startTransform;

				startTransform.setIdentity();
				startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

				pBody->setWorldTransform(startTransform);

				pBody->setActivationState(DISABLE_DEACTIVATION);

				pBody->setLinearFactor(btVector3(FROZEN_POS_X ? 0.f : 1.f, FROZEN_POS_Y ? 0.f : 1.f, FROZEN_POS_Z ? 0.f : 1.f));

				pBody->setAngularFactor(btVector3(FROZEN_ROT_X ? 0.f : 1.f, FROZEN_ROT_Y ? 0.f : 1.f, FROZEN_ROT_Z ? 0.f : 1.f));

				m_pDynamicsWorld->addRigidBody(pBody);

				SPhysNode *pNode = new SPhysNode(const_cast<CActor*>(pSender), pBody);
				m_ListPhysNodes.push_back(pNode);

				return pNode;
			}
		}
		bResult = true;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreateBoxTrigger(const class TriggerObject *pSender,
									const Matrix *pWTM, 
									const Vector *pExtends,
									TriggerShapeFlag flag)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CreatePlane(const CActor *pSender, const Matrix *pWTM)
{
	bool bResult = false;

	btCollisionShape *pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
	pShape->setUserPointer((void*)pSender);

	m_CollisionShapes.push_back(pShape);
	
	btTransform Transform;
	Transform.setIdentity();

	Vector v0((*pWTM)[0], (*pWTM)[1], (*pWTM)[2]);
	Vector v1((*pWTM)[4], (*pWTM)[5], (*pWTM)[6]);
	Vector v2((*pWTM)[8], (*pWTM)[9], (*pWTM)[10]);

	v0.normalize();
	v1.normalize();
	v2.normalize();
	
	btMatrix3x3 btBasis(v0[0], v1[0], v2[0],
						v0[1], v1[1], v2[1],
						v0[2], v1[2], v2[2]);

	Transform.setBasis(btBasis);
	Transform.setOrigin(btVector3((*pWTM)[12], (*pWTM)[13], (*pWTM)[14]));

	btRigidBody *pBody = new btRigidBody(0.f, 0, pShape, btVector3(0.f, 0.f, 0.f));

	pBody->setWorldTransform(Transform);
	
	m_pDynamicsWorld->addRigidBody(pBody);

	bResult = true;
	
	return bResult;
}

//----------------------------------------------------------------------------------------------
SPhysNode* CPhysicsEngineBullet::CreateRestrictedVolume(const CActor *pSender, const Vector &Ext, const Matrix *pWTM, bool bSides[6])
{
	std::vector<SBTObjInfo> localShapesInfos;

	if (bSides[0]) // bottom
	{
		SBTObjInfo ObjInfo;
		Matrix ltm( 1.f, 0.f, 0.f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f);

		ObjInfo.LTM = (*pWTM) * ltm;
		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		localShapesInfos.push_back(ObjInfo);
	}

	if (bSides[1]) // top
	{ 
		SBTObjInfo ObjInfo;
		Matrix ltm(-1.f, 0.f, 0.f, 0.f,
					0.f, -1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, Ext.y, 0.f, 1.f);

		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		ObjInfo.LTM = (*pWTM) * ltm;
		localShapesInfos.push_back(ObjInfo);
	}

	if (bSides[2]) // X+
	{
		SBTObjInfo ObjInfo;
		Matrix ltm(0.f, 1.f, 0.f, 0.f,
				   -1.f, 0.f, 0.f, 0.f,
				    0.f, 0.f, 1.f, 0.f,
					Ext.x * 0.5f, 0.f, 0.f, 1.f);
		
		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		ObjInfo.LTM = (*pWTM) * ltm;
		localShapesInfos.push_back(ObjInfo);
	}

	if (bSides[3]) // X-
	{
		SBTObjInfo ObjInfo;
		Matrix ltm(0.f, -1.f, 0.f, 0.f,
					1.f, 0.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					-Ext.x * 0.5f, 0.f, 0.f, 1.f);

		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		ObjInfo.LTM = (*pWTM) * ltm;
		localShapesInfos.push_back(ObjInfo);
	}

	if (bSides[4]) // Z+
	{
		SBTObjInfo ObjInfo;
		Matrix ltm(1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, -1.f, 0.f,
			0.f, -1.f, 0.f, 0.f,
			0, 0.f, Ext.z * 0.5f, 1.f);

		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		ObjInfo.LTM = (*pWTM) * ltm;
		localShapesInfos.push_back(ObjInfo);
	}

	if (bSides[5]) // Z-
	{ 
		SBTObjInfo ObjInfo;
		Matrix ltm(1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, -1.f, 0.f, 0.f,
			0, 0.f, -Ext.z * 0.5f, 1.f);

		ObjInfo.pShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
		ObjInfo.LTM = (*pWTM) * ltm;
		localShapesInfos.push_back(ObjInfo);
	}

	if (localShapesInfos.size() == 0){
		return NULL;
	}

	btCompoundShape *pCompound = new btCompoundShape();

	btTransform localTrans;
	localTrans.setIdentity();

	localShapesInfos[0].pShape->setUserPointer((void*)pSender);

	for (std::vector<SBTObjInfo>::iterator Iter = localShapesInfos.begin(); Iter != localShapesInfos.end(); ++Iter)
	{
	/*	btTransform btLTM;
		btLTM.setIdentity();
		btMatrix3x3 btBasis((*Iter).LTM[0], (*Iter).LTM[4], (*Iter).LTM[8],
							(*Iter).LTM[1], (*Iter).LTM[5], (*Iter).LTM[9],
							(*Iter).LTM[2], (*Iter).LTM[6], (*Iter).LTM[10]);

		btLTM.setBasis(btBasis);
		btLTM.setOrigin(btVector3((*Iter).LTM[12], (*Iter).LTM[13], (*Iter).LTM[14]));*/

		btTransform Transform;
		Transform.setIdentity();

		Vector v0((*Iter).LTM[0], (*Iter).LTM[1], (*Iter).LTM[2]);
		Vector v1((*Iter).LTM[4], (*Iter).LTM[5], (*Iter).LTM[6]);
		Vector v2((*Iter).LTM[8], (*Iter).LTM[9], (*Iter).LTM[10]);

		v0.normalize();
		v1.normalize();
		v2.normalize();

		btMatrix3x3 btBasis(v0[0], v1[0], v2[0],
							v0[1], v1[1], v2[1],
							v0[2], v1[2], v2[2]);

		Transform.setBasis(btBasis);
		Transform.setOrigin(btVector3((*Iter).LTM[12], (*Iter).LTM[13], (*Iter).LTM[14]));

		pCompound->addChildShape(Transform, (*Iter).pShape);
	}

	btRigidBody *pBody = new btRigidBody(0.f, 0, pCompound, btVector3(0.f, 0.f, 0.f));

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(pWTM->t.x, pWTM->t.y, pWTM->t.z));

	pBody->setWorldTransform(startTransform);
	m_pDynamicsWorld->addRigidBody(pBody);

	SPhysNode *pNode = new SPhysNode(const_cast<CActor*>(pSender), pBody);
	m_ListPhysNodes.push_back(pNode);

	return pNode;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SetActorEnable(const struct SPhysNode *pPhysNode, bool bFlag)
{
	//pPhysNode->pRigidBody->activate()
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::GetActorEnable(const struct SPhysNode *pPhysNode) const
{
	bool bOutValue = false;

	return bOutValue;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetCharacterMovement(const CActor *pSender, Vector *displace, float Speed /*= 1.f*/)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::SetCharacterMovementSpeed(const CActor *pSender, float Speed)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool CPhysicsEngineBullet::CharacterJump(const CActor *pSender, float Height)
{
	return false;
}

// helper functions
//----------------------------------------------------------------------------------------------
float CPhysicsEngineBullet::Helper_GetHeight(const CActor *pSender)
{
	return 0.f;
}


//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode)
{
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode)
{
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddLocalForceAtPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode)
{
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddLocalForceAtLocalPos(const class CActor *pSender, const Vector *pForce, const Vector *pos, EForceMode mode)
{
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddForce(const class CActor *pSender, const Vector *pForce, EForceMode mode)
{
	btCollisionObjectArray &refArray = m_pDynamicsWorld->getCollisionObjectArray();

	const size_t Num = refArray.size();

	for (size_t Index = 0; Index < Num; ++Index)
	{
		btCollisionObject *pColObj = refArray[Index];

		if (pColObj->isStaticObject()){
			continue;
		}

		btCollisionShape *pShape = pColObj->getCollisionShape();

		if (pShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
		{
			btCompoundShape *pCompoundShape = static_cast<btCompoundShape*>(pShape);

			for (int Index2 = pCompoundShape->getNumChildShapes() - 1; Index2 >= 0; --Index2)
			{
				btTransform childTrans = pCompoundShape->getChildTransform(Index2);

				btCollisionShape *pColShape = pCompoundShape->getChildShape(Index2);
				
				if (pColShape)
				{
					if (void *pUserData = pColShape->getUserPointer())
					{
						if (pUserData == pSender)
						{
							btRigidBody *pBody = btRigidBody::upcast(pColObj);

							if (pBody){
								pBody->applyCentralForce(btVector3(pForce->x, pForce->y, pForce->z));
							}
							return;
						}
					}
				}
			}
		}
		else
		{
			if (void *pUserData = pShape->getUserPointer())
			{
				if (pUserData == pSender)
				{
					btRigidBody *pBody = btRigidBody::upcast(pColObj);

					if (pBody){
						pBody->applyCentralForce(btVector3(pForce->x, pForce->y, pForce->z));
					}
					return;
				}
			}
		}
	}

}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddLocalForce(const class CActor *pSender, const Vector *pForce, EForceMode mode)
{

}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode)
{

}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::AddLocalTorque(const class CActor *pSender, const Vector *pTorque, EForceMode mode)
{

}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SetCentralForce(const struct SPhysNode *pPhysNode, const Vector *pValue)
{
	assert(pPhysNode && pPhysNode->pRigidBody);
	pPhysNode->pRigidBody->applyCentralForce(btVector3(pValue->x, pValue->y, pValue->z));
}

//----------------------------------------------------------------------------------------------
Vector*	CPhysicsEngineBullet::GetCentralForce(const class CActor *pSender) const
{
	return 0;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SetCentralImpulse(const struct SPhysNode *pPhysNode, const Vector *pValue)
{
	assert(pPhysNode && pPhysNode->pRigidBody);
	pPhysNode->pRigidBody->applyCentralImpulse(btVector3(pValue->x, pValue->y, pValue->z));
}

//----------------------------------------------------------------------------------------------
Vector*	CPhysicsEngineBullet::GetCentralImpulse(const class CActor *pSender) const
{
	return 0;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SetLinearVelocity(const SPhysNode *pPhysNode, const Vector *pValue)
{
	assert(pPhysNode && pPhysNode->pRigidBody);
	pPhysNode->pRigidBody->setLinearVelocity(btVector3(pValue->x, pValue->y, pValue->z));
}

//----------------------------------------------------------------------------------------------
Vector*	CPhysicsEngineBullet::GetLinearVelocity(const class CActor *pSender) const
{
	return 0;
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SetGravity(SPhysNode *pNode, const Vector *pValue)
{
	if (pNode && pNode->pRigidBody && pValue)
	{
		pNode->pRigidBody->setGravity(btVector3(pValue->x, pValue->y, pValue->z));
	}
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::ContactTest(SPhysNode *pNode)
{
	if (pNode && pNode->pRigidBody && pNode->pCallback)
	{
		m_pDynamicsWorld->contactTest(pNode->pRigidBody, *pNode->pCallback);
	}
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::SyncActors()
{
	//m_pCoreSDK->EnterCommandCS();

	CViewportManager *pViewMgr = m_pCoreSDK->GetViewportManager();

	if (m_SyncData.size() > 0)
	{
		TMapSyncDataIterator Iter = m_SyncData.begin();

		while (Iter != m_SyncData.end())
		{
			CActor *pParent = Iter->first->GetParent();

			pViewMgr->GetByActor(pParent)->SetPivot_(Iter->second); // TODO!!! REFACTOR!!!
			
			pViewMgr->RebuildTransform(Iter->first->GetParent());

			++Iter;
		}
		m_SyncData.clear();
	}

	//m_pCoreSDK->LeaveCommandCS();
}

//----------------------------------------------------------------------------------------------
void CPhysicsEngineBullet::ProcessContacts()
{
	// contact responds
	int numManifolds = m_pDynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold *contactManifold = m_pDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject *obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject *obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		// check shapes for user pointer data
		CActor *pUserA = GetActorByShape(obA->getCollisionShape());
		CActor *pUserB = GetActorByShape(obB->getCollisionShape());

		if (pUserA || pUserB)
		{
			int numContacts = contactManifold->getNumContacts();

			if (numContacts > 0)
			{
				Vector SumContactsA(0.f, 0.f, 0.f);
				Vector SumContactsB(0.f, 0.f, 0.f);
				Vector NormalAvgB(0.f, 0.f, 0.f);

				for (int j = 0; j < numContacts; j++)
				{
					btManifoldPoint &pt = contactManifold->getContactPoint(j);

					const btVector3 &posA = pt.getPositionWorldOnA();
					const btVector3 &posB = pt.getPositionWorldOnB();
					const btVector3 &NormB = pt.m_normalWorldOnB;


					btVector3 ptA = posA - obA->getWorldTransform().getOrigin();
					btVector3 ptB = posB - obB->getWorldTransform().getOrigin();

					SumContactsA += Vector(ptA.x(), ptA.y(), ptA.z());
					SumContactsB += Vector(ptB.x(), ptB.y(), ptB.z());

					NormalAvgB += Vector(NormB.x(), NormB.y(), NormB.z());

					//m_pCoreSDK->GetRenderSDK()->DrawLine(Vector(ptA.x(),ptA.y(),ptA.z()), Vector(ptB.x(),ptB.y(),ptB.z()), 0xffffffff, false);
				}

				NormalAvgB.normalize();

				if (pUserA)
				{
					CActor *pParentA = pUserA->GetParent();

					m_pCoreSDK->GetUpdateManager()->PostPhysEvent(pParentA, EPhysEventContactsSum, &SumContactsA, &NormalAvgB);

					if (pUserB){
						m_pCoreSDK->GetUpdateManager()->PostPhysEvent(pParentA, EPhysEventContact, pUserB->GetParent(), &NormalAvgB);
					}
				}

				if (pUserB)
				{
					CActor *pParentB = pUserB->GetParent();

					NormalAvgB *= -1.f; // invert

					m_pCoreSDK->GetUpdateManager()->PostPhysEvent(pParentB, EPhysEventContactsSum, &SumContactsB, &NormalAvgB);

					if (pUserA){
						m_pCoreSDK->GetUpdateManager()->PostPhysEvent(pParentB, EPhysEventContact, pUserA->GetParent(), &NormalAvgB);
					}
				}
			}
		}

		//you can un-comment out this line, and then all points are removed
		//contactManifold->clearManifold();
	}
}

//----------------------------------------------------------------------------------------------
CActor* CPhysicsEngineBullet::GetActorByShape(const btCollisionShape *pShape) const
{
	CActor *pOutActor = 0;

	// check shape A
	if (pShape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
	{
		const btCompoundShape *pCompoundShape = static_cast<const btCompoundShape*>(pShape);
		for (int Index = pCompoundShape->getNumChildShapes() - 1; Index >= 0; --Index)
		{
			const btCollisionShape *pColShape = pCompoundShape->getChildShape(Index);

			if (pColShape)
			{
				if (void *pUserData = pColShape->getUserPointer())
				{
					pOutActor = static_cast<CActor*> (pUserData);
					break;
				}
			}
		}
	}
	else
	{
		if (void *pUserData = pShape->getUserPointer())
		{
			pOutActor = static_cast<CActor*> (pUserData);
		}
	}
	return pOutActor;
}

#endif PHYS_ENABLED