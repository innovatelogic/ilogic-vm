#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        TextureNode::TextureNode(D3DDriver *d3d)
            : m_p3DDriver(d3d)
            , m_PixelsTmp(nullptr)
        {
            m_pRenderContext = m_p3DDriver->GetCurrentContext();
        }

        //----------------------------------------------------------------------------------------------
        TextureNode::~TextureNode()
        {
            if (m_PixelsTmp) {
                delete[] m_PixelsTmp;
            }
        }

        //----------------------------------------------------------------------------------------------
        bool TextureNode::GetInfo(unsigned int &Width, unsigned int &Height) const
        {
            bool bResult = false;

            /*D3DSURFACE_DESC desc;
            if (SUCCEEDED(TextureBitmap->GetLevelDesc(0, &desc)))
            {
                Width = desc.Width;
                Height = desc.Height;
                bResult = true;
            }*/

            return bResult;
        }
        //----------------------------------------------------------------------------------------------
        /*BYTE D3DGetFormatBitDepth(D3DFORMAT fmtFormat)
        {
            switch (fmtFormat)
            {
            case D3DFMT_DXT1:
                return 4;
            case D3DFMT_R3G3B2:
            case D3DFMT_A8:
            case D3DFMT_P8:
            case D3DFMT_L8:
            case D3DFMT_A4L4:
            case D3DFMT_DXT2:
            case D3DFMT_DXT3:
            case D3DFMT_DXT4:
            case D3DFMT_DXT5:
                return 8;
            case D3DFMT_X4R4G4B4:
            case D3DFMT_A4R4G4B4:
            case D3DFMT_A1R5G5B5:
            case D3DFMT_X1R5G5B5:
            case D3DFMT_R5G6B5:
            case D3DFMT_A8R3G3B2:
            case D3DFMT_A8P8:
            case D3DFMT_A8L8:
            case D3DFMT_V8U8:
            case D3DFMT_L6V5U5:
            case D3DFMT_D16_LOCKABLE:
            case D3DFMT_D15S1:
            case D3DFMT_D16:
            case D3DFMT_L16:
            case D3DFMT_INDEX16:
            case D3DFMT_CxV8U8:
            case D3DFMT_G8R8_G8B8:
            case D3DFMT_R8G8_B8G8:
            case D3DFMT_R16F:
                return 16;
            case D3DFMT_R8G8B8:
                return 24;
            case D3DFMT_A2W10V10U10:
            case D3DFMT_A2B10G10R10:
            case D3DFMT_A2R10G10B10:
            case D3DFMT_X8R8G8B8:
            case D3DFMT_A8R8G8B8:
            case D3DFMT_X8L8V8U8:
            case D3DFMT_Q8W8V8U8:
            case D3DFMT_V16U16:
                //case D3DFMT_W11V11U10: // Dropped in DX9.0
            case D3DFMT_UYVY:
            case D3DFMT_YUY2:
            case D3DFMT_G16R16:
            case D3DFMT_D32:
            case D3DFMT_D24S8:
            case D3DFMT_D24X8:
            case D3DFMT_D24X4S4:
            case D3DFMT_D24FS8:
            case D3DFMT_D32F_LOCKABLE:
            case D3DFMT_INDEX32:
            case D3DFMT_MULTI2_ARGB8:
            case D3DFMT_G16R16F:
            case D3DFMT_R32F:
                return 32;
            case D3DFMT_A16B16G16R16:
            case D3DFMT_Q16W16V16U16:
            case D3DFMT_A16B16G16R16F:
            case D3DFMT_G32R32F:
                return 64;
            case D3DFMT_A32B32G32R32F:
                return 128; // !
            }
            return 0;
        }*/

        //----------------------------------------------------------------------------------------------
        static inline double spline_cube(double value)
        {
            return value <= 0.0 ? 0.0 : value * value * value;
        }

        //----------------------------------------------------------------------------------------------
        static inline double spline_weight(double value)
        {
            return (spline_cube(value + 2) - 4 * spline_cube(value + 1) + 6 * spline_cube(value) - 4 * spline_cube(value - 1)) / 6;
        }

        //----------------------------------------------------------------------------------------------
        bool TextureNode::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
        {
            bool bResult = false;

            /*D3DSURFACE_DESC desc;
            if (SUCCEEDED(TextureBitmap->GetLevelDesc(0, &desc)))
            {
                D3DLOCKED_RECT locked;
                if (SUCCEEDED(TextureBitmap->LockRect(0, &locked, NULL, D3DLOCK_DISCARD)))
                {
                    unsigned int Width = desc.Width;
                    unsigned int Height = desc.Height;

                    if (bResample && (NewWidth > 0 && NewHeight > 0))
                    {
                        Width = NewWidth;
                        Height = NewHeight;

                        *pData = (BYTE*)new DWORD[Height * Width];

                        const BYTE BitsPPx = D3DGetFormatBitDepth(desc.Format);
                        const BYTE FormatSize = BitsPPx / 8;

                        if (*pData)
                        {
                            // Nearest neighbour
                            //{
                            //	double scaleWidth =  (double)Width / (double)desc.Width;
                            //	double scaleHeight = (double)Height / (double)desc.Height;

                            //	for (size_t y = 0; y < desc.Height; ++y)
                            //	{
                            //		for (size_t x = 0; x < desc.Width; ++x)
                            //		{
                            //			size_t pxIdxSource = x + (desc.Width * y);
                            //			size_t pxIdxDest = (int)((x * scaleWidth)) + (Width * (int)((y * scaleHeight)));

                            //			memcpy(((BYTE*)*pData) + (pxIdxDest * FormatSize), ((BYTE*)locked.pBits) + (pxIdxSource * FormatSize), FormatSize);
                            //		}
                            //	}
                            //}

                            // bicubic
                            for (size_t y = 0; y < Height; ++y)
                            {
                                double pxSrcY = double(y * desc.Height) / Height;
                                double dy = pxSrcY - (int)pxSrcY;

                                for (size_t x = 0; x < Width; ++x)
                                {
                                    double pxSrcX = double(x * desc.Width) / Width;
                                    double dx = pxSrcX - (int)pxSrcX;

                                    double SumR = 0, SumG = 0, SumB = 0, SumA = 0;
                                    for (int k = -1; k <= 2; ++k)
                                    {
                                        int y_offset = (pxSrcY + k) < 0.0 ? 0 : (pxSrcY + k) >= desc.Height	? desc.Height - 1 : (int)(pxSrcY + k);

                                        for (int i = -1; i <= 2; ++i)
                                        {
                                            int x_offset = (pxSrcX + i) < 0.0 ? 0 : (pxSrcX + i) >= desc.Width ? desc.Width - 1 : (int)(pxSrcX + i);

                                            int src_pixel_index = y_offset * desc.Width + x_offset;

                                            double pixel_weight = spline_weight(i - dx) * spline_weight(k - dy);

                                            SumR += *(((BYTE*)locked.pBits) + (src_pixel_index * FormatSize) + 0) * pixel_weight;
                                            SumG += *(((BYTE*)locked.pBits) + (src_pixel_index * FormatSize) + 1) * pixel_weight;
                                            SumB += *(((BYTE*)locked.pBits) + (src_pixel_index * FormatSize) + 2) * pixel_weight;
                                        }
                                    }

                                    size_t pxIdxDest = x + (Width * y);
                                    *(((BYTE*)*pData) + (pxIdxDest * FormatSize) + 0) = (BYTE)(SumR + 0.5);
                                    *(((BYTE*)*pData) + (pxIdxDest * FormatSize) + 1) = (BYTE)(SumG + 0.5);
                                    *(((BYTE*)*pData) + (pxIdxDest * FormatSize) + 2) = (BYTE)(SumB + 0.5);
                                    *(((BYTE*)*pData) + (pxIdxDest * FormatSize) + 3) = 0;
                                }
                            }
                        }
                    }
                    else
                    {
                        *pData = (BYTE*)new DWORD[desc.Height * desc.Width];
                        memcpy(*pData, locked.pBits, (desc.Width * desc.Height) * sizeof(DWORD));
                    }

                    TextureBitmap->UnlockRect(0);
                    bResult = true;
                }
            }*/
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        bool TextureNode::Lock()
        {
            bool bResult = false;
            if (m_PixelsTmp == nullptr && m_pSTextureBitmap.texID != 0)
            {
                GLuint size = m_pSTextureBitmap.height * m_pSTextureBitmap.width;

                m_PixelsTmp = new GLuint[size];

                glBindTexture(GL_TEXTURE_2D, m_pSTextureBitmap.texID);

                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_PixelsTmp);

                glBindTexture(GL_TEXTURE_2D, NULL);

                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        bool TextureNode::Unlock()
        {
            bool bResult = false;
            if (m_PixelsTmp && m_pSTextureBitmap.texID != 0)
            {
                glBindTexture(GL_TEXTURE_2D, m_pSTextureBitmap.texID);

                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pSTextureBitmap.width, m_pSTextureBitmap.height, GL_RGBA, GL_UNSIGNED_BYTE, m_PixelsTmp);

                delete[] m_PixelsTmp;
                m_PixelsTmp = nullptr;

                glBindTexture(GL_TEXTURE_2D, NULL);

                bResult = true;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        void TextureNode::DoRelease()
        {
            glDeleteTextures(1, &m_pSTextureBitmap.texID);

            Refcount::DoRelease();
        }
    }
}