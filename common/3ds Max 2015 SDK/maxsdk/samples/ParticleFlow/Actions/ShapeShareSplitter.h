/**********************************************************************
 *<
	FILE:			ShapeShareSplitter.h

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

#pragma once

#include <Noncopyable.h>
#include <map>
#include <vector>
#include <point3.h>

// forward declares
class Mesh;
class IParticleChannelMeshR;
class IParticleChannelIntR;
class IParticleChannelMeshMapR;

namespace PFActions {

class ShapeShareSplitter:	public MaxSDK::Util::Noncopyable 
{
	public:
		ShapeShareSplitter(const int count);
		~ShapeShareSplitter();

		bool Init(const IParticleChannelMeshR* chShape,
				  const IParticleChannelIntR* chMtlIndex,
				  IParticleChannelMeshMapR* chMapping,
				  int numMaterials);
		Mesh* GetShape(int particleIndex);

		size_t GetUVDataCount();
		Point3* GetUVData();
	private:
		ShapeShareSplitter(); // deliberatly private and undefined

		bool mappingGlobal(IParticleChannelMeshMapR* chMapping) const;
		bool materialNotShared(const IParticleChannelIntR* chMtlIndex) const;
		bool mappingNotShared(IParticleChannelMeshMapR* chMapping) const;
		void initGlobalMesh();
		void initSharedMeshes(const IParticleChannelMeshR* chShape, 
								const IParticleChannelIntR* chMtlIndex,
								IParticleChannelMeshMapR* chMapping);
		void initNonSharedMeshes();
		Mesh* createModifiedMesh(int particleIndex) const;
		Mesh* getSharedMesh(int particleIndex);
		Mesh* getNonSharedMesh(int particleIndex);
		Mesh* getMeshByIndices(const size_t shapeIndex, int particleIndex);

		std::map<int, size_t> m_indexShapeMap;
		std::vector<Mesh*> m_meshes;

		const int m_count;
		const IParticleChannelMeshR* m_chShape;
		const IParticleChannelIntR* m_chMtlIndex;
		IParticleChannelMeshMapR* m_chMapping;
		enum ShareType {
			kUndefined,
			kGlobal,
			kTrueShare,
			kIndexThru,
			kPassThru
		}	  m_shareType;
		bool  m_init;
		std::vector<Point3> m_UVs;
};

bool AssignMeshMaterialIndices(Mesh* mesh, const IParticleChannelIntR* chMtlIndex, int particleIndex);
bool AssignMeshMapping(Mesh* mesh, IParticleChannelMeshMapR* chMapping, int particleIndex);

} // end of namespace PFActions
