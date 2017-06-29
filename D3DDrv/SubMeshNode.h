
#pragma once

#ifdef WIN32
#pragma warning(disable: 4251)
#endif

#include "dxstdafx.h"
#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class SRenderContext;
        class SubMeshNode;

        typedef std::vector<SubMeshNode*> TVecMeshes;

        struct D3DDRIVER_API SVMeshNodes
        {
            TVecMeshes MeshNodes;
        };
        typedef std::vector<SVMeshNodes> TSVMeshNodes;

        struct SMeshNodesBank
        {
            TSVMeshNodes	VBankNodes;
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        class D3DDRIVER_API CSceneMeshNode : public Refcount
        {
        public:
            CSceneMeshNode(D3DDriver *pDriver);
            virtual ~CSceneMeshNode();

            std::vector<SMeshNodesBank>&	GetMeshBank() { return m_VMeshBank; }

            TVecMeshes&		GetSubMeshNodes() { return m_MeshNodes; }
            TSVMeshNodes&	GetSubMeshNodesLOD() { return m_VMeshNodesLOD; }
            TVecMeshes&		GetSubMeshDestrNodes() { return m_MeshDestrNodes; }
            TSVMeshNodes&	GetSubMeshDestrNodesLOD() { return m_VMeshDestrNodesLOD; }

            void SetSceneBBox(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
            {
                m_Scene_aabb_min_x = min_x;
                m_Scene_aabb_min_y = min_y;
                m_Scene_aabb_min_z = min_z;

                m_Scene_aabb_max_x = max_x;
                m_Scene_aabb_max_y = max_y;
                m_Scene_aabb_max_z = max_z;
            }

            void GetSceneBBox(float *out_min_x, float *out_min_y, float *out_min_z, float *out_max_x, float *out_max_y, float *out_max_z)
            {
                *out_min_x = m_Scene_aabb_min_x;
                *out_min_y = m_Scene_aabb_min_y;
                *out_min_z = m_Scene_aabb_min_z;

                *out_max_x = m_Scene_aabb_max_x;
                *out_max_y = m_Scene_aabb_max_y;
                *out_max_z = m_Scene_aabb_max_z;
            }

            void SetModelsBBox(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
            {
                m_Models_aabb_min_x = min_x;
                m_Models_aabb_min_y = min_y;
                m_Models_aabb_min_z = min_z;

                m_Models_aabb_max_x = max_x;
                m_Models_aabb_max_y = max_y;
                m_Models_aabb_max_z = max_z;
            }

            void GetModelsBBox(float *out_min_x, float *out_min_y, float *out_min_z, float *out_max_x, float *out_max_y, float *out_max_z)
            {
                *out_min_x = m_Models_aabb_min_x;
                *out_min_y = m_Models_aabb_min_y;
                *out_min_z = m_Models_aabb_min_z;

                *out_max_x = m_Models_aabb_max_x;
                *out_max_y = m_Models_aabb_max_y;
                *out_max_z = m_Models_aabb_max_z;
            }

            SRenderContext* GetOwnerContext() const { return m_pRenderContext; }

        protected:
            virtual void DoRelease();

        private:
            D3DDriver *m_p3DDriver;

            SRenderContext *m_pRenderContext;

            std::vector<SMeshNodesBank>		m_VMeshBank; // Dstr0[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]],
                                                         // Dstr1[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]],
                                                         // Dstr2[ Lod0[Node0, ...], Lod1[Node0, ...], Lod2[...]], 
            // meshes
            TVecMeshes		m_MeshNodes;
            TSVMeshNodes	m_VMeshNodesLOD;
            TVecMeshes		m_MeshDestrNodes;
            TSVMeshNodes	m_VMeshDestrNodesLOD;

            float m_Scene_aabb_min_x;
            float m_Scene_aabb_min_y;
            float m_Scene_aabb_min_z;
            float m_Scene_aabb_max_x;
            float m_Scene_aabb_max_y;
            float m_Scene_aabb_max_z;

            float m_Models_aabb_min_x;
            float m_Models_aabb_min_y;
            float m_Models_aabb_min_z;
            float m_Models_aabb_max_x;
            float m_Models_aabb_max_y;
            float m_Models_aabb_max_z;
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        class D3DDRIVER_API SubMeshNode
        {
        public:
            SubMeshNode(D3DDriver	*pDriver);
            virtual ~SubMeshNode();

            virtual void InitializeVertexData(const D3DDriver *pD3DInterface,
                float *vertexes,
                float *normals,
                float *colors,
                float *tangents,
                float *binormals,
                unsigned int *faces_idx,
                unsigned int NumVert,
                unsigned int NumFaces,
                float *texcoords,
                unsigned int num_texcoord_sets,
                bool bSkin,
                float	*weights,			// vertex weights
                unsigned int *bone_idxs,
                std::vector<int> &boneRemapper,
                unsigned int & numDistinctBones);

            virtual bool IsEmpty() const { return m_Vertex_VBOID == -1; }
            virtual void AddBoneIdx(int BoneIdx) { m_Bones.push_back(BoneIdx); }

            virtual void RenderMesh(const D3DDriver * D3DInterface);
            virtual void RenderNormals(const D3DDriver * D3DInterface);
            virtual void RenderSBuffer(const D3DDriver * D3DInterface);
            virtual void RenderTBuffer(const D3DDriver * D3DInterface);
            virtual void RenderSxTBuffer(const D3DDriver * D3DInterface);

            virtual bool CreateVertexBuffers(const D3DDriver * D3DInterface, unsigned int NumVertices, unsigned int NumTriangles, bool bGenerateStrips = false);

            void SetBBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
            void GetBBox(float* minX, float* minY, float* minZ, float* maxX, float* maxY, float* maxZ) const;

            // clear data
            void Clear();

        protected:
            virtual void DoRelease();
            virtual bool DoCreateVertexBuffers(const D3DDriver * D3DInterface, unsigned int NumVertices, unsigned int NumTriangles, bool bGenerateStrips = false);
            virtual void SetSizeBonesMat();

        public:
            unsigned int            m_NumVertices;
            unsigned int            m_NumTriangles;

            D3DVertex			   *m_pVertices;
            unsigned short		   *m_pIndices;
            uint32_t			   *m_pIndices32;

            GLuint					m_Vertex_VBOID;			// Vertex Buffer Object ID
            GLuint					m_Index_VBOID;			// Index Buffer Object ID
            GLuint					m_N_VBOID;
            GLuint					m_S_VBOID;
            GLuint					m_T_VBOID;
            GLuint					m_SxT_VBOID;

            // boolean values that describe the information available for every vertex of every mesh
            bool					m_VertexHasNormal;
            bool					m_VertexHasColor;
            bool					m_VertexHasTexture;
            bool					m_VertexHasS;
            bool					m_VertexHasT;
            bool					m_VertexHasSxT;
            bool					m_VertexHasTangentBasis;

            bool					m_IsSkinned;

            std::vector<int>		m_Bones;
            //std::vector<Matrix4f>	m_BoneMatrices;

            // boundering box
            SVector3f				m_AABB_Min;
            SVector3f				m_AABB_Max;

            unsigned int			m_materialID;

            D3DDriver			*p3DDriver;
        };

        //----------------------------------------------------------------------------------------------
        class D3DDRIVER_API SubMeshAgregator : public Refcount
        {
        public:
            SubMeshAgregator(D3DDriver *pDriver);
            virtual ~SubMeshAgregator();

            std::vector<SubMeshNode*> MeshNodes;
        };
    }
}