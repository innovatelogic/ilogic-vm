
#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;

        class D3DDRIVER_API D3DAnimMesh : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(D3DAnimMesh, D3DRenderInterface);

        public:
            D3DAnimMesh(D3DDriver *Interface);
            D3DAnimMesh(const D3DAnimMesh & Source);
            virtual ~D3DAnimMesh();

        protected:
        private:
        public:
            //the vertex buffer
            /*unsigned int            m_NumVertices;
            LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;

            unsigned int            m_NumTriangles;
            LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;

            //vertex buffers used to display the normals and tangent basis
            LPDIRECT3DVERTEXBUFFER9 m_NormalBuffer;
            LPDIRECT3DVERTEXBUFFER9 m_SBuffer;
            LPDIRECT3DVERTEXBUFFER9 m_TBuffer;
            LPDIRECT3DVERTEXBUFFER9 m_SxTBuffer;*/

            //boolean values that describe the information available for every vertex of every mesh
            bool m_VertexHasNormal;
            bool m_VertexHasColor;
            bool m_VertexHasTexture;
            bool m_VertexHasS;
            bool m_VertexHasT;
            bool m_VertexHasSxT;
            bool m_VertexHasTangentBasis;

            bool bShowNormals;
            bool bShowTangent;
            bool bShowBinormal;
        };
    }
}