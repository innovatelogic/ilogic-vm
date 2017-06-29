#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        MaterialEffect_ButtonStates::MaterialEffect_ButtonStates(const D3DDriver * Interface)
            : BaseMaterial(Interface)
            , StateIndex(0)
        {
            for (size_t Index = 0; Index < 5; ++Index)
            {
                DiffuseMaps.push_back(new Texture2D(Interface));
            }
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffect_ButtonStates::~MaterialEffect_ButtonStates()
        {
            for (size_t Index = 0; Index < DiffuseMaps.size(); ++Index)
            {
                DiffuseMaps[Index]->Release();
                delete DiffuseMaps[Index];
            }
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect_ButtonStates::Release()
        {
            BaseMaterial::Release();
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffect_ButtonStates::Affect()
        {
            GLint program = GetNode()->GetShaderProgram();

            float WVMat[16] = { 0.f },
                WVPMat[16] = { 0.f },
                UVMat[16] = { DiffuseMaps[StateIndex]->GetUL(), 0.f, 0.f, 0.f,
                              0.f, DiffuseMaps[StateIndex]->GetVL(), 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f };

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

            GLint modelUVMatrixLocation = glGetUniformLocation(program, "mUVMat");
            if (modelUVMatrixLocation != -1) {
                glUniformMatrix4fv(modelUVMatrixLocation, 1, GL_TRUE, UVMat);
            }

            GLint modelUVOffsetLocation = glGetUniformLocation(program, "UVOffset");
            if (modelUVOffsetLocation != -1) {
                glUniform2f(modelUVOffsetLocation, DiffuseMaps[StateIndex]->GetU(), DiffuseMaps[StateIndex]->GetV());
            }

            SetSampleTextureID(0, DiffuseMaps[StateIndex]);

            bool bFog = m_pD3DInterface->m_bFog && fog;

            GLint InFog = glGetUniformLocation(program, "bFog");
            if (InFog != -1) {
                glUniform1i(InFog, false);
            }

            GLint InFogColor = glGetUniformLocation(program, "fogColor");
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
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffect_ButtonStates::LoadDiffuseMap(const char *filename, unsigned int StateIdx)
        {
            bool bResult = false;

            if (StateIdx < DiffuseMaps.size())
            {
                bResult = DiffuseMaps[StateIdx]->Load(filename);
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        /*const*/ Texture2D* MaterialEffect_ButtonStates::GetDiffuseTexture(unsigned int StateIdx) const
        {
            if (StateIdx < DiffuseMaps.size())
            {
                return DiffuseMaps[StateIdx];
            }
            return NULL;
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffect_ButtonStates::SetActiveIdx(unsigned int Idx)
        {
            StateIndex = Idx;
        }
    }
}