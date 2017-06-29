#include "d3ddriverstdafx.h"

#undef D3DX_API

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        MaterialEffect::MaterialEffect(const D3DDriver * Interface)
            : BaseMaterial(Interface)
            , DiffuseMap(NULL)
            , HeightMap(NULL)
            , NormalMap(NULL)
        {
            DiffuseMap = new Texture2D(Interface);
            HeightMap = new Texture2D(Interface);
            NormalMap = new Texture2D(Interface);
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffect::~MaterialEffect()
        {
            Release();

            DiffuseMap->Release();
            delete DiffuseMap;

            HeightMap->Release();
            delete HeightMap;

            NormalMap->Release();
            delete NormalMap;
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect::Release()
        {
            BaseMaterial::Release();
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffect::Affect() /*const*/
        {
#ifndef D3DX_API

            GLuint program = GetNode()->GetShaderProgram();

            float WVMat[16] = { 0.f },
                WVPMat[16] = { 0.f };

            Matrix4Mul(WVMat, m_pD3DInterface->m_world, m_pD3DInterface->m_view);
            Matrix4Mul(WVPMat, WVMat, m_pD3DInterface->m_proj);

            GLint modelViewProjectionMatrixLocation = glGetUniformLocation(program, "mVPMat");
            if (modelViewProjectionMatrixLocation != -1) {
                glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, GL_TRUE, WVPMat);
            }

            GLint modelViewMatrixLocation = glGetUniformLocation(program, "mVMat");
            if (modelViewMatrixLocation != -1) {
                glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_TRUE, WVMat);
            }

            SetSampleTextureID(0, DiffuseMap);

            bool bFog = m_pD3DInterface->m_bFog && fog;

            GLint InFog = glGetUniformLocation(program, "bFog");
            if (InFog != -1) {
                glUniform1i(InFog, bFog);
            }

            GLuint InFogColor = glGetUniformLocation(program, "fogColor");
            if (InFogColor != -1)
            {
                byte a = (m_pD3DInterface->m_FogColor & 0xFF000000) >> 24;
                byte r = (m_pD3DInterface->m_FogColor & 0x00FF0000) >> 16;
                byte g = (m_pD3DInterface->m_FogColor & 0x0000FF00) >> 8;
                byte b = (m_pD3DInterface->m_FogColor & 0x000000FF);

                glUniform4f(InFogColor, r / 255.f, g / 255.f, b / 255.f, a / 255.f);
            }

            GLint InFogDensity = glGetUniformLocation(program, "fogDensity");
            if (InFogDensity != -1) {
                glUniform1f(InFogDensity, m_pD3DInterface->m_fFogDensity);
            }

#else
            D3DXMATRIX WorldView = m_pD3DInterface->m_world * m_pD3DInterface->m_view;
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

            if (m_pD3DInterface->IsRenderShadowmapPass() == false)
            {
                SetSampleTexture("DiffuseMap", DiffuseMap);
                SetSampleTexture("NormalMap", NormalMap);
                //SetSampleTextureDirect("ShadowMap", m_pD3DInterface->pTextureShadowMap);

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
            }
#endif//D3DX_API
        }

        //----------------------------------------------------------------------------------------------
        /*const*/ Texture2D* MaterialEffect::GetDiffuseTexture() const
        {
            return DiffuseMap;
        }

        //----------------------------------------------------------------------------------------------
        const Texture2D* MaterialEffect::GetNormalTexture() const
        {
            return NormalMap;
        }

        //----------------------------------------------------------------------------------------------
        const Texture2D* MaterialEffect::GetHeightTexure() const
        {
            return HeightMap;
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect::LoadDiffuseMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(DiffuseMap);
            return DiffuseMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect::LoadNormalMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(NormalMap);
            return NormalMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect::LoadHeightMap(const char *filename, bool ImmidiateLoad /*= true*/)
        {
            assert(HeightMap);
            return HeightMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        float MaterialEffect::GetDiffuseU() const
        {
            return DiffuseMap->GetU();
        }

        //----------------------------------------------------------------------------------------------
        float MaterialEffect::GetDiffuseV() const
        {
            return DiffuseMap->GetV();
        }

        //----------------------------------------------------------------------------------------------
        float MaterialEffect::GetDiffuseUL() const
        {
            return DiffuseMap->GetUL();
        }

        //----------------------------------------------------------------------------------------------
        float MaterialEffect::GetDiffuseVL() const
        {
            return DiffuseMap->GetVL();
        }
    }
}