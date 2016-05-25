#include "StdafxRender.h"

static const bool V_FALSE = false;
static const unsigned int IDX_ZERO = 0;

REGISTER_CLASS_A(Comp_StaticMesh, CRenderObject)
	new PropertyBOOL("bShowNormals", (BYTE*)&((Comp_StaticMesh*)NULL)->m_bShowNormals - (BYTE*)NULL, "Comp_StaticMesh", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("bShowTangent", (BYTE*)&((Comp_StaticMesh*)NULL)->m_bShowTangent - (BYTE*)NULL, "Comp_StaticMesh", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyBOOL("bShowBinormal", (BYTE*)&((Comp_StaticMesh*)NULL)->m_bShowBinormal - (BYTE*)NULL, "Comp_StaticMesh", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_FALSE),
	new PropertyUINT("nForceLODLvl", (BYTE*)&((Comp_StaticMesh*)NULL)->m_nForceLODLvl - (BYTE*)NULL, "Comp_StaticMesh", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &IDX_ZERO),
	new PropertyUINT("nDestrLvl", (BYTE*)&((Comp_StaticMesh*)NULL)->m_nDestrLvl - (BYTE*)NULL, "Comp_StaticMesh", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &IDX_ZERO),
END_REGISTER_CLASS(Comp_StaticMesh, CRenderObject);

//----------------------------------------------------------------------------------------------
Comp_StaticMesh::Comp_StaticMesh(const CObjectAbstract * Parent)
: Super(Parent)
, m_bShowNormals(V_FALSE)
, m_bShowTangent(V_FALSE)
, m_bShowBinormal(V_FALSE)
{
	m_pMesh = new D3DMeshAllocator(GetRenderComponent()->GetRenderDriver());
}

//----------------------------------------------------------------------------------------------
Comp_StaticMesh::~Comp_StaticMesh()
{
	delete m_pMesh;
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::OnPropertyChanged(const char *PropertyName)
{
	if (!strcmp(PropertyName, "bShowNormals")){
		m_pMesh->SetShowNormals(m_bShowNormals);
	}
	else if (!strcmp(PropertyName, "bShowTangent")){
		m_pMesh->SetShowTangent(m_bShowTangent);
	}
	else if (!strcmp(PropertyName, "bShowBinormal")){
		m_pMesh->SetShowBinormal(m_bShowBinormal);
	}
	else if (!strcmp(PropertyName, "nForceLODLvl")){
		m_pMesh->SetForceLODLvl(m_nForceLODLvl);
	}
	else if (!strcmp(PropertyName, "nDestrLvl")){
		m_pMesh->SetDestructLvl(m_nDestrLvl);
	}
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::LoadMesh(const SMeshMaterialSet &Desc)
{
	Clear();

	m_pMesh->LoadFromDesc(Desc);

	if (CSceneMeshNode *pScene = m_pMesh->GetSceneMeshNode())
	{
		Bounds3f modelBBox;

		pScene->GetModelsBBox(&modelBBox.bound_min.x, &modelBBox.bound_min.y, &modelBBox.bound_min.z,
								&modelBBox.bound_max.x, &modelBBox.bound_max.y, &modelBBox.bound_max.z);
		SetRBounds_(modelBBox);
	}
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::Clear()
{
	m_pMesh->Clear();
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::Render() const
{
	GetRenderComponent()->GetRenderDriver()->SetWorldMatrix(m_WorldMatrixTransform.GetPtr());

	m_pMesh->Render();
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetShowDebug(bool Value)
{
	m_pMesh->SetShowDebug(Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetShowNormals(bool Value)
{
	m_pMesh->SetShowNormals(m_bShowNormals = Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetShowTangent(bool Value)
{
	m_bShowTangent = Value;
	m_pMesh->SetShowTangent(m_bShowTangent);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetShowBinormal(bool Value)
{
	m_bShowBinormal = Value;
	m_pMesh->SetShowBinormal(m_bShowBinormal);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetCastShadows(bool Value)
{
	Super::SetCastShadows(Value);
	m_pMesh->SetCastShadows(Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetReciveShadows(bool Value)
{
	Super::SetReciveShadows(Value);
	m_pMesh->SetReciveShadows(Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetForceLODLvl(unsigned int Value) 
{
	IRenderInterface::SetForceLODLvl(Value);
	m_pMesh->SetForceLODLvl(Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::SetDestructLvl(unsigned int Value) 
{
	IRenderInterface::SetDestructLvl(Value);
	m_pMesh->SetDestructLvl(Value);
}

//----------------------------------------------------------------------------------------------
void Comp_StaticMesh::ReloadMesh()
{
#ifndef USE_STEREAM_ENGINE
	std::string sFilename;
	if (FindFile(m_filename.c_str(), &sFilename))
	{
		//m_pMesh->LoadFromFile(sFilename);
	}
#else
	DWORD ThreadId = ::GetCurrentThreadId();

	if (ThreadId != GetRenderComponent()->ThreadRenderID)
	{
		std::string sFilenameMat;
		if (FindFile(m_filename.c_str(), &sFilenameMat))
		{
			// @TODO: this object can not init BBox immidiatly so we need to put this object to quewe
			GetRenderComponent()->PushToRenderStack(new Command<Comp_StaticMesh, Comp_StaticMesh>(&Comp_StaticMesh::ReloadMesh, this), true);
		}
	}
	else
	{
		std::string sFilename;
		if (FindFile(m_filename.c_str(), &sFilename))
		{
			pMesh->LoadFromFile(sFilename);
		}
	}
#endif//USE_STEREAM_ENGINE
}