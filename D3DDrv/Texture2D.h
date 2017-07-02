
#pragma once

#ifdef _WIN32
#pragma warning(disable: 4251)
#endif

#include "D3DRenderInterface.h"

namespace oes
{
    namespace d3d
    {
        class TextureNode;

        class D3DDRIVER_API Texture2D : public D3DRenderInterface
        {
            DECL_CLASS_SIMPLE(Texture2D, D3DRenderInterface);

            // defines the different types of textures
            typedef enum TextureType
            {
                CUSTOM = 0x00000000,
                AMBIENT = 0x00000001,
                DIFFUSE = 0x00000002,
                SPECULAR = 0x00000003, // specular color map
                SPECULAR_POWER = 0x00000004, // specular highlight map
                GLOSS = 0x00000005,
                SELF_ILLUMATION = 0x00000006,
                BUMP = 0x00000007,
                NORMAL = 0x00000008,
                OPACITY = 0x00000009,
                REFLECTION = 0x0000000A,
                REFRACTION = 0x0000000B,
                CUBE_MAP = 0x0000000C
            } TexType;

        public:
            Texture2D(const D3DDriver *itf);
            virtual ~Texture2D();

            virtual bool		Load(const char *filepath, bool immidiateLoad = true);
            virtual bool		LoadW(const wchar_t *filepath);

            virtual void		Release();

            virtual void		SetReference(const std::string &ref) { m_Reference = ref; }
            virtual std::string	GetReference() const { return m_Reference; }

            virtual void		 SetColor(unsigned int color) { m_Color = color; }
            virtual unsigned int GetColor() { return m_Color; }

            GLuint				GetTextureGL(bool noNull = true) const;

            void SetTexcoords(float u, float v, float ul, float vl);

            float GetU() { return m_Texcoord.x; }
            float GetV() { return m_Texcoord.y; }
            float GetUL() { return m_Texcoord.z; }
            float GetVL() { return m_Texcoord.w; }

            bool GetInfo(unsigned int &width, unsigned int &height) const;
            bool GetDIB(BYTE** pdata, bool resample = false, unsigned int new_width = 0, unsigned int new_height = 0) const;

        private:

            unsigned int		m_Color;
            std::string         m_Reference;
            SVector4f			m_Texcoord;

            TextureNode         *m_node;
        };
    }
}