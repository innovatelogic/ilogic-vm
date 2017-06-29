
#pragma once

#ifdef _WIN32
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class D3DDriver;
        class BaseMaterial;

        //----------------------------------------------------------------------------------------------
        class D3DDRIVER_API CD3DFontMesh : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(CD3DFontMesh, D3DRenderInterface);

        public:
            CD3DFontMesh(const D3DDriver *pDrv);
            CD3DFontMesh(const D3DMesh &Sender);
            virtual ~CD3DFontMesh();

            virtual void	Release();
            virtual void	Clear();

            void InitializeVertexData(float *vertexes,
                float *texcoords,
                unsigned int *faces_idx,
                unsigned int NumVert,
                unsigned int NumFaces,
                BaseMaterial	*pEffect);

            void	Render();

        private:
            virtual void	DoRender();

        private:
            GLuint			m_Vertex_VBOID;			// Vertex Buffer Object ID
            GLuint			m_Index_VBOID;			// Index Buffer Object ID

            BaseMaterial	*m_pEffect;

            unsigned int	m_NumVertices;
        };
    }
}