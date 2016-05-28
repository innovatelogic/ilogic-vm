/**********************************************************************
 *<
	FILE:			ShapeShareSplitter.cpp

	DESCRIPTION:	Class to calculate the shape share uniqueness on the
					basis of the supplied particle data channels. If a 
					particle data container has channels for particle 
					shape, material index, and mapping, all these data
					channels may have different sharing/globality pro-
					perties. The class is given different particle data
					channels, and it calculates the aggregated unique-
					ness for particle shapes.
											 
	CREATED BY:		Oleg Bayborodin

	HISTORY:		created 11-24-12

 *>	Copyright (c) 2012, All Rights Reserved.
 **********************************************************************/

#include "ShapeShareSplitter.h"

#include <IParticleChannelMesh.h>
#include <IParticleChannelInt.h>
#include <IParticleChannelMeshMap.h>

// The following define is deliberately commented out
// to signify the reversal of the fix for the defect MAXX-7661;
// the define techique is used to mark all the places
// related to the switch-back of the defect MAXX-7661 fix.
// #define FIX_MAXX_7661

namespace PFActions {

ShapeShareSplitter::ShapeShareSplitter(const int count)
	:	m_count(count)
	,	m_chShape(nullptr)
	,	m_chMtlIndex(nullptr)
	,	m_chMapping(nullptr)
	,	m_shareType(kUndefined)
	,	m_init(false)
{
}

ShapeShareSplitter::~ShapeShareSplitter()
{
	for(size_t i=0; i<m_meshes.size(); ++i)
	{
		delete m_meshes[i];
		m_meshes[i] = nullptr;
	}
}

bool ShapeShareSplitter::Init(const IParticleChannelMeshR* chShape, 
							 const IParticleChannelIntR* chMtlIndex,
							 IParticleChannelMeshMapR* chMapping,
							 int numMaterials)
{
	if (m_init)
		return false;
	m_init = true;

	if ((chShape == nullptr) || (m_count <= 0))
		return true; // m_shareType: kUndefined by default
	m_chShape = chShape;
	m_shareType = kPassThru;

	if ((chMtlIndex == nullptr) && (chMapping == nullptr))
		return true;
	m_chMtlIndex = chMtlIndex;
	m_chMapping = chMapping;

	// is output shape global?
	bool isGlobal = chShape->IsShared() && (chShape->GetValueCount() <= 1);
	isGlobal = isGlobal && 
		((chMtlIndex ? chMtlIndex->IsGlobal() : true) 
		|| (numMaterials <= 1)); //If only 1 material applied to the particle system, we don't need to split the mesh.
#ifdef FIX_MAXX_7661
	isGlobal = isGlobal && mappingGlobal(chMapping);
#endif

	// no sharing at all for the output shape?
	bool isNotShared = !chShape->IsShared() || (chShape->GetValueCount() >= m_count) || materialNotShared(chMtlIndex);
#ifdef FIX_MAXX_7661
	isNotShared = isNotShared || mappingNotShared(chMapping);
#endif

	if (isGlobal) {
		initGlobalMesh();
	} else if (isNotShared) {
		initNonSharedMeshes();
	} else {
		initSharedMeshes(chShape, chMtlIndex, chMapping);
	}
	return true;
}

Mesh* ShapeShareSplitter::GetShape(int particleIndex)
{
	Mesh* pRes = nullptr;
	switch( m_shareType )
	{
	case kGlobal:
		pRes = m_meshes[0];
		break;
	case kTrueShare:
		pRes = getSharedMesh(particleIndex);
		break;
	case kIndexThru:
		pRes = getNonSharedMesh(particleIndex);
		break;
	case kPassThru:
		pRes = const_cast<Mesh*>(m_chShape->GetValue(particleIndex));
		break;
	}
	return pRes;
}

bool ShapeShareSplitter::mappingGlobal(IParticleChannelMeshMapR* chMapping) const
{
	if (chMapping == nullptr)
		return true;

	for(int mp=0; mp<chMapping->GetNumMaps(); ++mp)
	{
		if (chMapping->MapSupport(mp))
		{
			const IParticleChannelMapR* chMapR = chMapping->GetMapReadChannel(mp);
			if (chMapR != nullptr)
			{	
				if (!chMapR->IsUVVertShared() || (chMapR->GetUVVertCount() > 1))
					return false;
				if (!chMapR->IsTVFaceShared() || (chMapR->GetTVFaceCount() > 1))
					return false;
			}
		}
	}

	return true;
}

bool ShapeShareSplitter::materialNotShared(const IParticleChannelIntR* chMtlIndex) const
{
	if (chMtlIndex == nullptr)
		return false;
	if (chMtlIndex->IsGlobal())
		return false;

	std::map<int, int> mtlToParticle;
	for(int i=0; i<m_count; ++i)
	{
		int mtlIndex = chMtlIndex->GetValue(i);
		if (mtlToParticle.find(mtlIndex) != mtlToParticle.end())
			return false;
		mtlToParticle[mtlIndex] = i;
	}

	return true;
}

bool ShapeShareSplitter::mappingNotShared(IParticleChannelMeshMapR* chMapping) const
{
	if (chMapping == nullptr)
		return false;

	for(int mp=0; mp<chMapping->GetNumMaps(); ++mp)
	{
		if (chMapping->MapSupport(mp))
		{
			const IParticleChannelMapR* chMapR = chMapping->GetMapReadChannel(mp);
			if (chMapR != nullptr)
			{	
				if (!chMapR->IsUVVertShared() || !chMapR->IsTVFaceShared())
					return true;
			}
		}
	}

	return false;
}

void ShapeShareSplitter::initGlobalMesh()
{
	m_shareType = kGlobal;
	Mesh* pModified = createModifiedMesh(0);
	m_meshes.push_back(pModified);

	m_UVs.clear();
	if(m_chMapping)
	{
		for(int mp=0; mp<m_chMapping->GetNumMaps(); ++mp) 
		{
			if (!m_chMapping->MapSupport(mp)) 
				continue;	
			IParticleChannelMapR* map = m_chMapping->GetMapReadChannel(mp);
			if (map == nullptr) 
				continue;
			
			if(map)
			{
				for (int i=0;i<m_count;i++)
				{
					TabUVVert* uvvert = const_cast <TabUVVert*>(map->GetUVVert(i));
					m_UVs.push_back(Point3((*uvvert)[0].x,(*uvvert)[0].y,0.0f));
				}
			}
			
		}
	}


}

class ShapeShareIndex
{
public:
	ShapeShareIndex(int particleIndex,
					const IParticleChannelMeshR* chShape, 
					const IParticleChannelIntR* chMtlIndex,
					const size_t mappingDim,
					IParticleChannelMeshMapR* chMapping)
	{
		const size_t kMinIndicesSize = 2;
		mIndices.resize(kMinIndicesSize + mappingDim);
		mIndices[0] = chShape ? chShape->GetValueIndex(particleIndex) : -1;
		mIndices[1] = chMtlIndex ? chMtlIndex->GetValue(particleIndex) : -1;
		if (chMapping)
		{
			size_t index = kMinIndicesSize;
			for(int mp=0; mp<chMapping->GetNumMaps(); ++mp)
			{
				if (chMapping->MapSupport(mp))
				{
					const IParticleChannelMapR* chMapR = chMapping->GetMapReadChannel(mp);
					if (chMapR != nullptr)
					{
						mIndices[index++] = chMapR->GetUVVertIndex(particleIndex);
						mIndices[index++] = chMapR->GetTVFaceIndex(particleIndex);
					}
				}
			}
		}
	}

	bool operator<(const ShapeShareIndex& right) const
	{
		DbgAssert(mIndices.size() == right.mIndices.size());

		for(int i=0; i<mIndices.size(); ++i)
		{
			if (mIndices[i] < right.mIndices[i])
				return true;
		}
		return false;
	}

private:
	ShapeShareIndex(); // deliberatly private and undefined

	std::vector<int> mIndices;
};

size_t MappingDim(IParticleChannelMeshMapR* chMapping)
{
	size_t res = 0;
	if (chMapping)
	{
		for(int mp=0; mp<chMapping->GetNumMaps(); ++mp)
		{
			if (chMapping->MapSupport(mp))
			{
				if (chMapping->GetMapReadChannel(mp) != nullptr)
					++res;
			}
		}
	}
	return res;
}

void ShapeShareSplitter::initSharedMeshes(const IParticleChannelMeshR* chShape, 
										  const IParticleChannelIntR* chMtlIndex,
										  IParticleChannelMeshMapR* chMapping)
{
	typedef std::map<ShapeShareIndex,size_t,std::less<ShapeShareIndex>> ShapeMapType;
	ShapeMapType shapeMap;
	const size_t mappingDim = MappingDim(chMapping);
	size_t shapeIndex = 0;

	for(int i=0; i<m_count; ++i)
	{
#ifdef FIX_MAXX_7661
		ShapeShareIndex curSSI(i, chShape, chMtlIndex, mappingDim, chMapping);
#else
		ShapeShareIndex curSSI(i, chShape, chMtlIndex, 0, nullptr);
#endif
		ShapeMapType::const_iterator iter = shapeMap.find(curSSI);
		if (iter == shapeMap.end())
		{
			shapeMap[curSSI] = shapeIndex;
			m_indexShapeMap[i] = shapeIndex;
			++shapeIndex;
		}
		else
		{
			m_indexShapeMap[i] = iter->second;
		}
	}

	m_shareType = kTrueShare;
	m_meshes.resize(shapeIndex, nullptr);
}

void ShapeShareSplitter::initNonSharedMeshes()
{
	m_shareType = kIndexThru;
	m_meshes.resize(size_t(m_count), nullptr);	
}

Mesh* ShapeShareSplitter::createModifiedMesh(int particleIndex) const
{
	const Mesh* pMesh = m_chShape->GetValue(particleIndex);
	Mesh* pModified = pMesh ? new Mesh(*pMesh) : nullptr;

	const bool assignMtl = AssignMeshMaterialIndices(pModified, m_chMtlIndex, particleIndex);
	const bool assignMap = AssignMeshMapping(pModified, m_chMapping, particleIndex);

	if ((pModified != nullptr) && (assignMtl || assignMap))
	{
		pModified->InvalidateStrips();
	}

	return pModified;
}

Mesh* ShapeShareSplitter::getSharedMesh(int particleIndex)
{
	std::map<int, size_t>::const_iterator it = m_indexShapeMap.find(particleIndex);

	DbgAssert(it != m_indexShapeMap.end());
	if (it == m_indexShapeMap.end())
		return nullptr;

	const size_t shapeIndex = it->second;
	return getMeshByIndices(shapeIndex, particleIndex);
}

Mesh* ShapeShareSplitter::getNonSharedMesh(int particleIndex)
{
	return getMeshByIndices(size_t(particleIndex), particleIndex);
}

Mesh* ShapeShareSplitter::getMeshByIndices(const size_t shapeIndex, int particleIndex)
{
	Mesh* pRes = m_meshes[shapeIndex];
	if (pRes == nullptr)
	{
		pRes = createModifiedMesh(particleIndex);
		m_meshes[shapeIndex] = pRes;
	}
	return pRes;
}

size_t ShapeShareSplitter::GetUVDataCount()
{
	return m_UVs.size();
}

Point3* ShapeShareSplitter::GetUVData()
{
	return m_UVs.data();
}

bool AssignMeshMaterialIndices(Mesh* mesh, const IParticleChannelIntR* chMtlIndex, int particleIndex)
{
	if ((chMtlIndex == nullptr) || (mesh == nullptr))
		return false;

	int mtlIndex = chMtlIndex->GetValue(particleIndex);
	if (mtlIndex >= 0) 
	{
		for(int i=0; i<mesh->getNumFaces(); ++i)
			mesh->setFaceMtlIndex(i, mtlIndex);
		return true;
	}
	return false;
}

bool AssignMeshMapping(Mesh* mesh, IParticleChannelMeshMapR* chMapping, int particleIndex)
{
	if ((chMapping == nullptr) || (mesh == nullptr))
		return false;

	bool hasMappingAssigned = false;
	for(int mp=0; mp<chMapping->GetNumMaps(); ++mp) 
	{
		if (!chMapping->MapSupport(mp)) 
			continue;	
		IParticleChannelMapR* map = chMapping->GetMapReadChannel(mp);
		if (map == nullptr) 
			continue;
		map->Apply(mesh, particleIndex, mp);
		hasMappingAssigned = true;
	}
	return hasMappingAssigned;
}

} // end of namespace PFActions