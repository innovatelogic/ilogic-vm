#include "StdafxRender.h"

static const float fDefFogMin = 0.f;
static const float fDefFogMax = 100.f;
static const float fDefFogDensity = 0.02f;
static unsigned int DefFogColor = 0x0000ffff;

REGISTER_CLASS_A(CComp_EnvSceneInfo, CRenderObject)
	new PropertyFLOAT("FogMin", DATAFIELD_OFFSET(CComp_EnvSceneInfo, m_fFogMin), "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogMin),
	new PropertyFLOAT("FogMax", DATAFIELD_OFFSET(CComp_EnvSceneInfo, m_fFogMax), "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogMax),
	new PropertyFLOAT("FogDensity", DATAFIELD_OFFSET(CComp_EnvSceneInfo, m_fFogDensity), "CComp_EnvSceneInfo", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &fDefFogDensity),
	new PropertyUINT("FogColor", DATAFIELD_OFFSET(CComp_EnvSceneInfo, m_FogColor), "CComp_EnvSceneInfo",	"Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, &DefFogColor),
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