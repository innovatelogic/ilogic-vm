#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        CD3DFontMesh::CD3DFontMesh(const D3DDriver *pDrv)
            : D3DRenderInterface(pDrv)
        {

        }

        //----------------------------------------------------------------------------------------------
        CD3DFontMesh::CD3DFontMesh(const D3DMesh &Sender)
            : D3DRenderInterface(Sender)
        {

        }

        //----------------------------------------------------------------------------------------------
        CD3DFontMesh::~CD3DFontMesh()
        {

        }

        //----------------------------------------------------------------------------------------------
        void CD3DFontMesh::Release()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &m_Index_VBOID);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &m_Vertex_VBOID);
        }

        //----------------------------------------------------------------------------------------------
        void CD3DFontMesh::Clear()
        {

        }

        //----------------------------------------------------------------------------------------------
        void CD3DFontMesh::InitializeVertexData(float *vertices,
            float *texcoords,
            unsigned int *faces_idx,
            unsigned int NumVert,
            unsigned int NumIndices,
            BaseMaterial *pEffect)
        {
            m_NumVertices = NumVert;

            SD3DVertexFont *pVertices = new SD3DVertexFont[NumVert];

            for (unsigned int Index = 0; Index < NumVert; ++Index)
            {
                SD3DVertexFont& vertex = pVertices[Index];

                vertex.m_Position[0] = *(vertices + Index * 3 + 0);
                vertex.m_Position[1] = *(vertices + Index * 3 + 1);
                vertex.m_Position[2] = *(vertices + Index * 3 + 2);

                vertex.m_Texture[0] = *(texcoords + (2 * Index));
                vertex.m_Texture[1] = *(texcoords + (2 * Index + 1));
            }

            // vertex
            glGenBuffers(1, &m_Vertex_VBOID);				// Create the buffer ID
            glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_VBOID);	// Bind the buffer (vertex array data)
            glBufferData(GL_ARRAY_BUFFER, sizeof(SD3DVertexFont) * NumVert, 0, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SD3DVertexFont) * NumVert, pVertices);

            // index
            glGenBuffers(1, &m_Index_VBOID);	// Generate buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_VBOID); // Bind the element array buffer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices * sizeof(uint32_t), faces_idx, GL_STATIC_DRAW);

            delete[] pVertices;
        }

        //----------------------------------------------------------------------------------------------
        void CD3DFontMesh::Render()
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindBuffer(GL_ARRAY_BUFFER, m_Vertex_VBOID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Index_VBOID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, 0);  // pos3
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12); // tx0

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glDrawElements(GL_TRIANGLES, m_NumVertices * 3, GL_UNSIGNED_INT, 0);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            glBlendFunc(GL_ONE, GL_ZERO);
            glDisable(GL_BLEND);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        //----------------------------------------------------------------------------------------------
        void CD3DFontMesh::DoRender()
        {
        }
    }
}