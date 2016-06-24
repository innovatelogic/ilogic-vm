#include "coresdkafx.h"

static const bool	V_TRUE = true;

REGISTER_CLASS_A(CRestrictedVolume, ActorAllocator)
	new PropertyBOOL("bSideBT", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[0] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bSideTP", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[1] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bSideX+", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[2] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bSideX-", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[3] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bSideZ+", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[4] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
	new PropertyBOOL("bSideZ-", (BYTE*)&((CRestrictedVolume*)NULL)->m_bSides[5] - (BYTE*)NULL, "CRestrictedVolume", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &V_TRUE),
ASSEMBLE_PROPS(CRestrictedVolume)
BEGIN_INTERFACE_DECLARATION(CRestrictedVolume)
	new SInterfaceDecl("IDrawInterface", (BYTE*)&((CRestrictedVolume*)NULL)->___startObjectMarkerIDrawInterface - (BYTE*)NULL),
ASSEMBLE_INTERFACES(CRestrictedVolume)
CLASS_ENDL(CRestrictedVolume)
CLASS_INSTANCE_EX(CRestrictedVolume, ActorAllocator);

//----------------------------------------------------------------------------------------------
CRestrictedVolume::CRestrictedVolume(const CObjectAbstract *pParent)
: Super(pParent)
, IDrawInterface(pParent)
{
	RegisterDrawInterface(this);

	NEW_OBJECT_TRANSIENT_CHILD(m_pCollisionObject, CRestrictedVolumeCollisionController, "CollisionObject", this);

	memset(&m_bSides, V_TRUE, sizeof(bool) * 6);
}

//----------------------------------------------------------------------------------------------
CRestrictedVolume::~CRestrictedVolume()
{

}

//----------------------------------------------------------------------------------------------
void CRestrictedVolume::OnPropertyChanged(const char *pPropertyName)
{
	Super::OnPropertyChanged(pPropertyName);

	if (!strcmp(pPropertyName, "bSideBT") ||
		!strcmp(pPropertyName, "bSideTP") ||
		!strcmp(pPropertyName, "bSideX+") ||
		!strcmp(pPropertyName, "bSideX-") ||
		!strcmp(pPropertyName, "bSideZ+") ||
		!strcmp(pPropertyName, "bSideZ-"))
	{	
		m_pCollisionObject->ReleasePhysActor();
		InitPhysics();
	}
}

//----------------------------------------------------------------------------------------------
void CRestrictedVolume::Initialize()
{
	Super::Initialize();

	InitPhysics();
}

//----------------------------------------------------------------------------------------------
void CRestrictedVolume::DoDraw()
{
	/*if (IsFocused())
	{
		const float k = 20.f;

		Matrix3f STM = GetSTM_();
		Vector Ext(STM.a00, STM.a11, STM.a22);

		std::vector <Matrix> vMtx;
		
		vMtx.push_back(Matrix(1.f, 0.f, 0.f, 0.f,
							  0.f, 1.f, 0.f, 0.f,
							  0.f, 0.f, 1.f, 0.f,
							  0.f, 0.f, 0.f, 1.f));
		
		vMtx.push_back(Matrix(-1.f, 0.f, 0.f, 0.f,
							  0.f, -1.f, 0.f, 0.f,
							  0.f, 0.f, 1.f, 0.f,
							  0.f, Ext.y, 0.f, 1.f));

		vMtx.push_back(Matrix(0.f, 1.f, 0.f, 0.f,
							  -1.f, 0.f, 0.f, 0.f,
							  0.f, 0.f, 1.f, 0.f,
							  Ext.x * 0.5f, 0.f, 0.f, 1.f));

		vMtx.push_back(Matrix(0.f, -1.f, 0.f, 0.f,
							  1.f, 0.f, 0.f, 0.f,
							  0.f, 0.f, 1.f, 0.f,
							 -Ext.x * 0.5f, 0.f, 0.f, 1.f));

		vMtx.push_back(Matrix(1.f, 0.f, 0.f, 0.f,
							  0.f, 0.f, -1.f, 0.f,
							  0.f, -1.f, 0.f, 0.f,
							  0.f, 0.f, Ext.z * 0.5f, 1.f));

		vMtx.push_back(Matrix(1.f, 0.f, 0.f, 0.f,
								0.f, 0.f, 1.f, 0.f,
								0.f, -1.f, 0.f, 0.f,
								0, 0.f, -Ext.z * 0.5f, 1.f));

		for (int Index = 0; Index < 6; ++Index)
		{
			if (!m_bSides[Index]){
				continue;
			}

			Matrix Mat = GetTransformedWTM_() * vMtx[Index];

			Mat._row0.normalize();
			Mat._row1.normalize();
			Mat._row2.normalize();
								
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
		}
	}*/
}

//----------------------------------------------------------------------------------------------
bool CRestrictedVolume::InitPhysics()
{
	Matrix3f STM = GetSTM_();
	
	m_pCollisionObject->CreatePhysActor(GetTransformedWTM_(), Vector(STM.a00, STM.a11, STM.a22), m_bSides);

	return true;
}