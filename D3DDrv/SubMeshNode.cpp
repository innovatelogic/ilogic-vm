#include "d3ddriverstdafx.h"

#undef D3DX_API

#define BAD_IDX             0xffffffff
//#define USE_NV_STRIP

// A helper macro to get a position
//#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#if defined(DEBUG) | defined(_DEBUG)
//#pragma comment(lib, "D:/Project/Lib/lib/bin.v2/libs/nvstrip/debug/nvtristrip.lib")
#else
//#pragma comment(lib, "../lib/bin.v2/libs/nvstrip/release/nvtristrip.lib")
#endif

template<class T> class Ptr {
public:
	Ptr(unsigned int n) { m_Ptr = (n ? new T[n] : 0); }
	~Ptr() { if (m_Ptr) delete [] m_Ptr; }
	T& operator[](unsigned int i) const { return m_Ptr[i]; }
	size_t sizeof_T() const { return sizeof(T); }
public:
	T* m_Ptr;
};
namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        CSceneMeshNode::CSceneMeshNode(D3DDriver *pDriver)
            : m_p3DDriver(pDriver)
        {
            m_pRenderContext = m_p3DDriver->GetCurrentContext();
        }

        //----------------------------------------------------------------------------------------------
        CSceneMeshNode::~CSceneMeshNode()
        {

        }

        //----------------------------------------------------------------------------------------------
        void CSceneMeshNode::DoRelease()
        {
            for (TVecMeshes::iterator Iter = m_MeshNodes.begin(); Iter != m_MeshNodes.end(); ++Iter)
            {
                (*Iter)->Clear();
                delete *Iter;
            }
            m_MeshNodes.clear();

            delete this;
        }

        //----------------------------------------------------------------------------------------------
        SubMeshAgregator::SubMeshAgregator(D3DDriver *pDriver)
        {

        }

        //----------------------------------------------------------------------------------------------
        SubMeshAgregator::~SubMeshAgregator()
        {

        }

        //----------------------------------------------------------------------------------------------
        SubMeshNode::SubMeshNode(D3DDriver *pDriver)
            : m_NumVertices(0)
            , m_pVertices(NULL)
            , m_pIndices(NULL)
            , m_pIndices32(NULL)
            , m_NumTriangles(0)
            , m_VertexHasNormal(false)
            , m_VertexHasColor(false)
            , m_VertexHasTexture(false)
            , m_VertexHasS(false)
            , m_VertexHasT(false)
            , m_VertexHasSxT(false)
            , m_VertexHasTangentBasis(false)
            , m_IsSkinned(false)
            , m_materialID(-1)
            , p3DDriver(pDriver)
        {
            ;//
        }

        //----------------------------------------------------------------------------------------------
        SubMeshNode::~SubMeshNode()
        {
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::DoRelease()
        {
            //Clear();

        //	p3DDriver->ReleaseSubMeshNode(this);

        //	Refcount::DoRelease();
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::Clear()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &m_Index_VBOID);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &m_Vertex_VBOID);
            glDeleteBuffers(1, &m_N_VBOID);
            glDeleteBuffers(1, &m_S_VBOID);
            glDeleteBuffers(1, &m_T_VBOID);
            glDeleteBuffers(1, &m_SxT_VBOID);

            glBindVertexArray(0);

            m_NumVertices = 0;
            m_NumTriangles = 0;

            if (m_pVertices)
            {
                delete[] m_pVertices;
                m_pVertices = NULL;
            }

            if (m_pIndices)
            {
                delete[] m_pIndices;
                m_pIndices = NULL;
            }

            if (m_pIndices32)
            {
                delete[] m_pIndices32;
                m_pIndices32 = NULL;
            }

            m_Bones.empty();
            //m_BoneMatrices.empty();
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::InitializeVertexData(const D3DDriver * D3DInterface,
            float *vertices,
            float *normals,
            float *colors,
            float *tangents,
            float *binormals,
            unsigned int	*faces_idx,
            unsigned int NumVert,
            unsigned int NumFaces,
            float *texcoords,
            unsigned int num_texcoord_sets,
            bool bSkin,
            float *weights,			// vertex weights
            unsigned int *bone_idxs,
            std::vector<int> &boneRemapper,
            unsigned int &numDistinctBones)
        {
            m_NumVertices = NumVert;
            m_NumTriangles = NumFaces;
            m_VertexHasNormal = (normals != 0);
            m_VertexHasColor = (colors != 0);
            m_VertexHasTexture = (num_texcoord_sets > 0) && (texcoords != 0);
            m_VertexHasS = (num_texcoord_sets > 0) && (tangents != 0);
            m_VertexHasT = (num_texcoord_sets > 0) && (binormals != 0);
            m_VertexHasSxT = (normals != 0);
            m_VertexHasTangentBasis = (m_VertexHasS && m_VertexHasT && m_VertexHasSxT);
            m_IsSkinned = bSkin;

            // vertex buffer
            m_pVertices = new D3DVertex[m_NumVertices];

            Ptr<SVector3f[2]> normalsVB(normals ? m_NumVertices : 0);
            Ptr<SVector3f[2]> S(m_VertexHasS ? m_NumVertices : 0);
            Ptr<SVector3f[2]> T(m_VertexHasT ? m_NumVertices : 0);
            Ptr<SVector3f[2]> SxT(m_VertexHasSxT ? m_NumVertices : 0);

            for (unsigned int Index = 0; Index < m_NumVertices; ++Index)
            {
                D3DVertex & vertex = m_pVertices[Index];

                vertex.m_Position[0] = *(vertices + Index * 3 + 0);
                vertex.m_Position[1] = *(vertices + Index * 3 + 1);
                vertex.m_Position[2] = *(vertices + Index * 3 + 2);

                if (m_VertexHasNormal)
                {
                    vertex.m_Normal[0] = vertex.m_SxT[0] = *(normals + Index * 3 + 0);
                    vertex.m_Normal[1] = vertex.m_SxT[1] = *(normals + Index * 3 + 1);
                    vertex.m_Normal[2] = vertex.m_SxT[2] = *(normals + Index * 3 + 2);

                    //@TODO: DISABLE ON RELEASE debug visualizer 
                    normalsVB[Index][0].x = vertex.m_Position[0];
                    normalsVB[Index][0].y = vertex.m_Position[1];
                    normalsVB[Index][0].z = vertex.m_Position[2];

                    normalsVB[Index][1].x = vertex.m_Position[0] + vertex.m_Normal[0];
                    normalsVB[Index][1].y = vertex.m_Position[1] + vertex.m_Normal[1];
                    normalsVB[Index][1].z = vertex.m_Position[2] + vertex.m_Normal[2];
                }

                if (colors)
                {
                    vertex.m_Color[0] = *(colors + Index * 3 + 0);
                    vertex.m_Color[1] = *(colors + Index * 3 + 1);
                    vertex.m_Color[2] = *(colors + Index * 3 + 2);
                }

                if (m_VertexHasTexture)
                {
                    vertex.m_Texture[0] = vertex.m_Texture2[0] = *(texcoords + (2 * Index));
                    vertex.m_Texture[1] = vertex.m_Texture2[1] = *(texcoords + (2 * Index + 1));
                }

                if (m_VertexHasS)
                {
                    vertex.m_S[0] = *(tangents + Index * 3 + 0);
                    vertex.m_S[1] = *(tangents + Index * 3 + 1);
                    vertex.m_S[2] = *(tangents + Index * 3 + 2);

                    S[Index][0].x = vertex.m_Position[0];
                    S[Index][0].y = vertex.m_Position[1];
                    S[Index][0].z = vertex.m_Position[2];

                    S[Index][1].x = vertex.m_Position[0] + vertex.m_S[0];
                    S[Index][1].y = vertex.m_Position[1] + vertex.m_S[1];
                    S[Index][1].z = vertex.m_Position[2] + vertex.m_S[2];
                }

                if (m_VertexHasT)
                {
                    vertex.m_T[0] = -*(binormals + Index * 3 + 0);
                    vertex.m_T[1] = -*(binormals + Index * 3 + 1);
                    vertex.m_T[2] = -*(binormals + Index * 3 + 2);

                    T[Index][0].x = vertex.m_Position[0];
                    T[Index][0].y = vertex.m_Position[1];
                    T[Index][0].z = vertex.m_Position[2];

                    T[Index][1].x = vertex.m_Position[0] + vertex.m_T[0];
                    T[Index][1].y = vertex.m_Position[1] + vertex.m_T[1];
                    T[Index][1].z = vertex.m_Position[2] + vertex.m_T[2];
                }

                if (normals)
                {
                    vertex.m_SxT[0] = *(normals + Index * 3 + 0);
                    vertex.m_SxT[1] = *(normals + Index * 3 + 1);
                    vertex.m_SxT[2] = *(normals + Index * 3 + 2);
                }

                if (m_IsSkinned)
                {

                    size_t index0 = *(bone_idxs + Index * 4 + 0); // get bone index 0
                    if (index0 == BAD_IDX)
                    {
                        index0 = 0;
                        vertex.m_Weights[0] = 0.f;
                    }
                    else
                    {
                        vertex.m_Weights[0] = *(weights + Index * 4 + 0);
                    }

                    size_t index1 = *(bone_idxs + Index * 4 + 1); // get bone index 1
                    if (index1 == BAD_IDX)
                    {
                        index1 = 0;
                        vertex.m_Weights[1] = 0.f;
                    }
                    else
                    {
                        vertex.m_Weights[1] = *(weights + Index * 4 + 1);
                    }

                    size_t index2 = *(bone_idxs + Index * 4 + 2); // get bone index 2
                    if (index2 == BAD_IDX)
                    {
                        index2 = 0;
                        vertex.m_Weights[2] = 0.f;
                    }
                    else
                    {
                        vertex.m_Weights[2] = *(weights + Index * 4 + 2);
                    }

                    size_t index3 = *(bone_idxs + Index * 4 + 3); // bone index 3
                    if (index3 == BAD_IDX)
                    {
                        index3 = 0;
                        vertex.m_Weights[3] = 0.f;
                    }
                    else
                    {
                        vertex.m_Weights[3] = *(weights + Index * 4 + 3);
                    }

                    //do remapping
                    int remappedValue = boneRemapper[index0];
                    if (remappedValue == -1)
                    {
                        AddBoneIdx(index0);	// we've never seen this bone index before
                        vertex.m_Indices[0] = (float)numDistinctBones; // save affect bone index
                        boneRemapper[index0] = numDistinctBones++; // save remmaped bone index
                    }
                    else
                    {
                        vertex.m_Indices[0] = (float)remappedValue;
                    }

                    remappedValue = boneRemapper[index1];
                    if (remappedValue == -1)
                    {
                        AddBoneIdx(index1); //we've never seen this bone index before
#ifdef D3DX_API_FVF
                        vertex.m_Indices.y = (float)numDistinctBones;
#else
#endif//D3DX_API_FVF				
                        boneRemapper[index1] = numDistinctBones++; // save remapped index 
                    }
                    else
                    {
#ifdef D3DX_API_FVF
                        vertex.m_Indices.y = (float)remappedValue;   // use previous value
#else
#endif//D3DX_API_FVF
                    }

                    remappedValue = boneRemapper[index2];
                    if (remappedValue == -1)
                    {
                        AddBoneIdx(index2); //we've never seen this bone index before

                        vertex.m_Indices[2] = (float)numDistinctBones;

                        boneRemapper[index2] = numDistinctBones++;
                    }
                    else
                    {
                        vertex.m_Indices[2] = (float)remappedValue;   // use previous value
                    }

                    remappedValue = boneRemapper[index3];
                    if (remappedValue == -1)
                    {
                        AddBoneIdx(index3); //we've never seen this bone index before

                        vertex.m_Indices[3] = (float)numDistinctBones;

                        boneRemapper[index3] = numDistinctBones++;
                    }
                    else
                    {
                        vertex.m_Indices[3] = (float)remappedValue;   //use previous value
                    }
                }
            }

            // index buffer
            unsigned int numIndices = 3 * m_NumTriangles;

            m_pIndices32 = new uint32_t[numIndices];

            for (unsigned int n = 0; n < numIndices; n += 3)
            {
                m_pIndices32[n + 0] = faces_idx[n + 1];
                m_pIndices32[n + 1] = faces_idx[n + 0];
                m_pIndices32[n + 2] = faces_idx[n + 2];
            }


            glGenBuffers(1, &m_Vertex_VBOID);		// Create the buffer ID
            assert(m_Vertex_VBOID != 0);
            glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_VBOID); // Bind the buffer (vertex array data)
            assert(glGetError() == GL_NO_ERROR);

            glBufferData(GL_ARRAY_BUFFER, sizeof(D3DVertex) * m_NumVertices, 0, GL_STATIC_DRAW);
            assert(glGetError() == GL_NO_ERROR);

            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(D3DVertex) * m_NumVertices, m_pVertices);
            assert(glGetError() == GL_NO_ERROR);

            // index
            glGenBuffers(1, &m_Index_VBOID);	// Generate buffer
            assert(glGetError() == GL_NO_ERROR);
            assert(m_Index_VBOID != 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_VBOID); // Bind the element array buffer
            assert(glGetError() == GL_NO_ERROR);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), m_pIndices32, GL_STATIC_DRAW);
            assert(glGetError() == GL_NO_ERROR);

            // debug info
            //@TODO: DISABLE ON RELEASE debug visualizer
            if (m_VertexHasNormal)	// normal
            {
                glGenBuffers(1, &m_N_VBOID);				// Create the buffer ID
                assert(m_N_VBOID != 0);
                glBindBuffer(GL_ARRAY_BUFFER, m_N_VBOID);	// Bind the buffer (vertex array data)
                glBufferData(GL_ARRAY_BUFFER, sizeof(SVector3f) * m_NumVertices * 2, 0, GL_STATIC_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SVector3f) * m_NumVertices * 2, &normalsVB[0][0]);

                glGenBuffers(1, &m_SxT_VBOID);				// Create the buffer ID
                assert(m_SxT_VBOID != 0);
                glBindBuffer(GL_ARRAY_BUFFER, m_SxT_VBOID);	// Bind the buffer (vertex array data)
                glBufferData(GL_ARRAY_BUFFER, sizeof(SVector3f) * m_NumVertices * 2, 0, GL_STATIC_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SVector3f) * m_NumVertices * 2, &normalsVB[0][0]);
            }

            if (m_VertexHasS) // S buffer
            {
                glGenBuffers(1, &m_S_VBOID);				// Create the buffer ID
                assert(m_S_VBOID != 0);
                glBindBuffer(GL_ARRAY_BUFFER, m_S_VBOID);	// Bind the buffer (vertex array data)
                glBufferData(GL_ARRAY_BUFFER, sizeof(SVector3f) * m_NumVertices * 2, 0, GL_STATIC_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SVector3f) * m_NumVertices * 2, &S[0][0]);
            }

            if (m_VertexHasT) // S buffer
            {
                glGenBuffers(1, &m_T_VBOID);				// Create the buffer ID
                assert(m_T_VBOID != 0);
                glBindBuffer(GL_ARRAY_BUFFER, m_T_VBOID);	// Bind the buffer (vertex array data)
                glBufferData(GL_ARRAY_BUFFER, sizeof(SVector3f) * m_NumVertices * 2, 0, GL_STATIC_DRAW);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SVector3f) * m_NumVertices * 2, &T[0][0]);
            }
        }

        //----------------------------------------------------------------------------------------------
        bool SubMeshNode::CreateVertexBuffers(const D3DDriver * D3DInterface, unsigned int NumVertices, unsigned int NumTriangles, bool bGenerateStrips /*= false*/)
        {
            return DoCreateVertexBuffers(D3DInterface, NumVertices, NumTriangles, bGenerateStrips);
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::SetBBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
        {
            m_AABB_Min.x = minX;
            m_AABB_Min.y = minY;
            m_AABB_Min.z = minZ;
            m_AABB_Max.x = maxX;
            m_AABB_Max.y = maxY;
            m_AABB_Max.z = maxZ;
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::GetBBox(float* minX, float* minY, float* minZ, float* maxX, float* maxY, float* maxZ) const
        {
            *minX = m_AABB_Min.x;
            *minY = m_AABB_Min.y;
            *minZ = m_AABB_Min.z;

            *maxX = m_AABB_Max.x;
            *maxY = m_AABB_Max.y;
            *maxZ = m_AABB_Max.z;
        }

        //----------------------------------------------------------------------------------------------
        bool SubMeshNode::DoCreateVertexBuffers(const D3DDriver * D3DInterface, unsigned int NumVertices, unsigned int NumTriangles, bool bGenerateStrips /*= false*/)
        {
#undef D3DX_API
#ifdef D3DX_API
            unsigned int size = NumVertices * sizeof(D3DVertex);
            if (FAILED(D3DInterface->GetDevice()->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &VertexBuffer, NULL))) {
                return false;
            }
            D3DVertex* buffer;
            if (FAILED(VertexBuffer->Lock(0, size, (void**)&buffer, 0))) {
                return false;
            }
            memcpy(buffer, Vertices, size);
            if (FAILED(VertexBuffer->Unlock())) {
                return false;
            }

            unsigned int numIndices = 3 * NumTriangles;

#ifdef USE_NV_STRIP // NVStripCode

            if (bGenerateStrips)
            {
                SetCacheSize(CACHESIZE_GEFORCE3);
                SetStitchStrips(true);
                SetMinStripSize(0);

                //First, we create the optimized list indices
                SetListsOnly(true);
                PrimitiveGroup* pPrimitiveGroupsList = NULL;
                unsigned short numSections;

                /*if (GenerateStrips(&Indices[0], numIndices, &pPrimitiveGroupsList, &numSections))
                {
                    unsigned int size = pPrimitiveGroupsList[0].numIndices * sizeof(unsigned short);

                    if (FAILED(D3DInterface->GetDevice()->CreateIndexBuffer(size, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IndexBuffer, NULL))) {
                        return false;
                    }
                    unsigned short* pDest;
                    if (FAILED(IndexBuffer->Lock(0, size, (void**)&pDest, 0))) {
                        return false;
                    }
                    memcpy(pDest, pPrimitiveGroupsList[0].indices, size);
                    if (FAILED(IndexBuffer->Unlock())){
                        return false;
                    }
                    delete [] pPrimitiveGroupsList;
                }*/
            }
            else
            {
                unsigned int isize = numIndices * sizeof(unsigned short);

                if (FAILED(D3DInterface->GetDevice()->CreateIndexBuffer(isize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IndexBuffer, NULL))) {
                    return false;
                }
                unsigned short* indexBuffer;
                if (FAILED(IndexBuffer->Lock(0, isize, (void**)&indexBuffer, 0))) {
                    return false;
                }
                memcpy(indexBuffer, &Indices[0], isize);
                if (FAILED(IndexBuffer->Unlock())) {
                    return false;
                }
            }
#else
            unsigned int isize = numIndices * sizeof(unsigned short);

            if (FAILED(D3DInterface->GetDevice()->CreateIndexBuffer(isize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &IndexBuffer, NULL))) {
                return false;
            }
            unsigned short* indexBuffer;
            if (FAILED(IndexBuffer->Lock(0, isize, (void**)&indexBuffer, 0))) {
                return false;
            }
            memcpy(indexBuffer, &Indices[0], isize);
            if (FAILED(IndexBuffer->Unlock())) {
                return false;
            }
#endif
            SetSizeBonesMat();
#endif//D3DX_API
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::SetSizeBonesMat()
        {
            if (m_IsSkinned)
            {
                //assert(m_BoneMatrices.size() == 0);
                //m_BoneMatrices.resize(m_Bones.size());
                //assert(m_BoneMatrices.size() == m_Bones.size());
            }
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::RenderMesh(const D3DDriver * D3DInterface)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_VBOID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_VBOID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 120, 0);  // pos3
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 120, (void*)12); // n
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 120, (void*)24); // c
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 120, (void*)36); // tx0
            glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 120, (void*)44); //tx1
            glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 120, (void*)52); // s
            glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 120, (void*)64); // t
            glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 120, (void*)76);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 120, (void*)88);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 120, (void*)104);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);
            glEnableVertexAttribArray(9);

            glDrawElements(GL_TRIANGLES, m_NumTriangles * 3, GL_UNSIGNED_INT, 0);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
            glDisableVertexAttribArray(5);
            glDisableVertexAttribArray(6);
            glDisableVertexAttribArray(7);
            glDisableVertexAttribArray(8);
            glDisableVertexAttribArray(9);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::RenderNormals(const D3DDriver *pD3DInterface)
        {
            float MViewProj[16] = { 0.f }, NModelViewProj[16] = { 0.f };

            Matrix4Mul(MViewProj, pD3DInterface->GetWorldMatrix(), pD3DInterface->GetViewMatrix());
            Matrix4Mul(NModelViewProj, MViewProj, pD3DInterface->GetProjMatrix());

            GLuint program = pD3DInterface->GetEffectDefault(EEF_PlainDbg)->m_ShaderProgram;

            glUseProgram(program);

            GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");

            if (modelViewProjectionMatrixLocation != -1) {
                glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE, NModelViewProj);
            }

            GLint InColor = glGetUniformLocation(program, "InColor");
            if (InColor != -1) {
                glUniform4f(InColor, 0.f, 1.f, 0.f, 1.f);
            }

            glBindBuffer(GL_ARRAY_BUFFER, m_N_VBOID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glEnableVertexAttribArray(0);

            glDrawArrays(GL_LINES, 0, m_NumVertices * 2);

            glDisableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glUseProgram(0);
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::RenderSBuffer(const D3DDriver * pD3DInterface)
        {
            if (m_VertexHasS)
            {
                float MViewProj[16] = { 0.f }, NModelViewProj[16] = { 0.f };

                Matrix4Mul(MViewProj, pD3DInterface->GetWorldMatrix(), pD3DInterface->GetViewMatrix());
                Matrix4Mul(NModelViewProj, MViewProj, pD3DInterface->GetProjMatrix());

                GLuint program = pD3DInterface->GetEffectDefault(EEF_PlainDbg)->m_ShaderProgram;

                glUseProgram(program);

                GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");

                if (modelViewProjectionMatrixLocation != -1) {
                    glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE, NModelViewProj);
                }

                GLint InColor = glGetUniformLocation(program, "InColor");
                if (InColor != -1) {
                    glUniform4f(InColor, 1.f, 1.f, 0.f, 1.f);
                }

                glBindBuffer(GL_ARRAY_BUFFER, m_S_VBOID);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glEnableVertexAttribArray(0);

                glDrawArrays(GL_LINES, 0, m_NumVertices * 2);

                glDisableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glUseProgram(0);
            }
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::RenderTBuffer(const D3DDriver * pD3DInterface)
        {
            if (m_VertexHasT)
            {
                float MViewProj[16] = { 0.f }, NModelViewProj[16] = { 0.f };

                Matrix4Mul(MViewProj, pD3DInterface->GetWorldMatrix(), pD3DInterface->GetViewMatrix());
                Matrix4Mul(NModelViewProj, MViewProj, pD3DInterface->GetProjMatrix());

                GLuint program = pD3DInterface->GetEffectDefault(EEF_PlainDbg)->m_ShaderProgram;

                glUseProgram(program);

                GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");

                if (modelViewProjectionMatrixLocation != -1) {
                    glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE, NModelViewProj);
                }

                GLint InColor = glGetUniformLocation(program, "InColor");
                if (InColor != -1) {
                    glUniform4f(InColor, 0.f, 0.f, 1.f, 1.f);
                }

                glBindBuffer(GL_ARRAY_BUFFER, m_T_VBOID);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glEnableVertexAttribArray(0);

                glDrawArrays(GL_LINES, 0, m_NumVertices * 2);

                glDisableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glUseProgram(0);

            }
        }

        //----------------------------------------------------------------------------------------------
        void SubMeshNode::RenderSxTBuffer(const D3DDriver * pD3DInterface)
        {
            float MViewProj[16] = { 0.f }, NModelViewProj[16] = { 0.f };

            Matrix4Mul(MViewProj, pD3DInterface->GetProjMatrix(), pD3DInterface->GetViewMatrix());
            Matrix4Mul(NModelViewProj, MViewProj, pD3DInterface->GetWorldMatrix());

            GLuint program = pD3DInterface->GetEffectDefault(EEF_PlainDbg)->m_ShaderProgram;

            glUseProgram(program);

            GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");

            if (modelViewProjectionMatrixLocation != -1) {
                glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE, NModelViewProj);
            }

            GLint InColor = glGetUniformLocation(program, "InColor");
            if (InColor != -1) {
                glUniform4f(InColor, 1.f, 0.f, 0.f, 1.f);
            }

            glBindBuffer(GL_ARRAY_BUFFER, m_SxT_VBOID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glEnableVertexAttribArray(0);

            glDrawArrays(GL_LINES, 0, m_NumVertices * 2);

            glDisableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glUseProgram(0);
        }
    }
}