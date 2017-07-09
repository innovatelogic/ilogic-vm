#include "d3ddriverstdafx.h"
#include <ddraw.h>

namespace oes
{
    namespace d3d
    {
        struct SMipMap
        {
            GLubyte *pixels;
            size_t	nBytes;
        };

        //----------------------------------------------------------------------------------------------
        static void FlipDXT1BlockFull(unsigned char *data)
            // A DXT1 block layout is:
            // [0-1] color0.
            // [2-3] color1.
            // [4-7] color bitmap, 2 bits per pixel.
            // So each of the 4-7 bytes represents one line, flipping a block is just
            // flipping those bytes.
            // Note that http://src.chromium.org/viewvc/chrome/trunk/src/o3d/core/cross/bitmap_dds.cc?view=markup&pathrev=21227
            // contains an error in the last line: data[6]=data[5] is a bug!
        {
            //return;
            unsigned char tmp;

            tmp = data[4];
            data[4] = data[7];
            data[7] = tmp;

            tmp = data[5];
            data[5] = data[6];
            data[6] = tmp;

            //data[4]=data[5]=data[6]=data[7]=0;
        }

        //----------------------------------------------------------------------------------------------
        static void FlipDXT3BlockFull(unsigned char *block)
            // Flips a full DXT3 block in the y direction.
        {
            // A DXT3 block layout is:
            // [0-7]  alpha bitmap, 4 bits per pixel.
            // [8-15] a DXT1 block.

            // We can flip the alpha bits at the byte level (2 bytes per line).
            unsigned char tmp = block[0];
            block[0] = block[6];
            block[6] = tmp;
            tmp = block[1];
            block[1] = block[7];
            block[7] = tmp;
            tmp = block[2];
            block[2] = block[4];
            block[4] = tmp;
            tmp = block[3];
            block[3] = block[5];
            block[5] = tmp;

            // And flip the DXT1 block using the above function.
            FlipDXT1BlockFull(block + 8);
        }

        //----------------------------------------------------------------------------------------------
        static void FlipDXT5BlockFull(unsigned char *block)
            // From http://src.chromium.org/viewvc/chrome/trunk/src/o3d/core/cross/bitmap_dds.cc?view=markup&pathrev=21227
            // Original source contained bugs; fixed here.
        {
            // A DXT5 block layout is:
            // [0]    alpha0.
            // [1]    alpha1.
            // [2-7]  alpha bitmap, 3 bits per pixel.
            // [8-15] a DXT1 block.

            // The alpha bitmap doesn't easily map lines to bytes, so we have to
            // interpret it correctly.  Extracted from
            // http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt :
            //
            //   The 6 "bits" bytes of the block are decoded into one 48-bit integer:
            //
            //     bits = bits_0 + 256 * (bits_1 + 256 * (bits_2 + 256 * (bits_3 +
            //                   256 * (bits_4 + 256 * bits_5))))
            //
            //   bits is a 48-bit unsigned integer, from which a three-bit control code
            //   is extracted for a texel at location (x,y) in the block using:
            //
            //       code(x,y) = bits[3*(4*y+x)+1..3*(4*y+x)+0]
            //
            //   where bit 47 is the most significant and bit 0 is the least
            //   significant bit.

            // From Chromium (source was buggy)
            unsigned int line_0_1 = block[2] + 256 * (block[3] + 256 * block[4]);
            unsigned int line_2_3 = block[5] + 256 * (block[6] + 256 * block[7]);
            // swap lines 0 and 1 in line_0_1.
            unsigned int line_1_0 = ((line_0_1 & 0x000fff) << 12) | ((line_0_1 & 0xfff000) >> 12);
            // swap lines 2 and 3 in line_2_3.
            unsigned int line_3_2 = ((line_2_3 & 0x000fff) << 12) | ((line_2_3 & 0xfff000) >> 12);
            block[2] = line_3_2 & 0xff;
            block[3] = (line_3_2 & 0xff00) >> 8;
            block[4] = (line_3_2 & 0xff0000) >> 16;
            block[5] = line_1_0 & 0xff;
            block[6] = (line_1_0 & 0xff00) >> 8;
            block[7] = (line_1_0 & 0xff0000) >> 16;

            // And flip the DXT1 block using the above function.
            FlipDXT1BlockFull(block + 8);
        }

        //----------------------------------------------------------------------------------------------
        SDDS_IMAGE_DATA* D3DDriver::LoadDDSData(const wchar_t *filename)
        {
            int factor = 1;
            int bufferSize = 0;

            // Open the file
            FILE *pFile = _wfopen(filename, L"rb");

            if (pFile == NULL)
            {
                //char str[255];
                //sprintf(str, "loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename);
                //MessageBox(NULL, str, "ERROR", MB_OK | MB_ICONEXCLAMATION);
                return NULL;
            }

            // Verify the file is a true .dds file
            char filecode[4];
            fread(filecode, 1, 4, pFile);

            if (strncmp(filecode, "DDS ", 4) != 0)
            {
                //char str[255];
                //sprintf(str, "The file \"%s\" doesn't appear to be a valid .dds file!", filename);
                //MessageBox(NULL, str, "ERROR", MB_OK | MB_ICONEXCLAMATION);
                fclose(pFile);
                return NULL;
            }

            // Get the surface descriptor
            DDSURFACEDESC2 ddsd;
            fread(&ddsd, sizeof(ddsd), 1, pFile);

            unsigned int nWidth = ddsd.dwWidth;
            unsigned int nHeight = ddsd.dwHeight;
            unsigned int nMipMaps = ddsd.dwMipMapCount;

            SDDS_IMAGE_DATA *pDDSImageData = (SDDS_IMAGE_DATA*)malloc(sizeof(SDDS_IMAGE_DATA));
            memset(pDDSImageData, 0, sizeof(SDDS_IMAGE_DATA));

            unsigned int blockSize = 16;
            unsigned int nBytes = 0;
            unsigned int formatUncomp = 0;

            if (ddsd.ddpfPixelFormat.dwFourCC & DDPF_FOURCC) // compressed
            {
                unsigned int fourCC = ddsd.ddpfPixelFormat.dwFourCC;
                if (fourCC == FOURCC_DXT1)
                {
                    pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; // DXT1's compression ratio is 8:1
                    blockSize = 8;
                }
                else if (fourCC == FOURCC_DXT3)
                {
                    pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; // DXT3's compression ratio is 4:1
                }
                else if (fourCC == FOURCC_DXT5)
                {
                    pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; // DXT5's compression ratio is 4:1
                }
                else
                {
                    //MessageBox(NULL, "Unknown compression format", "ERROR", MB_OK | MB_ICONEXCLAMATION);
                    return 0;
                }
            }
            else if (ddsd.ddpfPixelFormat.dwRGBBitCount == 32)
            {
                formatUncomp = DDS_FORMAT_RGBA8;
                nBytes = nWidth * nHeight * (32 / 8);
            }
            else if (ddsd.ddpfPixelFormat.dwRGBBitCount == 24)
            {
                formatUncomp = DDS_FORMAT_RGB8;
                nBytes = nWidth * nHeight * (24 / 8);
            }
            else
            {
                //MessageBox(NULL, "Unknown DDS format", "ERROR", MB_OK | MB_ICONEXCLAMATION);
                return 0;
            }

            // Load all Mip maps
            int nMipWidth = nWidth;
            int nMipHeight = nHeight;

            SMipMap *pMips = new SMipMap[nMipMaps];

            unsigned char *pTempBuffer = 0;
            unsigned int nTotalSize = 0;
            unsigned int byteSize = 0;

            if (pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
                pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
                pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
            {
                byteSize = ((nMipWidth + 3) / 4) * ((nMipHeight + 3) / 4) * blockSize;
            }
            else
            {
                byteSize = nMipWidth * nMipHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);
            }

            assert(byteSize > 0);
            pTempBuffer = new unsigned char[byteSize];

            for (unsigned int Index = 0; Index < nMipMaps && Index < MAX_MIPMAP_NUM; ++Index)
            {
                if (nMipWidth == 0) nMipWidth = 1;
                if (nMipHeight == 0) nMipHeight = 1;

                if (pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ||
                    pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
                    pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
                {
                    nBytes = ((nMipWidth + 3) / 4) * ((nMipHeight + 3) / 4) * blockSize;
                }
                else
                {
                    nBytes = nMipWidth * nMipHeight * (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);
                }

                pMips[Index].pixels = (unsigned char*)malloc(nBytes * sizeof(unsigned char));
                pMips[Index].nBytes = nBytes;
                nTotalSize += nBytes;

                fread(pMips[Index].pixels, 1, nBytes, pFile);

                /*if (Index > 0)
                {
                    for (unsigned int i = 0; i < nBytes; ++i)
                    {
                        pMips[Index].pixels[i] *= (byte)Index;
                    }
                }*/

                if (formatUncomp != DDS_FORMAT_RGBA8)
                {
                    memcpy(pTempBuffer, pMips[Index].pixels, nBytes);

                    unsigned int widBytes = ((nMipWidth + 3) / 4) * blockSize;
                    unsigned char *s = pTempBuffer;
                    unsigned char *d = pMips[Index].pixels + ((nMipHeight + 3) / 4 - 1) * widBytes;

                    for (int j = 0; j < (nMipHeight + 3) / 4; j++)
                    {
                        memcpy(d, s, widBytes);

                        if (blockSize == 8)
                        {
                            const unsigned int nBlocks = widBytes / blockSize;
                            for (unsigned int k = 0; k < nBlocks; ++k) {
                                FlipDXT1BlockFull(d + k * blockSize);
                            }
                        }
                        else
                        {
                            // DXT3
                            if (pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
                            {
                                // DXT3
                                const unsigned int nBlocks = widBytes / blockSize;
                                for (unsigned int k = 0; k < nBlocks; ++k) {
                                    FlipDXT3BlockFull((unsigned char*)d + k * blockSize);
                                }
                            }
                            else
                            {
                                // DXT5
                                const unsigned int nBlocks = widBytes / blockSize;
                                for (unsigned int k = 0; k < nBlocks; ++k) {
                                    FlipDXT5BlockFull((unsigned char*)d + k * blockSize);
                                }
                            }
                        }
                        s += widBytes;
                        d -= widBytes;
                    }
                }

                nMipWidth /= 2;
                nMipHeight /= 2;
            }

            delete[] pTempBuffer;

            pDDSImageData->pixels = (unsigned char*)malloc(nTotalSize * sizeof(unsigned char));

            unsigned int nStride = 0;
            for (unsigned int Index = 0; Index < nMipMaps; ++Index)
            {
                memcpy(pDDSImageData->pixels + nStride, pMips[Index].pixels, pMips[Index].nBytes * sizeof(unsigned char));
                nStride += pMips[Index].nBytes;
            }

            for (unsigned int Index = 0; Index < nMipMaps; ++Index) {
                free(pMips[Index].pixels);
            }
            delete[] pMips;

            // Close the file
            fclose(pFile);

            pDDSImageData->width = ddsd.dwWidth;
            pDDSImageData->height = ddsd.dwHeight;
            pDDSImageData->numMipMaps = ddsd.dwMipMapCount;
            pDDSImageData->components = (ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1) ? 3 : 4;

            return pDDSImageData;
        }

        //----------------------------------------------------------------------------------------------
        bool D3DDriver::LoadDDS(TextureNode *pNode, const wchar_t *filename)
        {
            SDDS_IMAGE_DATA *pDDSImageData = LoadDDSData(filename);

            if (pDDSImageData != NULL)
            {
                int nHeight = pDDSImageData->height;
                int nWidth = pDDSImageData->width;
                int nNumMipMaps = pDDSImageData->numMipMaps;

                glGenTextures(1, &pNode->m_pSTextureBitmap.texID);
                glBindTexture(GL_TEXTURE_2D, pNode->m_pSTextureBitmap.texID);

                //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                int nSize;
                int nOffset = 0;

                // Load the mip-map levels
                for (int i = 0; i < nNumMipMaps; ++i)
                {
                    if (nWidth == 0) nWidth = 1;
                    if (nHeight == 0) nHeight = 1;

                    if (pDDSImageData->format != 0)
                    {
                        int nBlockSize = (pDDSImageData->format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;

                        nSize = ((nWidth + 3) / 4) * ((nHeight + 3) / 4) * nBlockSize;

                        glCompressedTexImage2DARB(GL_TEXTURE_2D,
                            i,
                            pDDSImageData->format,
                            nWidth,
                            nHeight,
                            0,
                            nSize,
                            pDDSImageData->pixels + nOffset);
                    }
                    else
                    {
                        nSize = (nWidth * nHeight) * 3;

                        glTexImage2D(GL_TEXTURE_2D,
                            i,
                            3,
                            nWidth,
                            nHeight,
                            0,
                            GL_RGB,
                            GL_UNSIGNED_BYTE,
                            pDDSImageData->pixels + nOffset);
                    }

                    nOffset += nSize;

                    // Half the image size for the next mip-map level...
                    nWidth = (nWidth / 2);
                    nHeight = (nHeight / 2);
                }
            }

            if (pDDSImageData != NULL)
            {
                if (pDDSImageData->pixels != NULL) {
                    free(pDDSImageData->pixels);
                }

                free(pDDSImageData);
            }
            return true;
        }

        //----------------------------------------------------------------------------------------------
        bool D3DDriver::LoadTextureFromPixels32(
            TextureNode *node,
            unsigned int *pixels,
            unsigned int imgWidth,
            unsigned int imgHeight,
            unsigned int texWidth,
            unsigned int texHeight)
        {
            const int nHeight = texHeight;
            const int nWidth = texWidth;

            glGenTextures(1, &node->m_pSTextureBitmap.texID);
            glBindTexture(GL_TEXTURE_2D, node->m_pSTextureBitmap.texID);

            //Generate texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            //Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_LINEAR
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            //Unbind texture
            glBindTexture(GL_TEXTURE_2D, NULL);

            node->m_pSTextureBitmap.width = nWidth;
            node->m_pSTextureBitmap.height = nHeight;

            //Check for error
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                printf("Error loading texture from %p pixels! %s\n", pixels, gluErrorString(error));
                return false;
            }
            return true;
        }
    }
}