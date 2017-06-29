#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        MaterialEffectUI::MaterialEffectUI(const D3DDriver * Interface)
            : BaseMaterial(Interface)
            , m_pRT(0)
        {
            m_pDiffuseMap = new Texture2D(Interface);
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffectUI::~MaterialEffectUI()
        {
            Release();

            m_pDiffuseMap->Release();
            delete m_pDiffuseMap;
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectUI::Release()
        {
            BaseMaterial::Release();
            return true;
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffectUI::Affect()
        {
            GLuint program = GetNode()->GetShaderProgram();

            float WVMat[16] = { 0.f },
                WVPMat[16] = { 0.f },
                UVMat[16] = { m_pDiffuseMap->GetUL(), 0.f, 0.f, 0.f,
                              0.f, m_pDiffuseMap->GetVL(), 0.f, 0.f,
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
                glUniform2f(modelUVOffsetLocation, m_pDiffuseMap->GetU(), m_pDiffuseMap->GetV());
            }

            if (m_pRT == nullptr)
            {
                SetSampleTextureID(0, m_pDiffuseMap);
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + 0);
                glBindTexture(GL_TEXTURE_2D, m_pRT->GetTexture());
            }

            bool bFog = m_pD3DInterface->m_bFog && fog;

            GLint InFog = glGetUniformLocation(program, "bFog");
            if (InFog != -1) {
                glUniform1i(InFog, bFog);
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

            glAlphaFunc(GL_GREATER, 0.1f);	// Set Alpha Testing     (disable blending)
            glEnable(GL_ALPHA_TEST);		// Enable Alpha Testing  (disable blending)
        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffectUI::EndEffect()
        {
            glDisable(GL_ALPHA_TEST);

            glUseProgram(0);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectUI::LoadDiffuseMap(const char* filename, bool ImmidiateLoad /*= true*/)
        {
            return m_pDiffuseMap->Load(filename, ImmidiateLoad);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectUI::LoadDiffuseMapW(const wchar_t *filename)
        {
            return m_pDiffuseMap->LoadW(filename);
        }

        //----------------------------------------------------------------------------------------------
        Texture2D* MaterialEffectUI::GetDiffuseTexture() const
        {
            return m_pDiffuseMap;
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectUI::GetInfo(unsigned int &Width, unsigned int &Height) const
        {
            return m_pDiffuseMap->GetInfo(Width, Height);
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectUI::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight/* = 0*/) const
        {
            return m_pDiffuseMap->GetDIB(pData, bResample, NewWidth, NewHeight);
        }
    }
}