
#pragma once

#ifdef WIN32
#pragma warning (disable:4251)
#endif

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        class D3DDRIVER_API MaterialEffectNode : public Refcount
        {

        public:
            MaterialEffectNode(const D3DDriver *pDriver);
            virtual ~MaterialEffectNode();

            virtual bool CompileEffect(const char *strPath);
            virtual bool CompileEffectW(const wchar_t *strPath);

            __forceinline GLuint GetShaderProgram() const { return m_ShaderProgram; }

            // Parameters check
            __forceinline bool IsParam_bool(const char* value) const
            {
                return std::find(Params_bool.begin(), Params_bool.end(), value) != Params_bool.end();
            }

            __forceinline bool IsParam_float4x4(const char *value) const
            {
                return std::find(Params_float4x4.begin(), Params_float4x4.end(), value) != Params_float4x4.end();
            }

            __forceinline bool IsParam_float4x3(const char *value) const
            {
                return std::find(Params_float4x3.begin(), Params_float4x3.end(), value) != Params_float4x3.end();
            }

            __forceinline bool IsParam_float3x3(const char *value) const
            {
                return std::find(Params_float3x3.begin(), Params_float3x3.end(), value) != Params_float3x3.end();
            }

            __forceinline bool IsParam_float4(const char *value) const
            {
                return std::find(Params_float4.begin(), Params_float4.end(), value) != Params_float4.end();
            }

            __forceinline bool IsParam_float3(const char *value) const
            {
                return std::find(Params_float3.begin(), Params_float3.end(), value) != Params_float3.end();
            }

            __forceinline bool IsParam_float(const char *value) const
            {
                return std::find(Params_float.begin(), Params_float.end(), value) != Params_float.end();
            }

            __forceinline bool IsParam_Texture(const char *value) const
            {
                return std::find(Params_texture.begin(), Params_texture.end(), value) != Params_texture.end();
            }

            SRenderContext* GetOwnerContext() const { return m_pRenderContext; }

        protected:
            virtual void DoRelease();

        private:
            std::vector<std::string>	Params_bool;
            std::vector<std::string>	Params_float4x4;
            std::vector<std::string>	Params_float4x3;
            std::vector<std::string>	Params_float3x3;
            std::vector<std::string>	Params_float4;
            std::vector<std::string>	Params_float3;
            std::vector<std::string>	Params_float;
            std::vector<std::string>	Params_texture;

        public:
            GLuint					m_ShaderProgram;

            SRenderContext			*m_pRenderContext;

            D3DDriver			*m_p3DDriver;
        };
    }
}
