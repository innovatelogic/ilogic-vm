
#pragma once

#ifdef WIN32
#pragma warning (disable:4251)
#endif

#include "Refcount.h"

namespace oes
{
    namespace d3d
    {
        struct vertex_node
        {
            vertex_node() :x(0.0f), y(0.0f), z(0.0f) {}
            float x, y, z;
        };

        struct uv_node
        {
            uv_node() :u(0.0f), v(0.0f) {}
            float u, v;
        };

        class D3DDRIVER_API CFontNode : public Refcount
        {
        public:
            struct char_info_t
            {
                int x;
                int y;
                int width;
                int height;
                int left;
                int top;
                int advance;
                int row;

                uv_node     uv[4];
                vertex_node v[4];
                uint8_t* bitmap;
            };

            struct font_info_t
            {
                int max_height;
                std::vector<char_info_t> ch;
            };

            CFontNode(D3DDriver *p3DDriver, const char* faceName, float height = 10.f);
            virtual ~CFontNode();

            SRenderContext* GetOwnerContext() const { return m_pRenderContext; }

        protected:
            virtual void DoRelease();

        public:
            std::string m_FaceName;
            font_info_t m_Info;

            float		m_fHeight;
            GLuint		*m_textures;
            GLuint		m_BitmapTexID;			// Texture ID

            SRenderContext	*m_pRenderContext;
            D3DDriver		*m_p3DDriver;
        };
    }
}