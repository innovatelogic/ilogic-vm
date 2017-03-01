#include "coresdkafx.h"

REGISTER_CLASS_A(CPawnCueBall, ActorAllocator)
	new oes::rflex::PropertyString("XRef", (BYTE*)&((CPawnCueBall*)NULL)->m_XRef - (BYTE*)NULL, "CPawnCueBall", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(CPawnCueBall, ActorAllocator);

//----------------------------------------------------------------------------------------------
CPawnCueBall::CPawnCueBall(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
, IEntityController(pParent)
, IUpdateInterface(pParent)
, m_EMoveDirection(EMoveDirectionDown)
, m_linearVelocity(0.f, 0.f, 0.f)
{
	RegisterDrawInterface(this);
	RegisterEntityInterface(this, true);
	RegisterUpdateInterface(this, pParent);
	
	NEW_OBJECT_TRANSIENT_CHILD(m_pMeshComponent, Comp_StaticMesh, "MeshComponent", this);
	NEW_OBJECT_TRANSIENT_CHILD(m_pSphereController, CSphereCollisionController, "CollisionObject", this);

	m_pContactCallback = new CContactSensorCallbackBt(this);

	RegisterCollisionEntity(m_pSphereController);
	RegisterRenderEntity(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
CPawnCueBall::~CPawnCueBall()
{

}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::OnPropertyChanged(const char *pPropertyName)
{
	Super::OnPropertyChanged(pPropertyName);
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::Initialize()
{
	Super::Initialize();

	// load mesh
	SMeshMaterialSet ObjectDesc;

	std::string sFilename;

	if (FindFile("sphere01_1.mat", &sFilename))
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
	}

	m_pMeshComponent->LoadMesh(ObjectDesc);

	// receive data from game info
	m_pSphereController->SetRadius(1.f);

	m_pSphereController->Set_FROZEN_POS_Z(true);
	m_pSphereController->Set_FROZEN_ROT_X(true);
	m_pSphereController->Set_FROZEN_ROT_Y(true);
	m_pSphereController->Set_FROZEN_ROT_Z(true);
	m_pSphereController->Set_DISABLE_GRAVITY(true);

	m_pSphereController->CreatePhysActor(m_pContactCallback);
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::PreRelease()
{
	Super::PreRelease();
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::DoDraw()
{
	GetRenderComponent()->DrawObject(m_pMeshComponent);
}

//----------------------------------------------------------------------------------------------
bool CPawnCueBall::ProcessInputMouseEnt(const MouseInputData &InputData)
{
	return true;
}

//----------------------------------------------------------------------------------------------
bool CPawnCueBall::ProcessInputMouseEnt(const MouseMoveInputData &InputData)
{
	if (GetAppMain()->GetCameraManager())
	{
		if (const CCamera *pCamera = GetAppMain()->GetCameraManager()->GetActiveCamera(InputData.pRenderContext))
		{
			Vector CamDir(pCamera->GetDirection());

			CamDir.normalize();

			Vector Intersect = RayPlaneIntersect(Vector(0.f,0.f,0.f), Vector(0.f, 0.f, 1.f), pCamera->GetTransformedWTM_().t, CamDir);

			m_totalForce -= Intersect - GetTransformedWTM_().t;

			m_totalForce.normalize();

			m_totalForce *= 5.f;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool CPawnCueBall::ProcessButton(const EventInput &InputData)
{
	bool bResult = false;

	if (InputData.Key == KEY_LEFT)
	{
		m_totalForce += Vector(-140.f, 0.f, 0.f);
		bResult = true;
	}

	if (InputData.Key == KEY_RIGHT)
	{
		m_totalForce += Vector(140.f, 0.f, 0.f);
		bResult = true;
	}

	if (InputData.Key == KEY_UP)
	{
		m_totalForce += Vector(0.f, 10.f, 0.f);
		bResult = true;
	}

	if (InputData.Key == KEY_RIGHT)
	{
		m_totalForce += Vector(0.f, -10.f, 0.f);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::Update(float fDeltaTime)
{
	// apply all forces
	Vector Fg = Vector(0.f, m_EMoveDirection == EMoveDirectionDown ? -10.f : 10.f, 0.f); // gravity force

	m_totalForce += Fg /* fDeltaTime*/;

	m_linearVelocity += m_totalForce * fDeltaTime;

	//GetAppMain()->GetPhysicsEngine()->SetCentralImpulse(m_pSphereController->GetPNode(), &m_totalForce);
	//GetAppMain()->GetPhysicsEngine()->SetCentralForce(m_pSphereController->GetPNode(), &m_totalForce);

	m_totalForce.Set(0.f, 0.f, 0.f);
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::PostPhysUpdate(float DeltaTime)
{
	//m_pSphereController->ContactTest();
}

//----------------------------------------------------------------------------------------------
void CPawnCueBall::PostPhysEvent(EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB)
{
	switch (EVENT_ID)
	{
	case EPhysEventContact:
		{
			CActor *pOtherActor = (CActor*)(pParamA);
			assert(pOtherActor);

			//pOtherActor->Release();

			for (std::vector<oes::common_base::IRenderInterface*>::iterator Iter = pOtherActor->m_VecRenderEntities.begin();
				Iter != pOtherActor->m_VecRenderEntities.end(); ++Iter)
			{
				(*Iter)->SetFlags((*Iter)->GetFlags() + 1);
			}

			//std::for_each(pOtherActor->m_VecCollisionEntities.begin(), pOtherActor->m_VecCollisionEntities.end(), std::bind2nd(std::mem_fun(&ICollisionInterface::Enable), false));

		}break;

	case EPhysEventContactsSum:
		{
			Vector *pVec = (Vector*)(pParamA);
			Vector *pN = (Vector*)(pParamB);

			if (m_EMoveDirection == EMoveDirectionDown && pN->y > 0.f){
				m_EMoveDirection = EMoveDirectionUp;
			}
			else if (m_EMoveDirection == EMoveDirectionUp && pN->y < 0.f){
				m_EMoveDirection = EMoveDirectionDown;
			}
		}break;

	default:
		break;
	}
}