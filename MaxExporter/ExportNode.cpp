#include "expstdafx.h"

//----------------------------------------------------------------------------------
void DumpSampleKeys(IGameNode *pIGameNode, m_node *pNode)
{
	TimeValue t0 = ExporterMAX::GetExporter()->GetIGame()->GetSceneStartTime();
	TimeValue t1 = ExporterMAX::GetExporter()->GetIGame()->GetSceneEndTime();
	TimeValue DeltaTime = ExporterMAX::GetExporter()->GetIGame()->GetSceneTicks();

	int SamplingRate = 1;
	int numkeys = (t1 - t0) / (DeltaTime * SamplingRate) + 1;

	pNode->anim.num_keys = numkeys;
	pNode->anim.pos = new Vector[pNode->anim.num_keys];
	pNode->anim.rot = new Quaternion[pNode->anim.num_keys];

	// we discard scale animation
	pNode->anim.scale = 0;

	AffineParts	PRS;
	int sample = 0;
	for(long i = t0; i <= t1; i+= DeltaTime * SamplingRate)
	{
		decomp_affine(pIGameNode->GetObjectTM(i).ExtractMatrix3(), &PRS);
		pNode->anim.rot[sample] = Quaternion(-PRS.q.x, -PRS.q.z, PRS.q.y, PRS.q.w);
		pNode->anim.pos[sample] = Vector(PRS.t.x, PRS.t.z, -PRS.t.y);
		//pNode->anim.scale[sample] = vec3(PRS.k.x * PRS.f, PRS.k.z * PRS.f, PRS.k.y * PRS.f);
		++sample;
	}
	pIGameNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame());
}

//----------------------------------------------------------------------------------------------
void GetNodeInfo(INode *pINode, unsigned int &lvlLOD, unsigned int &lvlDestr)
{
	TSTR Str;
	pINode->GetUserPropBuffer(Str);
	std::string StrProperties((char*)Str);

	lvlLOD = 0;
	lvlDestr = 0;

	ValueParser ValueStore(StrProperties);

	if (ValueStore.IsValue("LOD"))
	{
		lvlLOD = ValueStore.GetValueUINT("LOD");
	}
	if (ValueStore.IsValue("Dstr")){
		lvlDestr = ValueStore.GetValueUINT("Dstr");
	}
}

//----------------------------------------------------------------------------------------------
void DumpNode(IGameNode *pGameNode, m_node *pNode)
{
	ExporterMAX *pExporter = ExporterMAX::GetExporter();

	// the name might already be set because of the lazy creation scheme...
	if (pNode->name == ""){
		pNode->name = pGameNode->GetName();
	}

	GMatrix nodeTM = pGameNode->GetObjectTM(pExporter->GetStaticFrame());

	AffineParts	PRS;
	decomp_affine(nodeTM.ExtractMatrix3(), &PRS);

	pNode->xform.set_rot(Quaternion(PRS.q.x, PRS.q.z, PRS.q.y, PRS.q.w));
	pNode->xform.set_translation(Vector(-PRS.t.x, -PRS.t.z, -PRS.t.y));

	unsigned int lvlLOD = 0;
	unsigned int lvlDestr = 0;

	GetNodeInfo(pGameNode->GetMaxNode(), lvlLOD, lvlDestr);

	pExporter->AddNode(pNode, lvlLOD, lvlDestr);

	// recreate the hierarchy:
	// parent node...
	IGameNode *pParent = pGameNode->GetNodeParent();

	if (pParent)
	{
		unsigned int lvlPLOD = 0;
		unsigned int lvlPDestr = 0;

		GetNodeInfo(pParent->GetMaxNode(), lvlPLOD, lvlPDestr);

		unsigned int parent_idx = pExporter->FindNode(pParent->GetName(), lvlPLOD, lvlPDestr);

		if (parent_idx != BAD_IDX)
		{
			pNode->parent = parent_idx;
		}
		else
		{
			DumpMaxNode(pParent);
			pNode->parent = pExporter->FindNode(pParent->GetName(), lvlPLOD, lvlPDestr);
			assert(pNode->parent != BAD_IDX);
		}
	}
	else
	{
		pNode->parent = BAD_IDX;
		pNode->Parent = 0;
	}

	// children nodes...
	size_t numChildren = (size_t)pGameNode->GetChildCount();
	for (size_t Index = 0; Index < numChildren; ++Index)
	{
		pNode->childrens.push_back(0);
	}

	for (size_t i = 0; i < numChildren; ++i)
	{
		IGameNode *pChild = pGameNode->GetNodeChild(i);

		unsigned int lvlCLOD = 0;
		unsigned int lvlCDestr = 0;

		GetNodeInfo(pChild->GetMaxNode(), lvlCLOD, lvlCDestr);

		unsigned int child_idx = pExporter->FindNode(pChild->GetName(), lvlCLOD, lvlCDestr);
	
		if (child_idx != BAD_IDX)
		{
			pNode->childrens[i] = child_idx;
		}
		else
		{
			DumpMaxNode(pChild);
			pNode->childrens[i] = pExporter->FindNode(pChild->GetName(), lvlCLOD, lvlCDestr);
			assert(pNode->childrens[i] != BAD_IDX);
		}
	}

	DumpSampleKeys(pGameNode, pNode);
}

//----------------------------------------------------------------------------------
void DumpMaxNode(IGameNode *pGameNode)
{
	static int curNode = 0;

	IGameKeyTab poskeys;
	IGameKeyTab colkeys;
	IGameKeyTab rotkeys;
	IGameKeyTab scalekeys;
	TSTR buf, data;

	ExporterMAX *pExporter = ExporterMAX::GetExporter();

	unsigned int lvlLOD = 0;
	unsigned int lvlDestr = 0;

	GetNodeInfo(pGameNode->GetMaxNode(), lvlLOD, lvlDestr);

	m_node *pNode = NULL; // base interface

	IGameObject *pObj = pGameNode->GetIGameObject();
	
	unsigned int node_idx = pExporter->FindNode(pGameNode->GetName(), lvlLOD, lvlDestr);

	if (node_idx == BAD_IDX)
	{
		curNode++;

		buf = TSTR("Processing: ") + TSTR(pGameNode->GetName());

		GetCOREInterface()->ProgressUpdate((int)((float)curNode / pExporter->GetIGame()->GetTotalNodeCount() * 100.0f), FALSE, buf.data()); 
		
		buf += "\n";
		
		OutputDebugString(buf.data());

		switch (pObj->GetIGameType())
		{
		case IGameObject::IGAME_MESH:
			{
				m_model *pModel = m_factory::get_factory()->new_model();

				INode *pINode = pGameNode->GetMaxNode();

				if (pINode->IsNodeHidden() != 0 || // we don't export hidden objects as real meshes
					pINode->GetBoxMode() != 0 ||   // we don't export boxes as real meshes
					!DumpModel((IGameMesh*)pObj, pModel, pGameNode))
				{
					delete pModel; // failed
					
					// lets create a place holder...
					pNode = m_factory::get_factory()->new_node(); 
				}
				else
				{
					pNode = static_cast<m_node*>(pModel);
				}
				break;
			}

		case IGameObject::IGAME_HELPER:
		case IGameObject::IGAME_BONE:
		default:
			{
				pNode = m_factory::get_factory()->new_node();
			}
		}

		if (pNode)
		{
			DumpNode(pGameNode, pNode);
		}
	}

	for (int i = 0; i < pGameNode->GetChildCount(); ++i)
	{
		IGameNode *newchild = pGameNode->GetNodeChild(i);

		// we deal with targets in the light/camera section
		if (newchild->IsTarget()){
			continue;
		}
		DumpMaxNode(newchild);
	}

	pGameNode->ReleaseIGameObject();
}

//----------------------------------------------------------------------------------
void DumpNodeCollision(IGameNode *pGameNode)
{
	IGameObject *pObj = pGameNode->GetIGameObject();
	
	if (pObj->GetIGameType() == IGameObject::IGAME_MESH)
	{
		INode *pINode = pGameNode->GetMaxNode();
		GMatrix MT = pGameNode->GetWorldTM(ExporterMAX::GetExporter()->GetStaticFrame());

		TSTR Str;
		pINode->GetUserPropBuffer(Str);
		std::string StrProperties((char*)Str);

		if ((pINode->IsNodeHidden() || pINode->GetBoxMode()) && !StrProperties.empty())
		{
			INode *pINode = pGameNode->GetMaxNode();

			//AffineParts	PRS;
			Matrix3 world_to_obj = MT.ExtractMatrix3(); //Inverse(
			world_to_obj.Orthogonalize();
			//decomp_affine(MT.ExtractMatrix3(), &PRS);

			Point3 R0 = world_to_obj.GetRow(0);
			Point3 R1 = world_to_obj.GetRow(1);
			Point3 R2 = world_to_obj.GetRow(2);
			Point3 R3 = world_to_obj.GetRow(3);

			Matrix xform;
						
			xform.row0 = Vector4f(R0.x, R1.x, R2.x, R3.x);
			xform.row1 = Vector4f(R0.y, R1.y, R2.y, R3.y);
			xform.row2 = Vector4f(R0.z, R1.z, R2.z, R3.z);
			xform.row3 = Vector4f(0.f, 0.f, 0.f, 1.f);

			//xform._row0.normalize();
			//xform._row1.normalize();
			//xform._row2.normalize();

			//xform.set_rot(Quaternion(-PRS.q.x, -PRS.q.z, PRS.q.y, PRS.q.w));
			//xform.set_translation(Vector(-PRS.t.x, PRS.t.z, -PRS.t.y));

			ValueParser ValueStore(StrProperties);
			
			if (ValueStore.IsValue("Type"))
			{
				std::string StrType = ValueStore.GetValueString("Type");

				if (StrType == "Box")
				{
					CollisionShapeBox *pCollisionObject = new CollisionShapeBox();
					pCollisionObject->xform = xform;
					pCollisionObject->Ext = ValueStore.GetValueVector("Extends");

					ExporterMAX::GetExporter()->m_VCollisionObjects.push_back(pCollisionObject);
				}
				else if (StrType == "Sphere")
				{
					CollisionShapeSphere *pCollisionObject = new CollisionShapeSphere();
					pCollisionObject->xform = xform;
					pCollisionObject->Radius = ValueStore.GetValueFloat("Radius");

					ExporterMAX::GetExporter()->m_VCollisionObjects.push_back(pCollisionObject);
				}
				else if (StrType == "Capsule")
				{
					CollisionShapeCapsule *pCollisionObject = new CollisionShapeCapsule();
					pCollisionObject->xform = xform;
					// @TODO
				}
				else if (StrType == "Mesh")
				{
					CollisionShapeMesh *pCollisionObject = new CollisionShapeMesh();
					pCollisionObject->xform = xform;
					DumpModelCollision((IGameMesh*)pObj, pCollisionObject, pGameNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame()));

					ExporterMAX::GetExporter()->m_VCollisionObjects.push_back(pCollisionObject);
				}
				else if (StrType == "Convex")
				{
					CollisionShapeConvex * pCollisionObject = new CollisionShapeConvex();
					pCollisionObject->xform = xform;
					DumpModelCollision((IGameMesh*)pObj, pCollisionObject, pGameNode->GetObjectTM(ExporterMAX::GetExporter()->GetStaticFrame()));

					ExporterMAX::GetExporter()->m_VCollisionObjects.push_back(pCollisionObject);
				}
			}
		}
	}

	// iterate export childs
	for (int i = 0; i < pGameNode->GetChildCount(); ++i)
	{
		IGameNode * pNewChild = pGameNode->GetNodeChild(i);

		// we deal with targets in the light/camera section
		if (pNewChild->IsTarget()){
			continue;
		}
		DumpNodeCollision(pNewChild);
	}
}