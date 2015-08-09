#include "d3ddriverstdafx.h"

//----------------------------------------------------------------------------------------------
CMaterialEffectFont::CMaterialEffectFont(const class D3DDriver *pInterface)
: BaseMaterial(pInterface)
{

}

//----------------------------------------------------------------------------------------------
CMaterialEffectFont::CMaterialEffectFont(const CMaterialEffectFont &Source)
: BaseMaterial(Source)
{

}

//----------------------------------------------------------------------------------------------
CMaterialEffectFont::~CMaterialEffectFont()
{

}

//----------------------------------------------------------------------------------------------
bool CMaterialEffectFont::Release()
{
	return true;
}

//----------------------------------------------------------------------------------------------
void CMaterialEffectFont::Affect()
{

}
