#include "coresdkafx.h"
#include "CharacterControl.h"

#pragma warning(disable:4244)
/*
//----------------------------------------------------------------------------------------------
bool PhysicsEnginePhysX::CreateActorFromFile(const char *filename,
										const CActor * Sender,
										const Matrix* pWTM,
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
return false;
	std::string sFilename;
	if (!Sender->GetEnabled() || !FindFile(filename, &sFilename)){
		return false;
	}

	std::vector<std::string> FileStrings;
	std::ifstream oFile(sFilename.c_str());

	if (!m_pScene || oFile.fail()){
		return false;
	}

	unsigned int NumObjects;

	oFile >> NumObjects;

	Lock();

	ReleaseObject(Sender);

	NxActorDesc ActorDesc;
	NxBodyDesc	BodyDesc;

	for (size_t Index = 0; Index < NumObjects; ++Index)
	{
		unsigned int ObjectType;

		oFile >> ObjectType;

		if (ObjectType == 0)
		{
			NxBoxShapeDesc BoxShapeDesc;

			Matrix LTM;
			Vector Extends;

			oFile >> LTM._11 >> LTM._12 >> LTM._13 >> LTM._14 >>
				LTM._21 >> LTM._22 >> LTM._23 >> LTM._24 >>
				LTM._31 >> LTM._32 >> LTM._33 >> LTM._34 >>
				LTM._41 >> LTM._42 >> LTM._43 >> LTM._44;

			oFile >> Extends.x >> Extends.y >> Extends.z;

			Vector HalfExt = Extends * PHIS_MULTYPLIER * 0.5f;

			BoxShapeDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;
			BoxShapeDesc.dimensions.set(HalfExt.x, HalfExt.y, HalfExt.z);

			BoxShapeDesc.localPose.M.setRow(0, NxVec3(LTM._11, LTM._21, LTM._31));
			BoxShapeDesc.localPose.M.setRow(1, NxVec3(LTM._12, LTM._22, LTM._32));
			BoxShapeDesc.localPose.M.setRow(2, NxVec3(LTM._13, LTM._23, LTM._33));
			BoxShapeDesc.localPose.t = NxVec3(LTM.t.x, LTM.t.y, LTM.t.z) * PHIS_MULTYPLIER;

			ActorDesc.shapes.pushBack(&BoxShapeDesc);
		}
		else if (ObjectType == 1)
		{
			NxSphereShapeDesc ShpereDesc;
		}
		else if (ObjectType == 2)
		{
			NxCapsuleShapeDesc CapsuleDesc;
		}
		else if (ObjectType == 3)
		{
			std::vector<NxVec3> Verts;
			std::vector<NxU32> Indices;

			Matrix LTM;

			oFile >> LTM._11 >> LTM._12 >> LTM._13 >> LTM._14 >>
				LTM._21 >> LTM._22 >> LTM._23 >> LTM._24 >>
				LTM._31 >> LTM._32 >> LTM._33 >> LTM._34 >>
				LTM._41 >> LTM._42 >> LTM._43 >> LTM._44;

			unsigned int NumPoints;
			oFile >> NumPoints;
			for (unsigned int IndexPoint = 0; IndexPoint < NumPoints; ++IndexPoint)
			{
				NxVec3 V;
				oFile >> V.x >> V.y >> V.z;
				Verts.push_back(V);
			}

			unsigned int NumIndexes;
			oFile >> NumIndexes;
			for (unsigned int IndexIdx = 0; IndexIdx < NumIndexes; ++IndexIdx)
			{
				NxU32 I;
				oFile >> I;
				Indices.push_back(I);
			}

			// allocate new
			NxTriangleMeshDesc TriangleMeshDesc;

			TriangleMeshDesc.numVertices		= (NxU32)Verts.size();
			TriangleMeshDesc.pointStrideBytes	= sizeof(NxVec3);
			TriangleMeshDesc.points				= &Verts[0];
			TriangleMeshDesc.numTriangles		= (NxU32)Indices.size() / 3;
			TriangleMeshDesc.flags				= 0;
			TriangleMeshDesc.triangles			= &Indices[0];
			TriangleMeshDesc.triangleStrideBytes = 3 * sizeof(NxU32);

			MemoryWriteBuffer buf;
			NxTriangleMesh * pMesh;

			{
				NxInitCooking();
				bool status = NxCookTriangleMesh(TriangleMeshDesc, buf);

				if (status){
					pMesh = m_pPhisicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
				}else{
					assert(false);
					pMesh = NULL;
				}
				NxCloseCooking();
			}

			NxTriangleMeshShapeDesc tmsd;
			tmsd.meshPagingMode = NX_MESH_PAGING_AUTO;
			tmsd.group			= GROUP_COLLIDABLE_NON_PUSHABLE;
			tmsd.meshData		= pMesh;
			tmsd.localPose.t	= NxVec3(0, 0, 0) * PHIS_MULTYPLIER;

			ActorDesc.shapes.pushBack(&tmsd);
			bStatic = true;
		}
		else if (ObjectType == 4)
		{
			std::vector<NxVec3> Verts;
			std::vector<NxU32> Indices;

			Matrix LTM;

			oFile >> LTM._11 >> LTM._12 >> LTM._13 >> LTM._14 >>
				LTM._21 >> LTM._22 >> LTM._23 >> LTM._24 >>
				LTM._31 >> LTM._32 >> LTM._33 >> LTM._34 >>
				LTM._41 >> LTM._42 >> LTM._43 >> LTM._44;

			unsigned int NumPoints;
			oFile >> NumPoints;
			for (unsigned int IndexPoint = 0; IndexPoint < NumPoints; ++IndexPoint)
			{
				NxVec3 V;
				oFile >> V.x >> V.y >> V.z;
				Verts.push_back(V);
			}

			unsigned int NumIndexes;
			oFile >> NumIndexes;
			for (unsigned int IndexIdx = 0; IndexIdx < NumIndexes; ++IndexIdx)
			{
				NxU32 I;
				oFile >> I;
				Indices.push_back(I);
			}

			// allocate new
			NxConvexMeshDesc ConvexMeshDesc;
			ConvexMeshDesc.numVertices		 = (NxU32)Verts.size();
			ConvexMeshDesc.pointStrideBytes  = sizeof(NxVec3);
			ConvexMeshDesc.points			 = &Verts[0];
			ConvexMeshDesc.flags			 = NX_CF_COMPUTE_CONVEX;

			NxConvexShapeDesc convexShapeDesc;

			convexShapeDesc.localPose.M.setRow(0, NxVec3(LTM._11, LTM._21, LTM._31));
			convexShapeDesc.localPose.M.setRow(1, NxVec3(LTM._12, LTM._22, LTM._32));
			convexShapeDesc.localPose.M.setRow(2, NxVec3(LTM._13, LTM._23, LTM._33));
			convexShapeDesc.localPose.t = NxVec3(LTM.t.x, LTM.t.y, LTM.t.z) * PHIS_MULTYPLIER;
			//convexShapeDesc.userData = ConvexMeshDesc;

			MemoryWriteBuffer buf;
			NxConvexMesh * pMesh = NULL;

			{
				NxInitCooking();
				bool bStatus = NxCookConvexMesh(ConvexMeshDesc, buf);

				if (bStatus)
				{
					pMesh = m_pPhisicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
					assert(pMesh);
					convexShapeDesc.meshData = pMesh;
				}
				else
				{
					assert(false);
					pMesh = NULL;
				}

				NxCloseCooking();
			}
			assert(convexShapeDesc.isValid());
			ActorDesc.shapes.pushBack(&convexShapeDesc);
		}
	}

	ActorDesc.body = (!bStatic) ? &BodyDesc : NULL;
	ActorDesc.density = Density;

	ActorDesc.globalPose.M.setRow(0, NxVec3(pWTM->_11, pWTM->_21, pWTM->_31));
	ActorDesc.globalPose.M.setRow(1, NxVec3(pWTM->_12, pWTM->_22, pWTM->_32));
	ActorDesc.globalPose.M.setRow(2, NxVec3(pWTM->_13, pWTM->_23, pWTM->_33));
	ActorDesc.globalPose.t = NxVec3(pWTM->t.x, pWTM->t.y, pWTM->t.z)* PHIS_MULTYPLIER;
	ActorDesc.group = GROUP_COLLIDABLE_NON_PUSHABLE;

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

	Unlock();

	oFile.close();

	return bResult;
}*/