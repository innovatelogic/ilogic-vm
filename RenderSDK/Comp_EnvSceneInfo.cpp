#include "StdafxRender.h"

static const float fDefFogMin = 0.f;
static const float fDefFogMax = 100.f;
static const float fDefFogDensity = 0.02f;
static unsigned int DefFogColor = 0x0000ffff;

REGISTER_CLASS_A(CComp_EnvSceneInfo, CRenderObject)
	new PropertyFLOAT("FogMin", (BYTE*)&((CComp_EnvSceneInfo*)NULL)->m_fFogMin - (BYTE*)NULL, "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogMin),
	new PropertyFLOAT("FogMax", (BYTE*)&((CComp_EnvSceneInfo*)NULL)->m_fFogMax - (BYTE*)NULL, "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogMax),
	new PropertyFLOAT("FogDensity", (BYTE*)&((CComp_EnvSceneInfo*)NULL)->m_fFogDensity - (BYTE*)NULL, "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogDensity),
	new PropertyUINT("FogColor", (BYTE*)&((CComp_EnvSceneInfo*)NULL)->m_FogColor - (BYTE*)NULL, "CComp_EnvSceneInfo",	"Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &DefFogColor),
END_REGISTER_CLASS(CComp_EnvSceneInfo, CRenderObject);

//------------------------------------------------------------------------
CComp_EnvSceneInfo::CComp_EnvSceneInfo(const CObjectAbstract * Parent)
: Super(Parent)
, m_fFogMin(fDefFogMin)
, m_fFogMax(fDefFogMax)
, m_fFogDensity(fDefFogDensity)
, m_FogColor(DefFogColor)
{
	
}

//------------------------------------------------------------------------
CComp_EnvSceneInfo::CComp_EnvSceneInfo(const CComp_EnvSceneInfo & Source)
: Super(Source)
{
	if (this != &Source)
	{
	}
}

//------------------------------------------------------------------------
CComp_EnvSceneInfo::~CComp_EnvSceneInfo()
{

}