#include "d3ddriverstdafx.h"

#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/fttrigon.h"

static const GLubyte uTGAcompare[12] = {0, 0, 2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
static const GLubyte cTGAcompare[12] = {0, 0, 10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

static const char *tagVertexShader = "[VertexShader]";
static const char *tagFragmentShader = "[FragmentShader]";

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        ///This function gets the first power of 2 >= the
        ///int that we pass it.
        inline int _next_p2(int a)
        {
            int rval = 1;
            while (rval < a) { rval <<= 1; }
            return rval;
        }

        //----------------------------------------------------------------------------------------------
        void _fill_texture_data(uint32_t ch, CFontNode::font_info_t* font, uint32_t texture_width, uint8_t* texture_data)
        {
            CFontNode::char_info_t* char_info = &font->ch[ch];
            uint8_t* char_bmp = char_info->bitmap;

            int bmp_pos = 0;
            int tex_pos = 0;

            int char_x = char_info->x;
            int char_y = char_info->y;
            int char_width = char_info->width;
            int char_height = char_info->height;

            for (int bmp_y = 0; bmp_y < char_height; bmp_y++)
            {
                for (int bmp_x = 0; bmp_x < char_width; bmp_x++)
                {
                    bmp_pos = 2 * (bmp_x + bmp_y * char_width);
                    tex_pos = 2 * ((char_x + bmp_x) + ((char_y + bmp_y) * texture_width));

                    texture_data[tex_pos] = char_bmp[bmp_pos];
                    texture_data[tex_pos + 1] = char_bmp[bmp_pos + 1];
                }
            }
        }

        //----------------------------------------------------------------------------------------------
        ///Create a display list coresponding to the give character.
        void _make_dlist(FT_Face face, char ch, GLuint list_base, GLuint * tex_base)
        {
            //The first thing we do is get FreeType to render our character
            //into a bitmap.  This actually requires a couple of FreeType commands:

            //Load the Glyph for our character.
            if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
                return;
            }

            //Move the face's glyph into a Glyph object.
            FT_Glyph glyph;
            if (FT_Get_Glyph(face->glyph, &glyph)) {
                return;
            }

            //Convert the glyph to a bitmap.
            FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
            FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

            //This reference will make accessing the bitmap easier
            FT_Bitmap &bitmap = bitmap_glyph->bitmap;

            //Use our helper function to get the widths of
            //the bitmap data that we will need in order to create
            //our texture.
            int width = _next_p2(bitmap.width);
            int height = _next_p2(bitmap.rows);

            //Allocate memory for the texture data.
            GLubyte* expanded_data = new GLubyte[2 * width * height];

            //Here we fill in the data for the expanded bitmap.
            //Notice that we are using two channel bitmap (one for
            //luminocity and one for alpha), but we assign
            //both luminocity and alpha to the value that we
            //find in the FreeType bitmap. 
            //We use the ?: operator so that value which we use
            //will be 0 if we are in the padding zone, and whatever
            //is the the Freetype bitmap otherwise.
            for (int j = 0; j < height; j++) {
                for (int i = 0; i < width; i++)
                {
                    expanded_data[2 * (i + j * width)] =
                        expanded_data[2 * (i + j * width) + 1] =
                        (i >= bitmap.width || j >= bitmap.rows) ?
                        0 : bitmap.buffer[i + bitmap.width * j];
                }
            }

            //Now we just setup some texture paramaters.
            glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            //Here we actually create the texture itself, notice
            //that we are using GL_LUMINANCE_ALPHA to indicate that
            //we are using 2 channel data.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

            //With the texture created, we don't need to expanded data anymore
            delete[] expanded_data;

            //So now we can create the display list
            glNewList(list_base + ch, GL_COMPILE);

            //glBindTexture(GL_TEXTURE_2D, tex_base[ch]);

            glPushMatrix();

            //first we need to move over a little so that
            //the character has the right amount of space
            //between it and the one before it.
            glTranslatef((float)bitmap_glyph->left, 0.f, 0.f);

            //Now we move down a little in the case that the
            //bitmap extends past the bottom of the line 
            //(this is only true for characters like 'g' or 'y'.
            glTranslatef(0.f, (float)bitmap_glyph->top - bitmap.rows, 0.f);

            //Now we need to account for the fact that many of
            //our textures are filled with empty padding space.
            //We figure what portion of the texture is used by 
            //the actual character and store that information in 
            //the x and y variables, then when we draw the
            //quad, we will only reference the parts of the texture
            //that we contain the character itself.
            float x = (float)bitmap.width / (float)width,
                y = (float)bitmap.rows / (float)height;

            //Here we draw the texturemaped quads.
            //The bitmap that we got from FreeType was not 
            //oriented quite like we would like it to be,
            //so we need to link the texture to the quad
            //so that the result will be properly aligned.
            glBegin(GL_QUADS);
            glTexCoord2d(0, 0); glVertex2f(0, (float)bitmap.rows);
            glTexCoord2d(0, y); glVertex2f(0, 0);
            glTexCoord2d(x, y); glVertex2f((float)bitmap.width, 0);
            glTexCoord2d(x, 0); glVertex2f((float)bitmap.width, (float)bitmap.rows);
            glEnd();
            glPopMatrix();
            glTranslatef((float)(face->glyph->advance.x >> 6), 0, 0);

            //increment the raster position as if we were a bitmap font.
            //(only needed if you want to calculate text length)
            //glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

            //Finnish the display list
            glEndList();
        }

        //----------------------------------------------------------------------------------------------
        template<class TCLASS, class TCONT_CLASS, class TCLASS_DRIVER>
        TCLASS* TAllocStrategyShared(const wchar_t *url, TCONT_CLASS &refContainer, TCLASS_DRIVER *pDrv)
        {
            TCLASS *pOutNode = nullptr;

            if (url)
            {
                TCONT_CLASS::const_iterator Iter = refContainer.find(url);
                if (Iter != refContainer.end())
                {
                    Iter->second->AddRef();
                    return Iter->second;
                }
                else
                {
                    pOutNode = new TCLASS(pDrv);
                    refContainer.insert(std::make_pair(url, pOutNode));
                }
            }
            
            return pOutNode ? pOutNode : new TCLASS(pDrv);
        }

        //----------------------------------------------------------------------------------------------
        template <class TCLASS, class TCONT_CLASS>
        bool TReleaseStrategyBOOL(const TCLASS *pNode, TCONT_CLASS &refContainer)
        {
            bool bResult = false;

            TCONT_CLASS::const_iterator Iter = refContainer.begin();

            while (Iter != refContainer.end())
            {
                if (Iter->second == pNode)
                {
                    long refCount = Iter->second->Release();

                    assert(refCount >= 0);

                    if (refCount == 0) {
                        refContainer.erase(Iter);
                    }
                    bResult = true;
                    break;
                }
                ++Iter;
            }

            if (bResult == false)
            {
                const_cast<TCLASS*>(pNode)->Release();
            }

            //assert(bResult == true); // wrong context

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        template <class TCLASS, class TCONT_CLASS>
        bool TVecReleaseStrategyBOOL(const TCLASS *pNode, TCONT_CLASS &refContainer)
        {
            bool bResult = false;

            TCONT_CLASS::const_iterator Iter = refContainer.begin();

            while (Iter != refContainer.end())
            {
                if (*Iter == pNode)
                {
                    assert((*Iter)->IsClear());
                    refContainer.erase(Iter);
                    bResult = true;
                    break;
                }
                ++Iter;
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        template<class TCLASS, class TCONT_CLASS>
        TCLASS* TFindStrategyW(const wchar_t *pUrl, TCONT_CLASS &refContainer)
        {
            TCONT_CLASS::const_iterator Iter = refContainer.find(pUrl);

            if (pUrl && Iter != refContainer.end()) // non empty URL for streamable resources that should load once
            {
                //if (Iter->second->IsClear())
                {
                    return Iter->second;
                }
            }
            return 0;
        }
        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        SRenderContext::SRenderContext(D3DDriver *pDriver)
            : m_pDriver(pDriver)
            , m_bWireframe(false)
        {
            assert(m_pDriver);
        }

        //----------------------------------------------------------------------------------------------
        SRenderContext::~SRenderContext()
        {

        }

        //----------------------------------------------------------------------------------------------
        TextureNode* SRenderContext::LoadTextureW(const wchar_t *URL)
        {
            TextureNode *pNodeTexture = TAllocStrategyShared
                <TextureNode, TMapTextureNodeW, D3DDriver>(URL, m_MapTextureNodesW, m_pDriver);

            assert(pNodeTexture);

            if (pNodeTexture->GetRefCount() != 1) { // check first init
                return pNodeTexture;
            }

            if (wcsstr(URL, L".dds")){
                m_pDriver->LoadDDS(pNodeTexture, URL);
            }
            else{
                LoadTGAW(pNodeTexture, URL);
            }
            return pNodeTexture;
        }

        //----------------------------------------------------------------------------------------------
        TextureNode* SRenderContext::LoadTextureFromPixels32(
            unsigned int *pixels,
            unsigned int imgWidth,
            unsigned int imgHeight,
            unsigned int texWidth,
            unsigned int texHeight)
        {
            TextureNode *pNodeTexture = TAllocStrategyShared
                <TextureNode, TMapTextureNodeW, D3DDriver>(nullptr, m_MapTextureNodesW, m_pDriver);

            assert(pNodeTexture);

            m_pDriver->LoadTextureFromPixels32(pNodeTexture, pixels, imgWidth, imgHeight, texWidth, texHeight);

            return pNodeTexture;
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::UnregisterTexture(TextureNode *pNode)
        {
            return TReleaseStrategyBOOL<TextureNode, TMapTextureNodeW>(pNode, m_MapTextureNodesW);
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffectNode*	SRenderContext::LoadMaterialEffectW(const wchar_t *URL)
        {
            std::wstring fileName(URL);

            size_t idx_find = fileName.find_last_of(L"/\\");
            if (idx_find != std::wstring::npos){
                fileName = fileName.substr(idx_find + 1);
            }

            MaterialEffectNode *pEffectNode = TAllocStrategyShared
                <MaterialEffectNode, TMapMaterialEffectsW, D3DDriver>(fileName.c_str(), m_MapMaterialEffects, m_pDriver);

            if (pEffectNode->GetRefCount() == 1) // check first init
            {
                if (pEffectNode->CompileEffectW(URL))
                {
                    size_t tagVSLen = strlen(tagVertexShader);
                    size_t tagFSLen = strlen(tagFragmentShader);

                    char     shaderName[0x100]; // 256
                    uint8_t  *shaderSource = nullptr;
                    uint32_t sourceLength = 0;

                    pEffectNode->m_ShaderProgram = glCreateProgram();

                    assert(pEffectNode->m_ShaderProgram != 0);

                    memset(shaderName, 0, 0xFF);

                    GLuint shaderV = glCreateShader(GL_VERTEX_SHADER);
                    GLuint shaderF = glCreateShader(GL_FRAGMENT_SHADER);

                    assert(shaderV && shaderF);

                    if (shaderV != 0 && shaderF != 0)
                    {
                        if (LoadFile(URL, true, &shaderSource, &sourceLength))
                        {
                            const char *pChVS = strstr((const char*)shaderSource, tagVertexShader);
                            const char *pChFS = strstr((const char*)shaderSource, tagFragmentShader);

                            // Vertex shader should be first 
                            if (pChVS && pChFS)
                            {
                                size_t pChFSLen = strlen(pChFS); // size with tag
                                size_t pChVSLen = (unsigned int)pChFS - (unsigned int)shaderSource;

                                const char  *pSourceVS = pChVS + tagVSLen;
                                const int VSLen = pChVSLen - tagVSLen;

                                glShaderSource(shaderV, 1, (const GLchar**)&(pSourceVS), (const GLint*)&VSLen);
                                glCompileShader(shaderV);

                                if (ShaderStatus(shaderV, GL_COMPILE_STATUS) != GL_TRUE)
                                {
                                    glDeleteShader(shaderV);
                                    glDeleteProgram(pEffectNode->m_ShaderProgram);
                                    delete[] shaderSource;
                                    return 0;
                                }

                                glAttachShader(pEffectNode->m_ShaderProgram, shaderV);
                                glDeleteShader(shaderV);

                                // fragment shader
                                const char  *pSourceFS = pChFS + tagFSLen;
                                const int FSLen = strlen(pSourceFS); //(pChFSLen - tagFSLen) - 1;  

                                glShaderSource(shaderF, 1, (const GLchar**)&pSourceFS, (const GLint*)&FSLen);
                                glCompileShader(shaderF);

                                if (ShaderStatus(shaderF, GL_COMPILE_STATUS) != GL_TRUE)
                                {
                                    glDeleteShader(shaderF);
                                    glDeleteProgram(pEffectNode->m_ShaderProgram);
                                    delete[] shaderSource;

                                    return 0;
                                }

                                glAttachShader(pEffectNode->m_ShaderProgram, shaderF);
                                glDeleteShader(shaderF);

                                delete[] shaderSource;
                            }
                        }

                        glLinkProgram(pEffectNode->m_ShaderProgram);

                        GLint Success;
                        GLchar ErrorLog[1024] = { 0 };

                        glValidateProgram(pEffectNode->m_ShaderProgram);
                        glGetProgramiv(pEffectNode->m_ShaderProgram, GL_VALIDATE_STATUS, &Success);

                        if (!Success)
                        {
                            glGetProgramInfoLog(pEffectNode->m_ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
                            //MessageBox(NULL, (const char*)ErrorLog, "Fx Compile Error", MB_OK|MB_ICONEXCLAMATION);
                            return 0;
                        }
                    }
                    else
                    {
                        glDeleteShader(shaderV);
                        glDeleteShader(shaderF);
                        glDeleteProgram(pEffectNode->m_ShaderProgram);
                    }
                }
            }

            return pEffectNode;
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::UnregisterMaterialEffect(MaterialEffectNode *pNode)
        {
            return TReleaseStrategyBOOL<MaterialEffectNode, TMapMaterialEffectsW>(pNode, m_MapMaterialEffects);
        }

        //----------------------------------------------------------------------------------------------
        CFontNode* SRenderContext::LoadFontW(const char *Name, int Size)
        {
            CFontNode *pNodeFont = NULL;

            TVecFontNodes::iterator Iter = m_VecFontNodes.begin();

            while (Iter != m_VecFontNodes.end())
            {
                if (!strcmp((*Iter)->m_FaceName.c_str(), Name) && (*Iter)->m_fHeight == Size)
                {
                    if ((*Iter)->IsClear())
                    {
                        pNodeFont = *Iter;
                    }
                    else
                    {
                        (*Iter)->AddRef();
                        return *Iter;
                    }
                    break;
                }
                ++Iter;
            }

            if (!pNodeFont)
            {
                pNodeFont = new CFontNode(m_pDriver, Name, (float)Size);
                pNodeFont->AddRef();
                m_VecFontNodes.push_back(pNodeFont);
            }

            const uint32_t m_number_of_chars = 128;
            const uint32_t m_texture_max_width = 1024;

            //Create and initialize a freetype font library.
            FT_Library library;
            FT_Face face;

            if (FT_Init_FreeType(&library)) {
                return 0; // error
            }

            //This is where we load in the font information from the file.
            //Of all the places where the code might die, this is the most likely,
            //as FT_New_Face will die if the font file does not exist or is somehow broken.
            if (FT_New_Face(library, Name, 0, &face)) {
                return 0; // error
            }

            //For some twisted reason, Freetype measures font size
            //in terms of 1/64ths of pixels.  Thus, to make a font
            //h pixels high, we need to request a size of h*64.
            //(h << 6 is just a prettier way of writting h*64)
            FT_Set_Char_Size(face, Size << 6, Size << 6, 96, 96);

            pNodeFont->m_Info.ch.resize(128);
            pNodeFont->m_Info.max_height = 0;

            // Variables to hold the size the texture must have to hold all the bitmaps
            int max_width = 0;
            int max_rows = 0;

            for (unsigned int ch = 0; ch < m_number_of_chars; ++ch)
            {
                //Load the Glyph for our character.
                if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
                    return 0;
                }

                //Move the face's glyph into a Glyph object.
                FT_Glyph glyph;
                if (FT_Get_Glyph(face->glyph, &glyph)) {
                    return 0;
                }

                //Convert the glyph to a bitmap.
                FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
                FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

                //This reference will make accessing the bitmap easier
                FT_Bitmap &bitmap = bitmap_glyph->bitmap;

                //Get a pointer to the char info for easy access
                CFontNode::char_info_t *char_info = &pNodeFont->m_Info.ch[ch];
                char_info->width = bitmap.width;
                char_info->height = bitmap.rows;

                //Allocate memory for the bitmap data.
                char_info->bitmap = new uint8_t[2 * char_info->width * char_info->height];
                uint8_t *char_bmp = char_info->bitmap;

                // Fill the bitmap data
                for (int j = 0; j < char_info->height; ++j)
                {
                    for (int i = 0; i < char_info->width; ++i)
                    {
                        char_bmp[2 * (i + j * char_info->width)] =
                            char_bmp[2 * (i + j * char_info->width) + 1] =
                            (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
                    }
                }

                // Accumulate the width of the bitmaps. Increase the rows if we reached the width of the texture
                max_width += char_info->width;
                if (max_width > m_texture_max_width - 1)
                {
                    max_width = char_info->width;
                    max_rows++;
                }

                // Determine what's the maximum height a given character might have
                if (char_info->height > pNodeFont->m_Info.max_height)
                {
                    pNodeFont->m_Info.max_height = char_info->height;
                }

                // Store information about this character. We will use this to print it.
                char_info->row = max_rows;
                char_info->left = bitmap_glyph->left;
                char_info->top = bitmap_glyph->top;
                char_info->advance = face->glyph->advance.x >> 6;
                char_info->x = max_width - char_info->width;
            }

            // Multiply the maximum height a character might have by the amount of rows and calculate the proper (power of two) height the texture must have.
            int texture_height = _next_p2(pNodeFont->m_Info.max_height * (max_rows + 1));

            // Create the temporary buffer for the texture
            uint8_t *texture_data = new uint8_t[m_texture_max_width * texture_height * 2];

            // Fill the texture, set the vertex and uv array values and delete the bitmap
            for (unsigned int ch = 0; ch < m_number_of_chars; ch++)
            {
                CFontNode::char_info_t* char_info = &pNodeFont->m_Info.ch[ch];

                char_info->y = pNodeFont->m_Info.max_height * char_info->row;

                _fill_texture_data(ch, &pNodeFont->m_Info, m_texture_max_width, texture_data);

                //(x,h)
                char_info->uv[0].u = (float)char_info->x / m_texture_max_width;
                char_info->uv[0].v = (float)(char_info->y + char_info->height) / texture_height;
                char_info->v[0].x = 0.0f;
                char_info->v[0].y = (float)char_info->height;

                //(x,y)
                char_info->uv[1].u = (float)char_info->x / m_texture_max_width;
                char_info->uv[1].v = (float)char_info->y / texture_height;
                char_info->v[1].x = 0.0f;
                char_info->v[1].y = 0.0f;

                //(w,y)
                char_info->uv[2].u = (float)(char_info->x + char_info->width) / m_texture_max_width;
                char_info->uv[2].v = (float)(float)char_info->y / texture_height;
                char_info->v[2].x = (float)char_info->width;
                char_info->v[2].y = 0.0f;

                //(w,h)
                char_info->uv[3].u = (float)(char_info->x + char_info->width) / m_texture_max_width;
                char_info->uv[3].v = (float)(char_info->y + char_info->height) / texture_height;
                char_info->v[3].x = (float)char_info->width;
                char_info->v[3].y = (float)char_info->height;

                delete[] pNodeFont->m_Info.ch[ch].bitmap;
            }

            glGenTextures(1, &pNodeFont->m_BitmapTexID);
            glBindTexture(GL_TEXTURE_2D, pNodeFont->m_BitmapTexID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture_max_width, texture_height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texture_data);

            //We don't need the face information now that the display
            //lists have been created, so we free the associated resources.
            FT_Done_Face(face);

            //Ditto for the library.
            FT_Done_FreeType(library);

            return pNodeFont;
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::UnregisterFont(const CFontNode* pNode)
        {
            return TVecReleaseStrategyBOOL<CFontNode, TVecFontNodes>(pNode, m_VecFontNodes);
        }

        //----------------------------------------------------------------------------------------------
        CSceneMeshNode* SRenderContext::GetSceneMeshNode(const wchar_t *pURL)
        {
            return TFindStrategyW<CSceneMeshNode, TSceneMeshNodesW>(pURL, m_MapSceneMeshNodes);
        }

        //----------------------------------------------------------------------------------------------
        CSceneMeshNode*	SRenderContext::AllocSceneMeshNode(const wchar_t *pURL)
        {
            return TAllocStrategyShared<CSceneMeshNode, TSceneMeshNodesW, D3DDriver>(pURL, m_MapSceneMeshNodes, m_pDriver);
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::UnregisterSceneMeshNode(const CSceneMeshNode *pNode)
        {
            return TReleaseStrategyBOOL<CSceneMeshNode, TSceneMeshNodesW>(pNode, m_MapSceneMeshNodes);
        }

        //----------------------------------------------------------------------------------------------
        RenderDriver::RenderTargetNode* SRenderContext::AllocRenderTarget(unsigned int width, unsigned int height)
        {
            RenderDriver::RenderTargetNode *outRT = new RenderDriver::RenderTargetNode(width, height, this);

            m_renderTargets.push_back(outRT);

            return outRT;
        }

        //----------------------------------------------------------------------------------------------
        void SRenderContext::FreeRenderTarget(RenderDriver::RenderTargetNode *rt)
        {
            std::vector<RenderDriver::RenderTargetNode*>::iterator iterFind = std::find(m_renderTargets.begin(), m_renderTargets.end(), rt);
            if (iterFind != m_renderTargets.end())
            {
                (*iterFind)->Release();
                delete *iterFind;

                m_renderTargets.erase(iterFind);
            }
        }

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        bool SRenderContext::LoadTGAW(TextureNode *pNode, const wchar_t *filename)
        {
            FILE * fTGA = 0;											// File pointer to texture file
            fTGA = _wfopen(filename, L"rb");							// Open file for reading

            if (fTGA == NULL)											// If it didn't open....
            {
                //MessageBox(NULL, "Could not open texture file", "ERROR", MB_OK);	// Display an error message
                return false;														// Exit function
            }

            STGAHeader	tgaheader;									// TGA header

            if (fread(&tgaheader, sizeof(STGAHeader), 1, fTGA) == 0)				// Attempt to read 12 byte header from file
            {
                //MessageBox(NULL, "Could not read file header", "ERROR", MB_OK);	// If it fails, display an error message 
                if (fTGA != NULL)													// Check to seeiffile is still open
                {
                    fclose(fTGA);													// If it is, close it
                }
                return false;														// Exit function
            }

            if (memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)			// See if header matches the predefined header of 
            {																		// an Uncompressed TGA image
                LoadUncompressedTGA(pNode, fTGA);									// If so, jump to Uncompressed TGA loading code
            }
            else if (memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)		// See if header matches the predefined header of
            {																		// an RLE compressed TGA image
                LoadCompressedTGA(pNode, fTGA);										// If so, jump to Compressed TGA loading code
            }
            else																	// If header matches neither type
            {
                //MessageBox(NULL, "TGA file be type 2 or type 10 ", "Invalid Image", MB_OK);	// Display an error
                fclose(fTGA);
                return false;														// Exit function
            }

            // Typical Texture Generation Using Data From The TGA ( CHANGE )
            glGenTextures(1, &pNode->m_pSTextureBitmap.texID);				// Create The Texture ( CHANGE )
            glBindTexture(GL_TEXTURE_2D, pNode->m_pSTextureBitmap.texID);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, pNode->m_pSTextureBitmap.bpp / 8,
                pNode->m_pSTextureBitmap.width,
                pNode->m_pSTextureBitmap.height,
                0,
                pNode->m_pSTextureBitmap.type,
                GL_UNSIGNED_BYTE,
                pNode->m_pSTextureBitmap.imageData);

            if (pNode->m_pSTextureBitmap.imageData)						// If Texture Image Exists ( CHANGE )
            {
                free(pNode->m_pSTextureBitmap.imageData);				// Free The Texture Image Memory ( CHANGE )
            }

            return true;												// All went well, continue on
        }

        //----------------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------------
        bool SRenderContext::LoadUncompressedTGA(TextureNode * pNode, FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
        {
            STGADesc &tga = pNode->m_TGADesc;
            STextureBitmap *texture = &pNode->m_pSTextureBitmap;

            if (fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)				// Read TGA header
            {
                //MessageBox(NULL, "Could not read info header", "ERROR", MB_OK);	// Display error

                if (fTGA != NULL)													// if file is still open
                {
                    fclose(fTGA);													// Close it
                }
                return false;
            }

            texture->width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
            texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
            texture->bpp = tga.header[4];										// Determine the bits per pixel

            tga.Width = texture->width;										// Copy width into local structure						
            tga.Height = texture->height;										// Copy height into local structure
            tga.Bpp = texture->bpp;											// Copy BPP into local structure

            if ((texture->width <= 0) || (texture->height <= 0) ||
                ((texture->bpp != 24) && (texture->bpp != 32)))	// Make sure all information is valid
            {
                //MessageBox(NULL, "Invalid texture information", "ERROR", MB_OK);	// Display Error
                if (fTGA != NULL)													// Check if file is still open
                {
                    fclose(fTGA);													// If so, close it
                }
                return false;														// Return failed
            }

            if (texture->bpp == 24) {												// If the BPP of the image is 24...
                texture->type = GL_RGB;												// Set Image type to GL_RGB
            }
            else {																	// Else if its 32 BPP
                texture->type = GL_RGBA;											// Set image type to GL_RGBA
            }

            tga.bytesPerPixel = (tga.Bpp / 8);									// Compute the number of BYTES per pixel
            tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
            texture->imageData = (GLubyte*)malloc(tga.imageSize);					// Allocate that much memory

            if (texture->imageData == NULL)											// If no space was allocated
            {
                //MessageBox(NULL, "Could not allocate memory for image", "ERROR", MB_OK);	// Display Error
                fclose(fTGA);														// Close the file
                return false;														// Return failed
            }

            fseek(fTGA, 18, SEEK_SET);

            if (fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)	// Attempt to read image data
            {
                //MessageBox(NULL, "Could not read image data", "ERROR", MB_OK);	// Display Error
                if (texture->imageData != NULL)										// If imagedata has data in it
                {
                    free(texture->imageData);										// Delete data from memory
                }
                fclose(fTGA);														// Close file
                return false;														// Return failed
            }

            // Byte Swapping Optimized By Steve Thomas
            for (GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
            {
                texture->imageData[cswap] ^= texture->imageData[cswap + 2] ^=
                    texture->imageData[cswap] ^= texture->imageData[cswap + 2];
            }

            fclose(fTGA);															// Close file
            return true;															// Return success
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::LoadCompressedTGA(TextureNode *pNode, FILE *fTGA)		// Load COMPRESSED TGAs
        {
            STGADesc &tga = pNode->m_TGADesc;
            STextureBitmap *texture = &pNode->m_pSTextureBitmap;

            if (fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)				// Attempt to read header
            {
                //MessageBox(NULL, "Could not read info header", "ERROR", MB_OK);	// Display Error
                if (fTGA != NULL) {													// If file is open
                    fclose(fTGA);													// Close it
                }
                return false;														// Return failed
            }

            texture->width = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
            texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
            texture->bpp = tga.header[4];										// Determine Bits Per Pixel
            tga.Width = texture->width;										// Copy width to local structure
            tga.Height = texture->height;										// Copy width to local structure
            tga.Bpp = texture->bpp;											// Copy width to local structure

            if ((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))	//Make sure all texture info is ok
            {
                //MessageBox(NULL, "Invalid texture information", "ERROR", MB_OK);	// If it isnt...Display error
                if (fTGA != NULL) {												// Check if file is open
                    fclose(fTGA);													// Ifit is, close it
                }
                return false;														// Return failed
            }

            if (texture->bpp == 24) {												// If the BPP of the image is 24...
                texture->type = GL_RGB;												// Set Image type to GL_RGB
            }
            else {																	// Else if its 32 BPP
                texture->type = GL_RGBA;											// Set image type to GL_RGBA
            }

            tga.bytesPerPixel = (tga.Bpp / 8);									// Compute BYTES per pixel
            tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
            texture->imageData = (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

            if (texture->imageData == NULL)											// If it wasnt allocated correctly..
            {
                //MessageBox(NULL, "Could not allocate memory for image", "ERROR", MB_OK);	// Display Error
                fclose(fTGA);														// Close file
                return false;														// Return failed
            }

            GLuint pixelcount = tga.Height * tga.Width;							// Nuber of pixels in the image
            GLuint currentpixel = 0;												// Current pixel being read
            GLuint currentbyte = 0;												// Current byte 
            GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

            do
            {
                GLubyte chunkheader = 0;											// Storage for "chunk" header

                if (fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)				// Read in the 1 byte header
                {
                    //MessageBox(NULL, "Could not read RLE header", "ERROR", MB_OK);	// Display Error
                    if (fTGA != NULL)												// If file is open
                    {
                        fclose(fTGA);												// Close file
                    }
                    if (texture->imageData != NULL)									// If there is stored image data
                    {
                        free(texture->imageData);									// Delete image data
                    }
                    return false;													// Return failed
                }

                if (chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
                {																	// that follow the header
                    chunkheader++;													// add 1 to get number of following color values
                    for (short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
                    {
                        if (fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
                        {
                            //MessageBox(NULL, "Could not read image data", "ERROR", MB_OK);		// IF we cant, display an error

                            if (fTGA != NULL) {													// See if file is open
                                fclose(fTGA);													// If so, close file
                            }

                            if (colorbuffer != NULL) {											// See if colorbuffer has data in it
                                free(colorbuffer);												// If so, delete it
                            }

                            if (texture->imageData != NULL) {									// See if there is stored Image data
                                free(texture->imageData);										// If so, delete it too
                            }

                            return false;														// Return failed
                        }
                        // write to memory
                        texture->imageData[currentbyte] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
                        texture->imageData[currentbyte + 1] = colorbuffer[1];
                        texture->imageData[currentbyte + 2] = colorbuffer[0];

                        if (tga.bytesPerPixel == 4)												// if its a 32 bpp image
                        {
                            texture->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
                        }

                        currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
                        currentpixel++;															// Increase current pixel by 1

                        if (currentpixel > pixelcount)											// Make sure we havent read too many pixels
                        {
                            //MessageBox(NULL, "Too many pixels read", "ERROR", NULL);			// if there is too many... Display an error!

                            if (fTGA != NULL) {													// If there is a file open
                                fclose(fTGA);													// Close file
                            }

                            if (colorbuffer != NULL) {											// If there is data in colorbuffer
                                free(colorbuffer);												// Delete it
                            }

                            if (texture->imageData != NULL) {										// If there is Image data
                                free(texture->imageData);										// delete it
                            }
                            return false;														// Return failed
                        }
                    }
                }
                else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
                {
                    chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
                    if (fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
                    {
                        //MessageBox(NULL, "Could not read from file", "ERROR", MB_OK);			// If attempt fails.. Display error (again)

                        if (fTGA != NULL) {														// If thereis a file open
                            fclose(fTGA);														// Close it
                        }
                        if (colorbuffer != NULL) {												// If there is data in the colorbuffer
                            free(colorbuffer);													// delete it
                        }
                        if (texture->imageData != NULL) {											// If thereis image data
                            free(texture->imageData);											// delete it
                        }
                        return false;															// return failed
                    }

                    for (short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
                    {																			// by the header
                        texture->imageData[currentbyte] = colorbuffer[2];					// switch R and B bytes areound while copying
                        texture->imageData[currentbyte + 1] = colorbuffer[1];
                        texture->imageData[currentbyte + 2] = colorbuffer[0];

                        if (tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
                        {
                            texture->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
                        }

                        currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
                        currentpixel++;															// Increase pixel count by 1

                        if (currentpixel > pixelcount)											// Make sure we havent written too many pixels
                        {
                            //MessageBox(NULL, "Too many pixels read", "ERROR", NULL);			// if there is too many... Display an error!

                            if (fTGA != NULL) {													// If there is a file open
                                fclose(fTGA);													// Close file
                            }
                            if (colorbuffer != NULL) {												// If there is data in colorbuffer
                                free(colorbuffer);												// Delete it
                            }
                            if (texture->imageData != NULL) {									// If there is Image data
                                free(texture->imageData);										// delete it
                            }
                            return false;														// Return failed
                        }
                    }
                }
            } while (currentpixel < pixelcount);													// Loop while there are still pixels left

            fclose(fTGA);																		// Close the file

            return true;																		// return success
        }

        //----------------------------------------------------------------------------------------------
        bool SRenderContext::LoadFile(const wchar_t *fileName, bool binary, uint8_t **buffer, uint32_t *size)
        {
            bool bResult = false;

            if (fileName && buffer && size)
            {
                FILE *input = nullptr;
                uint32_t fileSize, readed;

                const wchar_t mode[] = { L'r', binary ? L'b' : L't', L'\0' };

                if ((input = _wfopen(fileName, mode)) != NULL)
                {
                    fseek(input, 0, SEEK_END);

                    fileSize = (uint32_t)ftell(input);

                    rewind(input);

                    if (fileSize > 0)
                    {
                        *buffer = new uint8_t[fileSize + 1]; // alloc filesize + 1 bytes

                        assert(*buffer);

                        memset(*buffer, '\0', sizeof(uint8_t) * (fileSize + 1));

                        readed = fread(*buffer, sizeof(uint8_t), fileSize, input);

                        if (readed == fileSize)
                        {
                            fclose(input);

                            *size = fileSize;

                            bResult = true;
                        }
                        else
                        {
                            //LOG_ERROR("Reading file '%s'\n", fileName);
                            fclose(input);

                            delete[] * buffer;
                        }
                    }
                    else
                    {
                        //LOG_ERROR("Empty file '%s'\n", fileName);
                        fclose(input);
                    }
                }
            }

            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        GLint SRenderContext::ShaderStatus(GLuint shader, GLenum param)
        {
            assert(shader);

            GLint status, length;
            GLchar buffer[1024];

            glGetShaderiv(shader, param, &status);

            if (status != GL_TRUE)
            {
                glGetShaderInfoLog(shader, 1024, &length, buffer);
                //MessageBox(NULL, buffer, "ShaderStatus Error", MB_OK|MB_ICONEXCLAMATION);
            }

            return status;
        }
    }
}