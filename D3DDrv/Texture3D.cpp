#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        Texture3D::Texture3D(const D3DDriver * Interface)
            : Super(Interface)
            , pVolumeTextureNode(NULL)
        {
            ;//
        }

        //----------------------------------------------------------------------------------------------
        Texture3D::Texture3D(const Texture3D &Sender)
            : Super(Sender)
            , pVolumeTextureNode(NULL)
        {
            if (this != &Sender)
            {
                Reference = Sender.Reference;
            }
        }

        //----------------------------------------------------------------------------------------------
        Texture3D::~Texture3D()
        {
            Release();
        }

        //----------------------------------------------------------------------------------------------
        bool Texture3D::Load(const std::string & URL, unsigned int width, unsigned int height, unsigned int depth, bool ImmidiateLoad /*= true*/)
        {
            bool bResult = false;

            /*Release(); // release previously

            if (URL != "")
            {
                pVolumeTextureNode = m_pD3DInterface->LoadVolumeTexture(URL.c_str(), width, height, depth, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ImmidiateLoad);
                bResult = (pVolumeTextureNode != 0);
            }*/

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void Texture3D::Release()
        {
            if (pVolumeTextureNode)
            {
                pVolumeTextureNode->Release();
                pVolumeTextureNode = NULL;
            }
        }

        //----------------------------------------------------------------------------------------------
        /*LPDIRECT3DVOLUMETEXTURE9 Texture3D::GetTexture(bool NoNull ) const
        {
            return pVolumeTextureNode->VolumeTexture;
        }*/
    }
}