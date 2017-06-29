#include "stdafxrender.h"

//----------------------------------------------------------------------------------------------
CFXMatManager::CFXMatManager(CRenderSDK * pRenderSDK)
: m_pRenderSDK(pRenderSDK)
{

}

//----------------------------------------------------------------------------------------------
CFXMatManager::~CFXMatManager()
{

}

//----------------------------------------------------------------------------------------------
void CFXMatManager::Release()
{

}

//----------------------------------------------------------------------------------------------
oes::d3d::MaterialEffect* CFXMatManager::GetMaterialInstance(const char *Name)
{
	std::string sFilename;  

	if (!FindFile(Name, &sFilename)){
		return NULL;
	}

    oes::d3d::MaterialEffect * pOutMatInstance = NULL;

	std::map<std::string, m_material>::iterator IterFind = m_MapMatPrototypes.find(sFilename.c_str());

	if (IterFind != m_MapMatPrototypes.end())
	{
		pOutMatInstance = oes::d3d::Alloc_MaterialEffect(m_pRenderSDK->GetRenderDriver());
		
		std::string sFXFilename;

		if (FindFile(IterFind->second.fx_file.c_str(), &sFXFilename))
		{
			if (pOutMatInstance->Load(sFXFilename.c_str()))
			{
				pOutMatInstance->SetTechniqueName(IterFind->second.tehnique.c_str());
			}
		}
	}
	else
	{		
		m_material OutMaterial;

		if (Load(sFilename.c_str(), OutMaterial))
		{
			pOutMatInstance = oes::d3d::Alloc_MaterialEffect(m_pRenderSDK->GetRenderDriver());

			std::string sFXFilename;

			if (FindFile(OutMaterial.fx_file.c_str(), &sFXFilename))
			{
				if (pOutMatInstance->Load(sFXFilename.c_str()))
				{
					pOutMatInstance->SetTechniqueName(OutMaterial.tehnique.c_str());
				}
			}

			m_MapMatPrototypes.insert(std::make_pair(sFilename, OutMaterial));
		}
	}
	
	return pOutMatInstance;
}

//----------------------------------------------------------------------------------------------
void CFXMatManager::ReleaseMaterial(oes::d3d::MaterialEffect *Inst)
{
	oes::d3d::Release_MaterialEffect(Inst);
}

//----------------------------------------------------------------------------------------------
bool CFXMatManager::Load(const char *Path, m_material &OutMaterial)
{
	bool bResult = false;

	LoadMaterial(Path, &OutMaterial);
	bResult = true;

	return bResult;
}