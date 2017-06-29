#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        MaterialEffectLava::MaterialEffectLava(const D3DDriver * Interface)
            : BaseMaterial(Interface)
            , noiseScale(0.21f)
            , morphRate(0.05f)
            , bubbleRate(0.12f)
            , bubbleScale(0.95f)
            , scale(0.82f)
            , bias(-0.12f)
        {
            DiffuseMap = new Texture2D(Interface);
            HeightMap = new Texture2D(Interface);
            NormalMap = new Texture2D(Interface);
            NoiseMap = new Texture3D(Interface);
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffectLava::~MaterialEffectLava()
        {
            Release();

            DiffuseMap->Release();
            delete DiffuseMap;

            HeightMap->Release();
            delete HeightMap;

            NormalMap->Release();
            delete NormalMap;

            NoiseMap->Release();
            delete NoiseMap;
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffectLava::Affect()
        {
            /*	D3DXMATRIX WorldView = m_pD3DInterface->m_world * m_pD3DInterface->m_view;
                D3DXMATRIX WorldViewProjection = WorldView * m_pD3DInterface->m_proj;

                if (m_pD3DInterface->IsRenderShadowmapPass())
                {
                    WorldViewProjection *= m_pD3DInterface->m_crop;
                }

                D3DXMATRIX InvView;
                D3DXMatrixInverse(&InvView, NULL, &m_pD3DInterface->m_view);

                D3DXVECTOR4 TLV;
                D3DXVECTOR3 LV = m_pD3DInterface->DiffusePoint - D3DXVECTOR3(m_pD3DInterface->m_world._41, m_pD3DInterface->m_world._42, m_pD3DInterface->m_world._43);
                D3DXVec3Transform(&TLV, &LV, &m_pD3DInterface->m_world);
                D3DXVec4Normalize(&TLV, &TLV);

                //	var argb  : int = (alpha<<24)|rgb;
                //	var rgb   : int = 0xFFFFFF & argb;
                //	var alpha : int = (argb>>24)&0xFF;
                D3DXVECTOR3 AmbientColor = D3DXVECTOR3(GetBValue(m_pD3DInterface->LightColor) / 255.f,
                    GetGValue(m_pD3DInterface->LightColor) / 255.f,
                    GetRValue(m_pD3DInterface->LightColor) / 255.f);

                SetMatrixParam("worldView", (float*)WorldView);
                SetMatrixParam("worldViewProj", (float*)WorldViewProjection);
                SetMatrixParam("matWorld", (float*)m_pD3DInterface->m_world);

                SetVector3Param("ViewPosition", (float*)D3DXVECTOR3(InvView._41, InvView._42, InvView._43));

                SetVector3Param("LightVector", (float*)LV);
                SetVector3Param("LightPosition", (float*)m_pD3DInterface->DiffusePoint);
                SetVector3Param("LightColor", (float*)AmbientColor);

                SetScalarValue("opacity", m_pD3DInterface->BlendOpacity);
                SetScalarValue("Ambient", 1.0f);
                SetScalarValue("time_0_X", m_pD3DInterface->fAppPassTime);

                if (m_pD3DInterface->IsRenderShadowmapPass() == false)
                {
                    SetSampleTexture("Flame_Tex", DiffuseMap);
                    SetSampleTexture("Noise_Tex",  NoiseMap);
                    SetSampleTextureDirect("ShadowMap", m_pD3DInterface->pTextureShadowMap);

                    D3DXMATRIXA16 mViewToLightProj;
                    D3DXMatrixMultiply(&mViewToLightProj, &m_pD3DInterface->MLightView, &m_pD3DInterface->MLightProj);
                    SetMatrixParam("ViewToLightProj", (float*)mViewToLightProj);

                    D3DXVECTOR4 v4;
                    D3DXVec3Transform(&v4, &m_pD3DInterface->DiffusePoint, &m_pD3DInterface->m_view);
                    SetVector3Param("g_vLightPos", (float*)v4);

                    D3DXVec3Transform(&v4, &m_pD3DInterface->LightDirection, &m_pD3DInterface->m_view);
                    D3DXVec3Normalize((D3DXVECTOR3*)&v4, (D3DXVECTOR3*)&v4);
                    SetVector3Param("g_vLightDir", (float*)m_pD3DInterface->LightDirection);

                    SetVector4Param("g_vSplitDistances", (float*)m_pD3DInterface->FrustrumSplitDistances);

                    for (unsigned int Index = 0; Index < 3; ++Index)
                    {
                        D3DXMATRIXA16 ViewToLightProjCrop = m_pD3DInterface->m_world;
                        D3DXMatrixMultiply(&ViewToLightProjCrop, &ViewToLightProjCrop, &m_pD3DInterface->MLightView);
                        D3DXMatrixMultiply(&ViewToLightProjCrop, &ViewToLightProjCrop, &m_pD3DInterface->MLightProj);
                        D3DXMatrixMultiply(&ViewToLightProjCrop, &ViewToLightProjCrop, &m_pD3DInterface->MLightProjCrop[Index]);

                        SetMatrixParam("AViewToLightProj", (float*)ViewToLightProjCrop, Index);
                    }
                }*/
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectLava::LoadDiffuseMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(DiffuseMap);
            return DiffuseMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectLava::LoadNormalMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(NormalMap);
            return NormalMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectLava::LoadHeightMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(HeightMap);
            return HeightMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectLava::LoadNoiseMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(NoiseMap);
            return NoiseMap->Load(filename, 0, 0, 0, ImmidiateLoad);
        }
    }
}