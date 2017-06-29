#include "d3ddriverstdafx.h"
#include <math.h>

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        D3DMesh::D3DMesh(const D3DDriver *pInterface)
            : Super(pInterface)
            , m_pSceneMesh(NULL)
            , m_bShowDebug(false)
            , bShowNormals(false)
            , bShowTangent(false)
            , bShowBinormal(false)
            , bCastShadow(true)
            , bReciveShadow(true)
            , m_nForceLODLvl(0)
            , m_nDestructLvl(0)
        {
        }

        //----------------------------------------------------------------------------------------------
        D3DMesh::D3DMesh(const D3DMesh &Sender)
            : Super(Sender)
            , m_pSceneMesh(NULL)
            , m_bShowDebug(false)
            , bShowNormals(false)
            , bShowTangent(false)
            , bShowBinormal(false)
            , bCastShadow(true)
            , bReciveShadow(true)
            , m_nForceLODLvl(-1)
            , m_nDestructLvl(-1)
        {
            if (this != &Sender)
            {
            }
        }

        //----------------------------------------------------------------------------------------------
        D3DMesh::~D3DMesh()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::Release()
        {
            Clear();
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::Clear(bool SkipEffects /*= false*/)
        {
            if (m_pSceneMesh)
            {
                m_pD3DInterface->UnregisterSceneMeshNode(m_pSceneMesh);
                m_pSceneMesh = nullptr;
            }

            for (std::vector<SMeshEffectBank>::iterator Iter = m_VMeshEffectBank.begin(); Iter != m_VMeshEffectBank.end(); ++Iter)
            {
                for (std::vector<SVecSubMeshes>::iterator Iter2 = (*Iter).VBankNodes.begin(); Iter2 != (*Iter).VBankNodes.end(); ++Iter2)
                {
                    for (TVecSubMeshes::iterator Iter3 = (*Iter2).VSubMeshes.begin(); Iter3 != (*Iter2).VSubMeshes.end(); ++Iter3)
                    {
                        SMeshEffectPair &pair = *Iter3;

                        if (pair.Effect && !SkipEffects) {
                            pair.Effect->Release();
                        }
                    }
                }
            }

            m_VMeshEffectBank.clear();
        }

        //----------------------------------------------------------------------------------------------
        bool D3DMesh::LoadFromDesc(const SMeshMaterialSet &Desc)
        {
            return false;
        }

        //----------------------------------------------------------------------------------------------
        BaseMaterial* D3DMesh::ApplyMeshMaterial(int materialID, const SMeshMaterialSet &MaterialSet)
        {
            return 0;
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::AddMesh(SMeshEffectPair &Pair, size_t LOD /*= 0*/, size_t Dstr /*= 0*/)
        {
            while (m_VMeshEffectBank.size() <= Dstr)
            {
                m_VMeshEffectBank.push_back(SMeshEffectBank());
                assert(m_VMeshEffectBank.size() <= 5);
            }

            while (m_VMeshEffectBank[Dstr].VBankNodes.size() <= LOD)
            {
                m_VMeshEffectBank[Dstr].VBankNodes.push_back(SVecSubMeshes());
                assert(m_VMeshEffectBank[Dstr].VBankNodes[LOD].VSubMeshes.size() <= 5);
            }

            m_VMeshEffectBank[Dstr].VBankNodes[LOD].VSubMeshes.push_back(Pair);
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::Render()
        {
            DoRender();
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::SetBoneTransform(unsigned int index, float * pMatrix)
        {
            /*	for (unsigned int Index = 0; Index < m_VecSubMeshes.size(); ++Index)
                {
                    SMeshEffectPair &pair = m_VecSubMeshes[Index];

                    pair.SubMesh->m_BoneMatrices[index]._11 = *(pMatrix + 0);
                    pair.SubMesh->m_BoneMatrices[index]._12 = *(pMatrix + 1);
                    pair.SubMesh->m_BoneMatrices[index]._13 = *(pMatrix + 2);
                    pair.SubMesh->m_BoneMatrices[index]._14 = *(pMatrix + 3);

                    pair.SubMesh->m_BoneMatrices[index]._21 = *(pMatrix + 4);
                    pair.SubMesh->m_BoneMatrices[index]._22 = *(pMatrix + 5);
                    pair.SubMesh->m_BoneMatrices[index]._23 = *(pMatrix + 6);
                    pair.SubMesh->m_BoneMatrices[index]._24 = *(pMatrix + 7);

                    pair.SubMesh->m_BoneMatrices[index]._31 = *(pMatrix + 8);
                    pair.SubMesh->m_BoneMatrices[index]._32 = *(pMatrix + 9);
                    pair.SubMesh->m_BoneMatrices[index]._33 = *(pMatrix + 10);
                    pair.SubMesh->m_BoneMatrices[index]._34 = *(pMatrix + 11);

                    pair.SubMesh->m_BoneMatrices[index]._41 = *(pMatrix + 12);
                    pair.SubMesh->m_BoneMatrices[index]._42 = *(pMatrix + 13);
                    pair.SubMesh->m_BoneMatrices[index]._43 = *(pMatrix + 14);
                    pair.SubMesh->m_BoneMatrices[index]._44 = *(pMatrix + 15);
                }*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DMesh::DoRender()
        {
            if ((m_pD3DInterface->GetActiveRenderTarget() ||
                m_pD3DInterface->IsRenderShadowmapPass()) && !GetCastShadows()) {
                return;
            }

            if (m_VMeshEffectBank.size() == 0 ||
                m_VMeshEffectBank[0].VBankNodes.size() == 0) {
                return;
            }


            unsigned int nLOD = 0;

            if (m_pSceneMesh)
            {
                // LOD benefit function
                float bboxMinMax[6];
                m_pSceneMesh->GetSceneBBox(&bboxMinMax[0], &bboxMinMax[1], &bboxMinMax[2],
                    &bboxMinMax[3], &bboxMinMax[4], &bboxMinMax[5]);

                nLOD = m_pD3DInterface->GetLodBenefitByBBox(&bboxMinMax[0], &bboxMinMax[1], &bboxMinMax[2],
                    &bboxMinMax[3], &bboxMinMax[4], &bboxMinMax[5]);
            }

            unsigned int nDstrLvl = *m_pD3DInterface->GetObjectFlags();
            unsigned int nBank = nDstrLvl >= 0 && nDstrLvl < m_VMeshEffectBank.size() ? nDstrLvl : m_VMeshEffectBank.size() - 1;

            if (nLOD > m_VMeshEffectBank[nBank].VBankNodes.size() - 1) {
                nLOD = m_VMeshEffectBank[nBank].VBankNodes.size() - 1;
            }

            if (TVecSubMeshes *pSubMeshes = &m_VMeshEffectBank[nBank].VBankNodes[nLOD].VSubMeshes)
            {
                for (unsigned int Index = 0; Index < pSubMeshes->size(); ++Index)
                {
                    SMeshEffectPair &pair = (*pSubMeshes)[Index];

                    if (pair.Effect && pair.SubMesh)
                    {
                        pair.Effect->BeginEffect(0);

                        pair.Effect->Affect();

                        pair.SubMesh->RenderMesh(NULL);

                        pair.Effect->EndEffect();

                        if (m_bShowDebug) {
                        }

                        // debug render info
                        if (bShowNormals && pair.SubMesh) {
                            pair.SubMesh->RenderNormals(GetInterface());
                        }
                        if (bShowBinormal && pair.SubMesh) {
                            pair.SubMesh->RenderSBuffer(GetInterface());
                        }
                        if (bShowTangent && pair.SubMesh) {
                            pair.SubMesh->RenderTBuffer(GetInterface());
                        }
                    }
                }
            }
        }
    }
}