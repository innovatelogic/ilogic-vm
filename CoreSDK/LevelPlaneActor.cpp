#include "coresdkafx.h"

REGISTER_CLASS_A(CLevelPlaneActor, ActorAllocator)
	new oes::rflex::PropertyString("XRef", (BYTE*)&((CLevelPlaneActor*)NULL)->m_XRef - (BYTE*)NULL, "CLevelPlaneActor", "Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(CLevelPlaneActor, ActorAllocator);

//----------------------------------------------------------------------------------------------
CLevelPlaneActor::CLevelPlaneActor(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pCollisionObject, CPlaneCollisionController, "CollisionObject", this);

	RegisterRenderEntity(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
CLevelPlaneActor::~CLevelPlaneActor()
{

}

//----------------------------------------------------------------------------------------------
void CLevelPlaneActor::OnPropertyChanged(const char *PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "XRef"))
	{
		LoadMesh(m_XRef.c_str());

		//SetBounds(m_pMeshComponent->GetBounds());
	}
}

//----------------------------------------------------------------------------------------------
void CLevelPlaneActor::Initialize()
{
	Super::Initialize();

	LoadMesh(m_XRef.c_str());

	InitPhysPlane();
}

//----------------------------------------------------------------------------------------------
void CLevelPlaneActor::DoDraw()
{
	GetRenderComponent()->DrawObject(m_pMeshComponent);

/*	if (IsFocused())
	{
		Matrix Mat = GetTransformedWTM_() * m_pCollisionObject->GetLTM();

		const float k = 20.f;
		const Vector AxisX = Mat._row0 * k;
		const Vector AxisY = Mat._row1 * (k * 0.5f);
		const Vector AxisZ = Mat._row2 * k;

		const Vector VC0 = Mat.t - AxisX - AxisZ;
		const Vector VC1 = Mat.t + AxisX + AxisZ;

		m_pCoreSDK->GetRenderSDK()->DrawLine(VC0, VC0 + (AxisZ * 2), COLOR_YELLOW, false);
		m_pCoreSDK->GetRenderSDK()->DrawLine(VC0, VC0 + (AxisX * 2), COLOR_YELLOW, false);
				
		m_pCoreSDK->GetRenderSDK()->DrawLine(VC1, VC1 - (AxisZ * 2), COLOR_YELLOW, false);
		m_pCoreSDK->GetRenderSDK()->DrawLine(VC1, VC1 - (AxisX * 2), COLOR_YELLOW, false);

		m_pCoreSDK->GetRenderSDK()->DrawLine(Mat.t, Mat.t + AxisY, COLOR_YELLOW, false);
	}*/
}

//----------------------------------------------------------------------------------------------
bool CLevelPlaneActor::InitPhysPlane()
{
	return m_pCollisionObject->CreatePhysActor(GetTransformedWTM_());
}

//----------------------------------------------------------------------------------------------
bool CLevelPlaneActor::LoadMesh(const char *url)
{
	bool bResult = false;
   
	SMeshMaterialSet ObjectDesc;

	std::string sFilename;

	if (url && FindFile(url, &sFilename))
	{
		XML_PARSE_FILE(sFilename.c_str())
		{
			XML_FOR_EACH_TREE(XML_CURRENT_NODE)
			{
				XML_CHECK_NODE("mesh")
				{
					XML_DEF_ATTRIBUTES_NODE(obj);
					XML_DEF_ATTRIBUTES_NODE(hsk);
					XML_DEF_ATTRIBUTES_NODE(trk);
					XML_DEF_ATTRIBUTES_NODE(col);

					ObjectDesc.ObjectFile = obj;
					ObjectDesc.SklFile = hsk;
					ObjectDesc.TrkFile = trk;
					ObjectDesc.ColFile = col;

					XML_FOR_EACH_TREE(XML_CURRENT_NODE)
					{
						XML_CHECK_NODE("textures")
						{
							XML_FOR_EACH_TREE(XML_CURRENT_NODE)
							{
								XML_CHECK_NODE("tex")
								{
									XML_DEF_ATTRIBUTES_NODE(name);
									ObjectDesc.Textures.push_back(name);
								}
							}
						}

						XML_CHECK_NODE("mtrls")
						{
							XML_FOR_EACH_TREE(XML_CURRENT_NODE)
							{
								XML_CHECK_NODE("mat")
								{
									XML_DEF_ATTRIBUTES_NODE(name);
									XML_DEF_ATTRIBUTES_NODE(fx);
									XML_DEF_ATTRIBUTES_NODE(tehnique);
									XML_DEF_ATTRIBUTES_NODE(id);
									XML_DEF_ATTRIBUTES_NODE(txids);
									XML_DEF_ATTRIBUTES_NODE(txchs);
									XML_DEF_ATTRIBUTES_NODE(diffuse);
									XML_DEF_ATTRIBUTES_NODE(specular);
									XML_DEF_ATTRIBUTES_NODE(shininess);
									XML_DEF_ATTRIBUTES_NODE(ambient);
									XML_DEF_ATTRIBUTES_NODE(emission);
									XML_DEF_ATTRIBUTES_NODE(transparent);
									XML_DEF_ATTRIBUTES_NODE(fog);
									XML_DEF_ATTRIBUTES_NODE(fogcolor);

									SMeshMaterial MeshMat;

									MeshMat.Name = name;
									MeshMat.FX = fx;
									MeshMat.tehnique = tehnique;
									MeshMat.id = atoi(id.c_str());

									CStringUtility<int> Parse_txids(txids, ";");
									std::vector<int>::iterator IterTxids = Parse_txids.m_vector.begin();

									while (IterTxids != Parse_txids.m_vector.end())
									{
										MeshMat.textures.push_back(*IterTxids);
										++IterTxids;
									}

									CStringUtility<int> Parse_txchs(txids, ";");
									std::vector<int>::iterator IterTxchs = Parse_txchs.m_vector.begin();

									while (IterTxchs != Parse_txchs.m_vector.end())
									{
										MeshMat.textures.push_back(*IterTxchs);
										++IterTxchs;
									}

									// diffuse
									CStringUtility<float> ParseDiffuse(diffuse, ";");

									if (ParseDiffuse.m_vector.size() == 4)
									{
										MeshMat.diffuse[0] = ParseDiffuse.m_vector[0];
										MeshMat.diffuse[1] = ParseDiffuse.m_vector[1];
										MeshMat.diffuse[2] = ParseDiffuse.m_vector[2];
										MeshMat.diffuse[3] = ParseDiffuse.m_vector[3];
									}
									else { // TODO: Warning
									}

									// specular
									CStringUtility<float> ParseSpecular(specular, ";");
									if (ParseSpecular.m_vector.size() == 4)
									{
										MeshMat.specular[0] = ParseSpecular.m_vector[0];
										MeshMat.specular[1] = ParseSpecular.m_vector[1];
										MeshMat.specular[2] = ParseSpecular.m_vector[2];
										MeshMat.specular[3] = ParseSpecular.m_vector[3];
									}
									else { // TODO: Warning
									}

									// shiness
									MeshMat.shininess = (float)atof(shininess.c_str());

									// ambient
									CStringUtility<float> ParseAmbient(ambient, ";");
									if (ParseAmbient.m_vector.size() == 4)
									{
										MeshMat.ambient[0] = ParseAmbient.m_vector[0];
										MeshMat.ambient[1] = ParseAmbient.m_vector[1];
										MeshMat.ambient[2] = ParseAmbient.m_vector[2];
										MeshMat.ambient[3] = ParseAmbient.m_vector[3];
									}
									else { // TODO: Warning
									}

									// emission
									CStringUtility<float> ParseEmission(emission, ";");
									if (ParseEmission.m_vector.size() == 4)
									{
										MeshMat.emission[0] = ParseEmission.m_vector[0];
										MeshMat.emission[1] = ParseEmission.m_vector[1];
										MeshMat.emission[2] = ParseEmission.m_vector[2];
										MeshMat.emission[3] = ParseEmission.m_vector[3];
									}
									else { // TODO: Warning
									}

									MeshMat.transparent = (float)atof(transparent.c_str());

									bool bFogFlag = (strcmp(fog.c_str(), "true") == 0 ||
										strcmp(fog.c_str(), "True") == 0 ||
										strcmp(fog.c_str(), "1") == 0) ? true : false;

									MeshMat.fog = bFogFlag;

									// emission
									CStringUtility<float> ParseFogColor(fogcolor, ";");
									if (ParseFogColor.m_vector.size() == 4)
									{
										MeshMat.fog_color[0] = ParseFogColor.m_vector[0];
										MeshMat.fog_color[1] = ParseFogColor.m_vector[1];
										MeshMat.fog_color[2] = ParseFogColor.m_vector[2];
										MeshMat.fog_color[3] = ParseFogColor.m_vector[3];
									}
									else { // TODO: Warning
									}

									ObjectDesc.Materials.push_back(MeshMat);
								}
							}
						}
					}
				}
			}
		}
		
		m_pMeshComponent->LoadMesh(ObjectDesc);

		bResult = true;
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
void CLevelPlaneActor::Clear()
{

}