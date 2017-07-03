#include "Texture2D.h"
#include "D3DDriver.h"
#include "TextureNode.h"
#include <string>

namespace oes
{
    namespace d3d
    {
        //-----------------------------------------------------------------------
        Texture2D::Texture2D(const D3DDriver * Interface)
            : Super(Interface)
            , m_node(nullptr)
            , m_Reference("")
            , m_Color(0xffffffff)
            //, m_Texcoord(0.f, 0.f, 1.f, 1.f)
        {
            ;//
        }

        //-----------------------------------------------------------------------
       /* Texture2D::Texture2D(const Texture2D &Sender)
            : Super(Sender)
            , m_pTextureNode(NULL)
        {
            if (this != &Sender)
            {
                Color = Sender.Color;
                Reference = Sender.Reference;
                m_Texcoord = Sender.m_Texcoord;
            }
        }*/

        //-----------------------------------------------------------------------
        Texture2D::~Texture2D()
        {
            Release();
        }

        //-----------------------------------------------------------------------
        bool Texture2D::Load(const char *filepath, bool ImmidiateLoad /*= true*/)
        {
            bool bResult = false;

            Release(); // release previously

            if (filepath != "")
            {
                m_node = m_pD3DInterface->LoadTexture(filepath);

               /* unsigned char px[] = { 255, 0, 0, 0, 
                                       255, 0, 0, 0,
                                       255, 0, 0, 0,
                                       255, 0, 0, 0 };

                m_node = m_pD3DInterface->LoadTextureFromPixels32((unsigned int*)px, 2, 2, 2, 2);

                m_node->Lock();

                m_node->SetPixel32(1, 1, 0);

                m_node->Unlock();*/
                
                bResult = (m_node != 0);
            }

            return bResult;
        }

        //-----------------------------------------------------------------------
        bool Texture2D::LoadW(const wchar_t *filepath)
        {
            bool bResult = false;

            Release(); // release previously

            if (filepath != L"")
            {
                m_node = m_pD3DInterface->LoadTextureW(filepath);
                bResult = (m_node != 0);
            }

            return bResult;
        }

        //-----------------------------------------------------------------------
        void Texture2D::Release()
        {
            if (m_node)
            {
                m_pD3DInterface->UnregisterTexture(m_node);
                m_node = nullptr;
            }
        }

        //----------------------------------------------------------------------------------------------
        GLuint Texture2D::GetTextureGL(bool noNull /*= true*/) const
        {
            GLuint OutID = (m_node != NULL) ? m_node->m_pSTextureBitmap.texID : 0;
            return (OutID != 0) ? OutID : ((!noNull) ? 0 : m_pD3DInterface->m_pTextureDef->m_pSTextureBitmap.texID);
        }

        //----------------------------------------------------------------------------------------------
        void Texture2D::SetTexcoords(float U, float V, float UL, float VL)
        {
            m_Texcoord.x = U > 1.f ? (U / m_node->m_TGADesc.Width) : U;
            m_Texcoord.y = V > 1.f ? (V / m_node->m_TGADesc.Height) : V;
            m_Texcoord.z = UL > 1.f ? (UL / m_node->m_TGADesc.Width) : UL;
            m_Texcoord.w = VL > 1.f ? (VL / m_node->m_TGADesc.Height) : VL;
        }

        //----------------------------------------------------------------------------------------------
        bool Texture2D::GetInfo(unsigned int &width, unsigned int &height) const
        {
            return (m_node != NULL) ? m_node->GetInfo(width, height) : false;
        }

        //----------------------------------------------------------------------------------------------
        bool Texture2D::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
        {
            return (m_node != NULL) ? m_node->GetDIB(pData, bResample, NewWidth, NewHeight) : false;
        }
    }
}