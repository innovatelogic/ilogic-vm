
#pragma once

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class Texture2D;
        class Texture3D;

        class D3DDRIVER_API BaseMaterial : public D3DRenderInterface
        {
        public:
            BaseMaterial(const BaseMaterial &other) = delete;
            //BaseMaterial operator= (const BaseMaterial &other) = delete;

            BaseMaterial(const D3DDriver *pInterface);
            virtual ~BaseMaterial();

            virtual bool				Load(const char *URL);
            virtual bool				LoadW(const wchar_t *URL);

            virtual bool				Release();
            virtual void				Affect() = 0;

            virtual unsigned int		BeginEffect(const char *Tehnique) const;
            virtual void				EndEffect();

            virtual void				BeginPass(unsigned int Pass) const;
            virtual void				EndPass() const;

            void						SetBoolParam(const char *ParamName, bool Value) const;
            void						SetMatrixParam(const char *ParamName, const float* Value, int index = -1) const;
            void						SetVector3Param(const char *ParamName, const float* Value) const;
            void						SetVector4Param(const char *ParamName, const float* Value) const;
            void						SetSampleTexture(const char *ParamName, const Texture2D * Value) const;
            void						SetSampleTextureID(short Index, const Texture2D * Value) const;
            void						SetSampleTexture(const char *ParamName, const Texture3D * Value) const;
            //	void						SetSampleTextureDirect(const char *ParamName, LPDIRECT3DTEXTURE9 Value) const;
            void						SetScalarValue(const char *ParamName, float Value) const;

            const char*					GetTechniqueName() const { return m_TechniqueName.c_str(); }
            void						SetTechniqueName(const char *name) { m_TechniqueName = name; }

            void						SetReference(const char *Ref) { m_Reference = Ref; }
            const char*					GetReference() const { return m_Reference.c_str(); }

            MaterialEffectNode*	        GetNode() const { return m_pEffectNode; }

            void						SetAlphaTestEnable(bool flag) { m_bAlphaTest = flag; }
            bool						GetAlphaTestEnable() { return m_bAlphaTest; }

            void						SetTransparent(float value) { transperent = value; }
            float						GetTransparent() { return transperent; }

        public:
            unsigned int				diffuse;	// diffuse color
            unsigned int				specular;	// specular color
            float						shininess;
            unsigned int				ambient;
            unsigned int				emmision;
            float						transperent;
            bool						fog;
            unsigned int				fog_color;

            // opengl states
            bool						m_bAlphaTest;

        protected:
            std::string					m_Reference;
            std::string					m_TechniqueName;

            MaterialEffectNode	        *m_pEffectNode;
        };
    }
}