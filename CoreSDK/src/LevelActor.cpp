#include "LevelActor.h"
#include "RenderSDK.h"
#include "D3DMesh.h"
#include "Comp_StaticMesh.h"
//#include "../ActorCollisionController.h"


volatile static oes::rflex::CAuto<core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>, ActorAllocator>
ClassRegistration___("core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>", 0, 0,
    "core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>", "ActorAllocator");

REGISTER_CLASS_A(CLevelActor, CLevelActor::Super)
	//new oes::rflex::PropertyString("XRef", DATAFIELD_OFFSET(CLevelActor, m_XRef) - 4, "CLevelActor", "Value", READ_WRITE, CTRL_EDIT_RESOURCE, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
    new oes::rflex::TProperty<std::string, CLevelActor>("XRef", "CLevelActor", "Value", 
        [&](const void *ptr, const std::string &v)
        { 
            void *nc_ptr = const_cast<void*>(ptr);
            CLevelActor *act = static_cast<CLevelActor*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));
            act->m_XRef = v;
        }),
    
    ASSEMBLE_PROPS(CLevelActor)
BEGIN_INTERFACE_DECLARATION(CLevelActor)
    new oes::rflex::SInterfaceDecl("IDrawInterface", (BYTE*)&((CLevelActor*)NULL)->___startObjectMarkerIDrawInterface - (BYTE*)NULL),
ASSEMBLE_INTERFACES(CLevelActor)
CLASS_ENDL(CLevelActor)
CLASS_INSTANCE_EX(CLevelActor, CLevelActor::Super);

//----------------------------------------------------------------------------------------------
CLevelActor::CLevelActor(const CObjectAbstract *pParent)
: core_sdk_api::NpActorTemplate<core_sdk_api::PxLevelActor>(pParent)
, IDrawInterface(pParent)
, m_XRef("BEEF")
{
    //int k = (int)offsetof(CLevelActor, m_XRef);

	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "MeshComponent", this);
	//NEW_OBJECT_TRANSIENT_CHILD(m_pCollisionObject, ActorCollisionController, "CollisionObject", this);

	RegisterRenderEntity(m_pMeshComponent);
	//RegisterCollisionEntity(m_pCollisionObject);
}

//----------------------------------------------------------------------------------------------
CLevelActor::~CLevelActor()
{

}

//----------------------------------------------------------------------------------------------
void CLevelActor::OnPropertyChanged(const char *PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "XRef"))
	{
		Load(m_XRef.c_str());

		//SetBounds(m_pMeshComponent->GetBounds());
	}
}

//----------------------------------------------------------------------------------------------
void CLevelActor::Initialize()
{
	Super::Initialize();

	Load(m_XRef.c_str());
}

//----------------------------------------------------------------------------------------------
void CLevelActor::DoDraw()
{
	GetRenderComponent()->DrawObject(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
bool CLevelActor::Load(const char *url)
{
	bool bResult = false;
   
	SMeshMaterialSet ObjectDesc;

	std::string sFilename;

	if (FindFile(url, &sFilename))
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
		
		//m_pCollisionObject->Load(ObjectDesc.ColFile.c_str());

		bResult = true;
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
void CLevelActor::Clear()
{

}