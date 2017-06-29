#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //-----------------------------------------------------------------------
        D3DAnimMesh::D3DAnimMesh(D3DDriver * Interface)
            : Super(Interface)
            /*, m_NumVertices(0)
            , m_VertexBuffer(NULL)
            , m_NumTriangles(0)
            , m_IndexBuffer(NULL)
            , m_NormalBuffer(NULL)
            , m_SBuffer(NULL)
            , m_TBuffer(NULL)
            , m_SxTBuffer(NULL)*/
            , m_VertexHasNormal(false)
            , m_VertexHasColor(false)
            , m_VertexHasTexture(false)
            , m_VertexHasS(false)
            , m_VertexHasT(false)
            , m_VertexHasSxT(false)
            , m_VertexHasTangentBasis(false)
            , bShowNormals(false)
            , bShowTangent(false)
            , bShowBinormal(false)
        {

        }

        //-----------------------------------------------------------------------
        D3DAnimMesh::D3DAnimMesh(const D3DAnimMesh & Source)
            : Super(Source)
        {

        }

        //-----------------------------------------------------------------------
        D3DAnimMesh::~D3DAnimMesh()
        {

        }
    }
}