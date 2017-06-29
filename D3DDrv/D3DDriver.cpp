#include "d3ddriverstdafx.h"

#ifdef _WIN32
#pragma warning(disable: 4333)
#endif

#define PI_CONST		3.14159265358979323846f

#undef D3DX_API

namespace oes
{
    namespace d3d
    {
        struct DeleteVectorFntor
        {
	        // Overloaded () operator.
	        // This will be called by for_each() function.
	        template<typename T>
	        void operator()(const T* x) const
	        {
		        // Delete pointer.
		        delete x;
	        }
        };

        //----------------------------------------------------------------------------------------------
        D3DDriver::D3DDriver()
        : m_bValidDevice(FALSE)
        , m_bWindowed(true)
        , m_fBlendOpacity(1.f)
        , bForceSoftware(false)
        , ActiveRenderTarget(NULL)
        , m_bParalell(true)
        , m_eventCPS(0)
        , m_updateCPS(0)
        , m_renderCPS(0)
        , m_timerCPS(0)
        , m_streamCPS(0)
        , fAppPassTime(0.f)
        , fDeltaTime(0.f)
        , m_bFog(false)
        , m_fFogMin(0.f)
        , m_fFogMax(1.f)
        , m_fFogDensity(0.1f)
        , m_FogColor(0)
        , m_pDefaultRenderContext(new SRenderContext(this))
        {
	        InitializeCriticalSection(&CommandsCS);
        }

        //----------------------------------------------------------------------------------------------
        D3DDriver::~D3DDriver()
        { 
	        delete m_pDefaultRenderContext;

 	        DeleteCriticalSection(&CommandsCS);
        }
 
        //----------------------------------------------------------------------------------------------
        void D3DDriver::ReleaseDriver(void)
        {
	        TMapTextureNode::iterator Iter = m_MapTextureNodes.begin();
	        while (Iter != m_MapTextureNodes.end())
	        {
		        //Iter->second->Release(true);
		        //delete Iter->second;
		        ++Iter;
	        }

	        TMapVolumeTextureNode::iterator IterVolume = m_MapVolumeTextureNodes.begin();
	        while (IterVolume != m_MapVolumeTextureNodes.end())
	        {
		        //IterVolume->second->Release(true);
		        //delete IterVolume->second;
		        ++IterVolume;
	        }

	        TVecFontNodes::iterator IterFont = m_VecFontNodes.begin();
	        while (IterFont != m_VecFontNodes.end())
	        {
		        //IterFont->second->Release(true);
		        ++IterFont;
	        }

	        TSubMeshNodes::iterator IterMesh = SubMeshNodes.begin();
	        while (IterMesh != SubMeshNodes.end())
	        {
		        //IterMesh->second->Release(true);
		        //delete IterMesh->second;
		        ++IterMesh;
	        }

            assert(m_stackContext.empty()); // ensure no remain active contexts

	        delete this;
        }

        //----------------------------------------------------------------------------------------------
        bool D3DDriver::ResizeWindow(unsigned int width, unsigned int height, SRenderContext *pContext /*= 0*/)
        {
	        SRenderContext *pResizeContext = (pContext == 0) ? m_pDefaultRenderContext : pContext;

	        pResizeContext->m_displayModeWidth = width;
	        pResizeContext->m_displayModeHeight = height;

	        //glViewport(0, 0, width, height);
	        return true;
        }

        //----------------------------------------------------------------------------------------------
        bool D3DDriver::GetWireframeMode() const
        {
	        GLint nVal[2];
	        glGetIntegerv(GL_POLYGON_MODE, nVal);

 	        return nVal[0] == GL_LINE;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetWireframeMode(bool flag)
        {
	        glPolygonMode(GL_FRONT_AND_BACK, flag ? GL_LINE : GL_FILL);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetRenderContext(SRenderContext *pContext)
        {
	        if (pContext)
	        {
		        wglMakeCurrent(pContext->m_hDC, pContext->m_hRC);

		        glViewport(0, 0, pContext->m_displayModeWidth, pContext->m_displayModeHeight);	
	        }
	        else 
	        {
		        wglMakeCurrent(0, 0);
	        }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::PushContext(SRenderContext *pContext)
        {
	        assert(pContext);

	        wglMakeCurrent(pContext->m_hDC, pContext->m_hRC);

	        glViewport(0, 0, pContext->m_displayModeWidth, pContext->m_displayModeHeight);

            SetWireframeMode(pContext->m_bWireframe);

	        m_stackContext.push(pContext);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::PopContext()
        {
	        assert(m_stackContext.size() > 0);

	        m_stackContext.pop();

	        if (m_stackContext.size() > 0)
	        {
		        SRenderContext *pCtxtTop = m_stackContext.top();

		        wglMakeCurrent(pCtxtTop->m_hDC, pCtxtTop->m_hRC);

		        glViewport(0, 0, pCtxtTop->m_displayModeWidth, pCtxtTop->m_displayModeHeight);	
	        }
	        else
	        {
		        wglMakeCurrent(0, 0);
	        }
        }

        //----------------------------------------------------------------------------------------------
        SRenderContext* D3DDriver::GetCurrentContext() const
        {
	        SRenderContext* pOutContext = nullptr;

	        if (m_stackContext.size() > 0)
	        {
		        pOutContext = m_stackContext.top();
	        }

	        return pOutContext;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::UpdateDriverThread()
        {
        // 	TMapTextureNode::iterator Iter = MapTextureNodes.begin();
        // 	while (Iter != MapTextureNodes.end())
        // 	{
        // 		if (Iter->second->IsClear())
        // 		{
        // 			delete Iter->second;
        // 			Iter = MapTextureNodes.erase(Iter);
        // 			continue;
        // 		}
        // 		++Iter;
        // 	}

	        TMapVolumeTextureNode::iterator IterVolume = m_MapVolumeTextureNodes.begin();
	        while (IterVolume != m_MapVolumeTextureNodes.end())
	        {
		        if (IterVolume->second->IsClear())
		        {
			        delete IterVolume->second;
			        IterVolume = m_MapVolumeTextureNodes.erase(IterVolume);
			        continue;
		        }
		        ++IterVolume;
	        }

	        TMapMaterialEffects::const_iterator IterMat = m_MapMaterialEffects.begin();
	        while (IterMat != m_MapMaterialEffects.end())
	        {
		        if (IterMat->second->IsClear())
		        {
			        delete IterMat->second;
			        IterMat = m_MapMaterialEffects.erase(IterMat);
			        continue;
		        }
		        ++IterMat;
	        }

	        TSkeletonNodes::iterator IterSkel = SkeletonNodes.begin();
	        while (IterSkel != SkeletonNodes.end())
	        {
		        if (IterSkel->second->IsClear())
		        {
			        delete IterSkel->second;
			        IterSkel = SkeletonNodes.erase(IterSkel);
			        continue;
		        }
		        ++IterSkel;
	        }
        }

        //----------------------------------------------------------------------------------------------
        BOOL D3DDriver::InitRenderDriver(void *canvas, unsigned int width, unsigned int height, SRenderContext *pContext /*= nullprt*/)
        { 
	        HRESULT hr = 0;

	        SRenderContext *pFillContext = (pContext != 0) ? pContext : m_pDefaultRenderContext;

	        pFillContext->m_hWnd = (HWND)canvas;
            pFillContext->m_displayModeWidth = width;
            pFillContext->m_displayModeHeight = height;

	        const int bits = 16;

	        static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	        {
		        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		        1,											// Version Number
		        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		        PFD_TYPE_RGBA,								// Request An RGBA Format
		        bits,										// Select Our Color Depth
		        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		        0,											// No Alpha Buffer
		        0,											// Shift Bit Ignored
		        0,											// No Accumulation Buffer
		        0, 0, 0, 0,									// Accumulation Bits Ignored
		        16,											// 16Bit Z-Buffer (Depth Buffer)  
		        0,											// No Stencil Buffer
		        0,											// No Auxiliary Buffer
		        PFD_MAIN_PLANE,								// Main Drawing Layer
		        0,											// Reserved
		        0, 0, 0										// Layer Masks Ignored
	        };

	        if (!(pFillContext->m_hDC = GetDC(pFillContext->m_hWnd)))						// Did We Get A Device Context?
	        {
		        //MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		        return false;
	        }

	        if (!(pFillContext->m_PixelFormat = ChoosePixelFormat(pFillContext->m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	        {
		        //MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		        return false;
	        }

	        if (!SetPixelFormat(pFillContext->m_hDC, pFillContext->m_PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	        {
		        //MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		        return false;
	        }

	        if (!(pFillContext->m_hRC = wglCreateContext(pFillContext->m_hDC)))				// Are We Able To Get A Rendering Context?
	        {
		        //MessageBox(NULL, "Can't Create A GL Rendering Context.","ERROR", MB_OK | MB_ICONEXCLAMATION);
		        return false;
	        }
	
	        // default context

	        if (!wglMakeCurrent(pFillContext->m_hDC, pFillContext->m_hRC))					// Try To Activate The Rendering Context
	        {
		        //MessageBox(NULL, "Can't Activate The GL Rendering Context.","ERROR", MB_OK | MB_ICONEXCLAMATION);
		        return false;
	        }

	        //
	        // EXT_framebuffer_object
	        //

	        char *ext = (char*)glGetString( GL_EXTENSIONS );

	        if (strstr(ext, "EXT_framebuffer_object") == nullptr)
	        {
		        //MessageBox(NULL,"EXT_framebuffer_object extension was not found",
		        //	"ERROR",MB_OK|MB_ICONEXCLAMATION);
		        return false;
	        }
	        else
	        {
		        glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		        glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		        glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		        glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		        glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		        glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		        glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		        glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		        glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		        glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		        glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		        glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		        glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		        glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		        glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		        glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		        glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		        if( !glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT || 
			        !glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT || 
			        !glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT || 
			        !glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT || 
			        !glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT||  
			        !glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT )
		        {
			        //MessageBox(NULL,"One or more EXT_framebuffer_object functions were not found",
			        //	"ERROR",MB_OK|MB_ICONEXCLAMATION);
			        return false;
		        }
	        }

	        //ShowWindow(hWnd,SW_SHOW);
	        //SetForegroundWindow(hWnd);						// Slightly Higher Priority
	        //SetFocus(hWnd);	

	        glViewport(0, 0, pFillContext->m_displayModeWidth, pFillContext->m_displayModeHeight);
	        glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	        glClearDepth(1.0f);
	        glEnable(GL_DEPTH_TEST);
	        glEnable(GL_CULL_FACE);

        /*	
	        glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	        glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
        */
	        glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	        if (pContext == 0)
	        {
		        PushContext(m_pDefaultRenderContext);

		        GLenum Result = glewInit();
		        InitDefaultResources();

		        PopContext();
	        }
	        return true;
        }

        //----------------------------------------------------------------------------------------------
        bool D3DDriver::InitDefaultResources()
        {
	        m_pTextureDef = LoadTextureW(L"../data/$cashe/Textures/dev_common_64_64.tga");

	        m_pDefaultDiffuse[EEF_PlainDbg]		 = LoadMaterialEffect("../data/$system/ogl_plain_dbg.vs");
	        m_pDefaultDiffuse[EEF_PlainColorDbg] = LoadMaterialEffect("../data/$system/ogl_plain_color_dbg.vs");

	        return true;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::ProcessCommands()
        {
	        EnterCriticalSection(&CommandsCS);

	        for_each(ListCommands.begin(), ListCommands.end(), std::mem_fun(&oes::d3d::CommandBaseRI::Execute));
	        for_each(ListCommands.begin(), ListCommands.end(), DeleteVectorFntor());
	        ListCommands.clear();

	        LeaveCriticalSection(&CommandsCS);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddCommandRI(CommandBaseRI *Command)
        {
	        ListCommands.push_back(Command);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddCommandLockRI(CommandBaseRI *Command)
        {
	        EnterCriticalSection(&CommandsCS);
	
	        AddCommandRI(Command);
	
	        LeaveCriticalSection(&CommandsCS);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddCommandsArrayRI(std::vector<CommandBaseRI*> &ArrayCmd)
        {
	        EnterCriticalSection(&CommandsCS);

	        std::vector<CommandBaseRI*>::iterator Iter = ArrayCmd.begin();

	        while(Iter != ArrayCmd.end())
	        {
		        AddCommandRI(*Iter);
		        Iter++;
	        }

	        LeaveCriticalSection(&CommandsCS);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetProjCropMatrix(const float* mat)
        { 
	        //m_crop = *(D3DMATRIX*)mat;
        }

        //----------------------------------------------------------------------------------------------
        float* D3DDriver::GetProjCropMatrix() const
        {
	        return 0;//(float*)&m_crop;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetBlendOpacity(float Opacity)
        {
	        if (m_fBlendOpacity != Opacity){
		        m_fBlendOpacity = Opacity;
	        }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetLight(const float *position,
						         const float *direction,
						         const float *LightView,
						         const float *LightProj,
						         unsigned int color,
						         const float *SplitDistances,
						         const float *pCropMatrixes,
						         int Num)
        {
	        m_vDiffusePoint = *(SVector3f*)position;
	        m_vLightDirection = *(SVector3f*)direction;
	
	        memcpy(m_mLightView, LightView, sizeof(Matrix4f));
	        memcpy(m_mLightProj, LightProj, sizeof(Matrix4f));

	        if (Num >= 3)
	        {
		        memcpy(m_mLightProjCrop[0], pCropMatrixes, sizeof(Matrix4f));
		        memcpy(m_mLightProjCrop[1], pCropMatrixes + 16, sizeof(Matrix4f));
		        memcpy(m_mLightProjCrop[2], pCropMatrixes + 32, sizeof(Matrix4f));
		        //D3DXMatrixMultiply(&MLightProj, &MLightProj, &CropMatrix);
	        }

	        m_vFrustrumSplitDistances = *(SVector4f*)SplitDistances;
	        m_fLightColor	 = color;
        }

        //----------------------------------------------------------------------------------------------
        float D3DDriver::GetBlendOpacity() const
        {
 	        return m_fBlendOpacity;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetViewport(float x, float y, float ext_x, float ext_y)
        {
	        /*RECT pRect;

 	        pRect.left   = (LONG)x;
 	        pRect.top    = (LONG)y;
 	        pRect.right  = (LONG)(x + ext_x);
 	        pRect.bottom = (LONG)(y + ext_y);

	        GetDevice()->SetScissorRect(&pRect);*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::ClearBackBuffer()
        {
        }

        //----------------------------------------------------------------------------------------------
        /*void OrthoProj(D3DMATRIX &vOut, float l, float r, float b, float t, float zn, float zf)
        {
	        // 2/(r-l)      0            0           0
	        // 0            2/(t-b)      0           0
	        // 0            0            1/(zn-zf)   0
	        // (l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l

	        vOut._11  = 2.f/(r-l);
	        vOut._22  = 2.f/(t-b);
	        vOut._33 = 1.f/(zn-zf);	
	        vOut._41 = (l+r)/(l-r);	
	        vOut._42 = (t+b)/(b-t);	
	        vOut._43 = zn/(zn-zf);
        }*/

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DriverBeginDraw()
        {
	        glClearColor(0.20f, 0.20f, 0.20f, 1.0f);
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DriverEndDraw()
        {
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::Present(const SRenderContext *pContext /*= 0*/)
        {
	        SwapBuffers((pContext == 0) ? m_pDefaultRenderContext->m_hDC : pContext->m_hDC);	// Swap Buffers (Double Buffering)
        }

        //----------------------------------------------------------------------------------------------
        BOOL D3DDriver::DrawRectangle2D(Texture2D* texture, const float *position1, const float* position2, DWORD Color, float U /*= 0.f*/, float V /*= 0.f*/, float UL /*= 0.f*/, float VL/* = 0.f*/)
        {
            BOOL bResult = FALSE;
   	        return bResult;
        }

        //----------------------------------------------------------------------------------------------
        BOOL D3DDriver::DrawTriangle2D(const float* position0,
				                       const float* position1,
							           const float* position2,
							           DWORD Color /*= 0xffffffff*/,
							           bool RHW /*= false*/)
        {
	        return TRUE;
        }

        //----------------------------------------------------------------------------------------------
        BOOL D3DDriver::DrawTriangle2D(Texture2D* texture,
							           const float* position0,
							           const float* position1,
							           const float* position2,
							           const float* UV0,
							           const float* UV1,
							           const float* UV2,
							           DWORD Color /*= 0xffffffff*/,
							           bool RHW /*= false*/)
        {
	        return TRUE;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddLine(const float *pStart, const float* pEnd, unsigned int color, bool bZEnable/* = true*/)
        {
        //	if (bRenderShadowmap || ActiveRenderTarget){
        //		return;
        //	}
	        byte a = (byte)((color & 0xFF000000) >> 24);
	        byte r = (byte)((color & 0x00FF0000) >> 16);
	        byte g = (byte)((color & 0x0000FF00) >> 8);
	        byte b = (byte)(color & 0x000000FF);

	        D3DVertexDbg P;	
	        P.m_Position[0] = *pStart;
	        P.m_Position[1] = *(pStart+1);
	        P.m_Position[2] = *(pStart+2);

	        P.m_Color[0] = r / 255.f;
	        P.m_Color[1] = g / 255.f;
	        P.m_Color[2] = b / 255.f;

	        if (bZEnable){
		        m_3DPointList.push_back(P);
	        }else{
		        m_PointListNoZ.push_back(P);
	        }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddDot(const float *point, unsigned int color, bool bZEnable /*= true*/)
        {
	        //if (bRenderShadowmap || ActiveRenderTarget){
	        //	return;
	        //}

	        byte a = (byte)((color & 0xFF000000) >> 24);
	        byte r = (byte)((color & 0x00FF0000) >> 16);
	        byte g = (byte)((color & 0x0000FF00) >> 8);
	        byte b = (byte)(color & 0x000000FF);

	        D3DVertexDbg P;	
	        P.m_Position[0] = *point; 
	        P.m_Position[1] = *(point + 1);
	        P.m_Position[2] = *(point + 2);

	        P.m_Color[0] = r / 255.f;
	        P.m_Color[1] = g / 255.f;
	        P.m_Color[2] = b / 255.f;

	        if (bZEnable){
		        m_3DDotsList.push_back(P);
	        }else{
		        m_3DDotsListNoZ.push_back(P);
	        }
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddTriangle2D(const float *p0, const float *p1, const float *p2, unsigned int Color)
        {
        /*	if (bRenderShadowmap || ActiveRenderTarget){
		        return;
	        }

	        byte a = (byte)((Color & 0xFF000000) >> 24);
	        byte r = (byte)((Color & 0x00FF0000) >> 16);
	        byte g = (byte)((Color & 0x0000FF00) >> 8);
	        byte b = (byte)(Color & 0x000000FF);

	        D3DXVECTOR3 A(*p0, *(p0+1), 0.f);
	        D3DXVECTOR3 B(*p1, *(p1+1), 0.f);
	        D3DXVECTOR3 C(*p2, *(p2+1), 0.f);

	        D3DXMATRIX worldViewProjection = m_view * m_proj;
	        D3DXVec3TransformCoord(&A, &A, &worldViewProjection);
	        D3DXVec3TransformCoord(&B, &B, &worldViewProjection);
	        D3DXVec3TransformCoord(&C, &C, &worldViewProjection);

	        D3DVertexDbg P0, P1, P2;

	        P0.m_Position[0] = A.x;
	        P0.m_Position[1] = A.y;
	        P0.m_Position[2] = A.z;

	        P1.m_Position[0] = B.x;
	        P1.m_Position[1] = B.y;
	        P1.m_Position[2] = B.z;

	        P2.m_Position[0] = C.x;
	        P2.m_Position[1] = C.y;
	        P2.m_Position[2] = C.z;

	        P0.m_Color[0] = P1.m_Color[0] = P2.m_Color[0] = r / 255.f;
	        P0.m_Color[1] = P1.m_Color[1] = P2.m_Color[1] = g / 255.f;
	        P0.m_Color[2] = P1.m_Color[2] = P2.m_Color[2] = b / 255.f;

	        m_PointsTriangle.push_back(P0);
	        m_PointsTriangle.push_back(P1);
	        m_PointsTriangle.push_back(P2);*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddTriangle(const float *p0, const float *p1, const float *p2, unsigned int Color, float tu/*=1.f*/, float tv/*=1.f*/, BOOL bZenable/* = TRUE*/)
        {
	        //if (bRenderShadowmap || ActiveRenderTarget){
	        //	return;
	        //}

	        byte a = (byte)((Color & 0xFF000000) >> 24);
	        byte r = (byte)((Color & 0x00FF0000) >> 16);
	        byte g = (byte)((Color & 0x0000FF00) >> 8);
	        byte b = (byte)(Color & 0x000000FF);

	        D3DVertexDbg P0, P1, P2;

	        P0.m_Position[0] = *p0;
	        P0.m_Position[1] = *(p0+1);
	        P0.m_Position[2] = *(p0+2);

	        P1.m_Position[0] = *p1;
	        P1.m_Position[1] = *(p1+1);
	        P1.m_Position[2] = *(p1+2);

	        P2.m_Position[0] = *p2;
	        P2.m_Position[1] = *(p2+1);
	        P2.m_Position[2] = *(p2+2);

	        P0.m_Color[0] = P1.m_Color[0] = P2.m_Color[0] = r / 255.f;
	        P0.m_Color[1] = P1.m_Color[1] = P2.m_Color[1] = g / 255.f;
	        P0.m_Color[2] = P1.m_Color[2] = P2.m_Color[2] = b / 255.f;

	        m_PointsTriangle.push_back(P0);
	        m_PointsTriangle.push_back(P2);
	        m_PointsTriangle.push_back(P1);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::AddSphere(const float *pos, const float rad, const unsigned int color, const unsigned int segments /*= 16*/)
        {

        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::_DrawText2D(const wchar_t *text, float x, float y, DWORD color, bool shadowed)
        {
        /*	RECT rc;

	        if (text != NULL)
	        {	
		        size_t Size = wcslen(text);

		        m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		        m_pTextSprite->SetTransform(&m_world);

		        //if (shadowed)
		        {
			        SetRect( &rc, 1, 1, 0, 0 );
			        m_CanvasFont->DrawTextW(m_pTextSprite, text, -1, &rc, DT_NOCLIP, 0xff000000);
		        }

		        SetRect( &rc, 0, 0, 0, 0 );
		        m_CanvasFont->DrawTextW(m_pTextSprite, text, -1, &rc, DT_NOCLIP, color);
		        m_pTextSprite->End();
	        }*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::_DrawText2D(const wchar_t *text, float x, float y, float ext_x, float ext_y, DWORD color, bool shadowed)
        {
        /*	RECT rc;

	        if (text != NULL)
	        {	
		        size_t Size = wcslen(text);
		
		        m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		        m_pTextSprite->SetTransform(&m_world);

		        //if (shadowed)
		        {
			        SetRect( &rc, 1, 1, 100, 100 );
			        m_CanvasFont->DrawTextW(m_pTextSprite, text, -1, &rc, DT_NOCLIP, 0xff000000);
		        }
		
		        SetRect( &rc, 0, 0, 100, 100 );
		        m_CanvasFont->DrawTextW(m_pTextSprite, text, -1, &rc, DT_NOCLIP, color);
		        m_pTextSprite->End();
	        }*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::RenderCircle(const float* pWTM, float rad, unsigned int color, unsigned int segments /*=16*/)
        {
        /*	D3DXMATRIXA16 DX_WTM = *(D3DMATRIX*)pWTM;

	        float d_a = (2.f * D3DX_PI) / segments;

	        D3DXVECTOR3 vStart(rad, 0.f, 0.f);
	        D3DXVec3TransformCoord(&vStart, &vStart, &DX_WTM);

	        for (size_t index = 0; index < segments; index++ )
	        {
		        float ang = d_a * index;

		        D3DXVECTOR3 vEnd(cosf(ang) * rad, sinf(ang) * rad, 0.f);
		        D3DXVec3TransformCoord(&vEnd, &vEnd, &DX_WTM);

		        //AddLine((float*)vStart, (float*)vEnd, color);
		        vStart = vEnd;
	        }
	        D3DXVECTOR3 vEndl(rad, 0.f, 0.f);
	        D3DXVec3TransformCoord(&vEndl, &vEndl, &DX_WTM);
	        //AddLine((float*)vStart, (float*)vEndl, color);*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::RenderDebugSphere(const float *pos, const float rad, const unsigned int color, const unsigned short segments /*= 16*/)
        {
	        // half slices: n - 1
	        // total slices: 2 (n - 1) + 1 -> (2n - 1)

	        unsigned short nSegments = segments;
	
	        if (nSegments < 3){
		        nSegments = 3;
	        }
	        else if (nSegments > 128){
		        nSegments = 128;
	        }

	        const unsigned int nTotalSlices = 2 * nSegments - 1;
	        const unsigned int NumQuads = (unsigned int) nSegments * (nTotalSlices - 1);

	        D3DVertexDbg * pVertices = new D3DVertexDbg[NumQuads * 6];	// array of vertices

	        const float DeltaTh = PI_CONST / (nTotalSlices - 1);
	        const float DeltaAl = (2.f * PI_CONST) / nSegments;

	        float b = ((BYTE)(((DWORD_PTR)(color)) & 0xff)) / 255.f;
	        float g = ((BYTE)(((DWORD)(color) >> 8) & 0xff)) / 255.f;
	        float r = ((BYTE)((color >> 16) & 0xff)) / 255.f;
	        float a = ((BYTE)((color >> 24) & 0xff)) / 255.f;
	
	        for (size_t Index1 = 0; Index1 < nTotalSlices - 1; ++Index1) // vertical
	        {
		        for (size_t Index2 = 0; Index2 < nSegments; ++Index2) // horizontal
		        {
			        float th0 = Index1 * DeltaTh; // vert
			        float al0 = Index2 * DeltaAl; // horizontal

			        float th1 = (Index1 + 1) * DeltaTh; // vert
			        float al1 = (Index2 + 1) * DeltaAl; // horizontal

			        unsigned int CurQuad = (Index1 * nSegments) + Index2;

			        const float sin_th0 = sinf(th0);
			        const float cos_th0 = cosf(th0);
			        const float sin_th1 = sinf(th1);
			        const float cos_th1 = cosf(th1);

			        const float sin_al0 = sinf(al0);
			        const float cos_al0 = cosf(al0);
			        const float sin_al1 = sinf(al1);
			        const float cos_al1 = cosf(al1);

			        float P0[3] = { (sin(th0) * sinf(al0)) * rad, cos_th0 * rad, (sinf(th0) * cosf(al0)) * rad };
			        float P1[3] = { (sin(th1) * sinf(al0)) * rad, cos_th1 * rad, (sinf(th1) * cosf(al0)) * rad };
			        float P2[3] = { (sin(th1) * sinf(al1)) * rad, cos_th1 * rad, (sinf(th1) * cosf(al1)) * rad };
			        float P3[3] = { (sin(th0) * sinf(al1)) * rad, cos_th0 * rad, (sinf(th0) * cosf(al1)) * rad };
			
			        pVertices[CurQuad * 6 + 0].m_Position[0] = P0[0];
			        pVertices[CurQuad * 6 + 0].m_Position[1] = P0[1];
			        pVertices[CurQuad * 6 + 0].m_Position[2] = P0[2];
			
			        pVertices[CurQuad * 6 + 1].m_Position[0] = P2[0];
			        pVertices[CurQuad * 6 + 1].m_Position[1] = P2[1];
			        pVertices[CurQuad * 6 + 1].m_Position[2] = P2[2];

			        pVertices[CurQuad * 6 + 2].m_Position[0] = P1[0];
			        pVertices[CurQuad * 6 + 2].m_Position[1] = P1[1];
			        pVertices[CurQuad * 6 + 2].m_Position[2] = P1[2];

			        pVertices[CurQuad * 6 + 3].m_Position[0] = P2[0];
			        pVertices[CurQuad * 6 + 3].m_Position[1] = P2[1];
			        pVertices[CurQuad * 6 + 3].m_Position[2] = P2[2];

			        pVertices[CurQuad * 6 + 4].m_Position[0] = P0[0];
			        pVertices[CurQuad * 6 + 4].m_Position[1] = P0[1];
			        pVertices[CurQuad * 6 + 4].m_Position[2] = P0[2];

			        pVertices[CurQuad * 6 + 5].m_Position[0] = P3[0];
			        pVertices[CurQuad * 6 + 5].m_Position[1] = P3[1];
			        pVertices[CurQuad * 6 + 5].m_Position[2] = P3[2];
			
			        pVertices[CurQuad * 6 + 0].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 0].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 0].m_Color[2] = b;

			        pVertices[CurQuad * 6 + 1].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 1].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 1].m_Color[2] = b;

			        pVertices[CurQuad * 6 + 2].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 2].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 2].m_Color[2] = b;

			        pVertices[CurQuad * 6 + 3].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 3].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 3].m_Color[2] = b;

			        pVertices[CurQuad * 6 + 4].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 4].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 4].m_Color[2] = b;

			        pVertices[CurQuad * 6 + 5].m_Color[0] = r;
			        pVertices[CurQuad * 6 + 5].m_Color[1] = g;
			        pVertices[CurQuad * 6 + 5].m_Color[2] = b;

			        ++CurQuad;
		        }
	        }
	
	        glMatrixMode(GL_PROJECTION);
	        glLoadMatrixf(GetProjMatrix());

	        glMatrixMode(GL_MODELVIEW);
	        glLoadMatrixf(GetViewMatrix());
	
	        //glDisable(GL_DEPTH_TEST);

	        glEnableClientState(GL_VERTEX_ARRAY);
	        glVertexPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), pVertices[0].m_Position);

	        glEnableClientState(GL_COLOR_ARRAY);
	        glColorPointer(3, GL_FLOAT, sizeof(D3DVertexDbg), pVertices[0].m_Color);
	
	        glDrawArrays(GL_TRIANGLES, 0, NumQuads * 6);

	        glDisableClientState(GL_COLOR_ARRAY);
	        glDisableClientState(GL_VERTEX_ARRAY);

	        //glEnable(GL_DEPTH_TEST);

	        delete [] pVertices;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::UnprojectScreenToWorld( float * v_out, 
									            const float* mat_view,
										        const float* mat_proj,
                                                const float* screen_pos)
        {
        /*  D3DXVECTOR3 vOut;
            D3DVIEWPORT9 viewPort;

            viewPort.X = viewPort.Y = 0;
            viewPort.Width  = m_displayModeWidth;
            viewPort.Height = m_displayModeHeight;
            viewPort.MinZ = 0;
            viewPort.MaxZ = 1;

            D3DXMATRIX mat;

            D3DXMatrixIdentity(&mat);

            D3DXVECTOR3 vectPickRay1,
                        vectPickRay2;

            D3DXMATRIX  DX_view = m_view;
            D3DXMATRIX  DX_proj = m_proj;

            D3DXVec3Unproject( 
                &vectPickRay1, 
                &D3DXVECTOR3(screen_pos[0] * m_displayModeWidth, screen_pos[1] * m_displayModeHeight, 0.f), 
                &viewPort, 
                &DX_proj, 
                &DX_view, 
                &mat );

            D3DXVec3Unproject( 
                &vectPickRay2, 
                &D3DXVECTOR3(screen_pos[0] * m_displayModeWidth, screen_pos[1] * m_displayModeHeight, 1.f), 
                &viewPort, 
                &DX_proj, 
                &DX_view, 
                &mat );

            vOut = vectPickRay2 - vectPickRay1;

            D3DXVec3Normalize(&vOut,&vOut);

            v_out[0] = vOut.x;
            v_out[1] = vOut.y;
            v_out[2] = vOut.z;*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::ProjectWorldToScreen(float *v_out, const float *position)
        {
        /*	D3DXVECTOR3 vOut;
	        D3DVIEWPORT9 viewPort;

	        viewPort.X = viewPort.Y = 0;
	        viewPort.Width  = m_displayModeWidth;
	        viewPort.Height = m_displayModeHeight;
	        viewPort.MinZ = 0.f;
	        viewPort.MaxZ = 1.f;

	        D3DXVECTOR3 DX_vec(*(position), *(position+1),*(position+2));

 	        D3DXMATRIX mat;
 	        D3DXMatrixIdentity(&mat);

	        m_proj._44 = m_view._44 = 1.f;
	        D3DXVec3Project(&vOut, &DX_vec, &viewPort, &m_proj, &m_view, &mat);
	
	        v_out[0] = vOut.x;
	        v_out[1] = vOut.y;*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetRenderTarget(const D3DRenderTarget *rt, bool clear /*= false*/, DWORD color /*= 0x00000000*/)
        {
	        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt->GetFrameBuffer()); // switch to rendering on our FBO
	        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rt->GetTexture(), 0);
	        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rt->GetDepthBuffer());

	        glViewport(0, 0, rt->GetWidth(), rt->GetHeight()); // set The Current Viewport to the fbo size

	        const float alpha = 0.1f;
	        glClearColor(0.0f, 0.0f, 0.0f, alpha);

	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer on the fbo

	        glMatrixMode(GL_MODELVIEW);
	        glLoadIdentity();
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::EndRenderTarget(const D3DRenderTarget *rt)
        {
	        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // switch to rendering on the framebuffer
	        //glColorMask(TRUE, TRUE, TRUE, TRUE);
        }

        //----------------------------------------------------------------------------------------------
        const D3DRenderTarget* D3DDriver::GetActiveRenderTarget() const
        {
	        return ActiveRenderTarget;
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float MinZ /*= 0.f*/, float MaxZ /*= 1.f*/)
        {
	        //D3DVIEWPORT9 Viewport = {x, y, width, height, MinZ, MaxZ};
	        //m_pd3dDevice->SetViewport(&Viewport);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::DebugRenderShadowmap()
        {
        /*	D3DXMATRIX TmpView, TmpProj;

	        D3DXMatrixIdentity(&TmpView);
	        D3DXMatrixIdentity(&TmpProj);

	        OrthoProj(TmpProj, 0.f, (float)GetWidth(), (float)GetHeight(), 0.f, 0.f, 1000.f);

	        D3DXMATRIX WorldViewProjection = TmpView * TmpProj;

	        pEffect->SetTechnique("DrawHardwareShadowMap");

	        pEffect->SetMatrix("worldViewProj", &WorldViewProjection);
	        pEffect->SetTexture("DiffuseMap", pTextureShadowMap);

	        m_pd3dDevice->SetStreamSource(0, pQuardVB, 0, sizeof(D3DVertex));

	        //set index buffer
	        m_pd3dDevice->SetIndices(pQuardIB);

	        //render quad using HardwareShadowMapTechnique
	        UINT uPasses;
	        if (D3D_OK == pEffect->Begin(&uPasses, 0)) // The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will save and restore all state modified by the effect.
	        {  
		        for (UINT uPass = 0; uPass < uPasses; uPass++)
		        {
			        // Set the state for a particular pass in a technique.
			        pEffect->BeginPass(uPass);

			        // Draw it 
			        m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

			        //End Pass
			        pEffect->EndPass();
		        }
		        pEffect->End();
	        }*/
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetBias(float DepthBias, float SlopeDepthBias)
        {
	        //m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&DepthBias);
	        //m_pd3dDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&SlopeDepthBias);
        }

        //----------------------------------------------------------------------------------------------
        void D3DDriver::SetDebugInfoCPS(bool bParallel, int eventCPS, int updateCPS, int renderCPS, int timerCPS, int streamCPS)
        {
	        m_bParalell = bParallel;
	        m_eventCPS = eventCPS;
	        m_updateCPS = updateCPS;
	        m_renderCPS = renderCPS;
	        m_timerCPS = timerCPS;
	        m_streamCPS = streamCPS;
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffectNode* D3DDriver::GetEffectDefault(EEffectDefault effect) const
        {
	        return m_pDefaultDiffuse[effect];
        }

        //----------------------------------------------------------------------------------------------
        unsigned int D3DDriver::GetLodBenefitByBBox(float *min_x, float *min_y, float *min_z,
											        float *max_x, float *max_y, float *max_z)
        {
	        unsigned int nLOD = 0;

	        if (min_x && min_y && min_z && max_x && max_y && max_z)
	        {
		        float dx = *max_x - *min_x;
		        float dy = *max_y - *min_y;
		        float dz = *max_z - *min_z;

		        float fRad = sqrtf(dx * dx + dy * dy + dz * dz) * 0.5f;

		        float *pViewM = GetViewMatrix();
		        float *pWorldM = GetWorldMatrix();
		        float *pViewPos = GetViewPos();

		        float ViewPoint[3] = { pViewM[12], pViewM[13], pViewM[14] };
		        float WorldPoint[3] = { pWorldM[12], pWorldM[13], pWorldM[14] };

		        // distance to viewer
		        float c[3] = { pWorldM[12] - pViewPos[0], pWorldM[13] - pViewPos[1], pWorldM[14] - pViewPos[2] };
		        float dir[3] = { pViewM[2], pViewM[6], pViewM[10] };

		        float norm = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);

		        if (norm > 0.000001f) {
			        norm = 1.f / norm;
		        }else{
			        norm = 0.f;
		        }

		        dir[0] *= norm;
		        dir[1] *= norm;
		        dir[2] *= norm;

		        // get projection c to dir
		        float fDst = dir[0] * c[0] + dir[1] * c[1] + dir[2] * c[2];

		        float fNear = GetNearPlane();
		        float fProjRad = (fNear * fRad) / fDst;
		        float fArea = PI_CONST * (fProjRad * fProjRad);

		        float fScrRadW = 2.f * tan(30.f * PI_CONST / 180) * fNear; // TODO ADD FOW
		        float fScrRadH = (fScrRadW / 1.3333f); // TODO ADD ASPECT
		        float fScrArea = fScrRadW * fScrRadH;

		        float fMagnitude[] = { fScrArea * 0.01f, fScrArea * 0.005f, fScrArea * 0.0005f, fScrArea * 0.0001f };

		        if (fArea <= fMagnitude[0] && fArea > fMagnitude[1]){
			        nLOD = 1;
		        }
		        else if (fArea <= fMagnitude[1] && fArea > fMagnitude[2]){
			        nLOD = 2;
		        }
		        else if (fArea <= fMagnitude[2] && fArea > fMagnitude[3]){
			        nLOD = 3;
		        }
		        else if (fArea <= fMagnitude[3]){
			        nLOD = 4;
		        }
	        }
	        return nLOD;
        }

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        D3DMesh * AllocD3DMesh(const D3DDriver * Interface)
        {
	        return new D3DMesh(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void ReleaseD3DMesh(D3DMesh* Ptr)
        {
	        delete Ptr;
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffect_ButtonStates * Alloc_MaterialEffect_ButtonStates(const D3DDriver * Interface)
        {
	        return new MaterialEffect_ButtonStates(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_MaterialEffect_ButtonStates(MaterialEffect_ButtonStates* Ptr)
        {
	        Ptr->Release();
	        delete Ptr;
        }

        //----------------------------------------------------------------------------------------------
        D3DSkeleton * Alloc_D3DSkeleton(const D3DDriver * Interface)
        {
	        return new D3DSkeleton(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_D3DSkeleton(D3DSkeleton* Ptr)
        {
	        Ptr->Release();
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffect * Alloc_MaterialEffect(const D3DDriver * Interface)
        {
	        return new MaterialEffect(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_MaterialEffect(MaterialEffect *Ptr)
        {
	        Ptr->Release();
	        delete Ptr;
        }
        //----------------------------------------------------------------------------------------------
        MaterialEffectUI * Alloc_MaterialEffectUI(const D3DDriver * Interface)
        {
	        return new MaterialEffectUI(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_MaterialEffectUI(MaterialEffectUI *Ptr)
        {
	        Ptr->Release();
	        delete Ptr;
        }

        //----------------------------------------------------------------------------------------------
        CMaterialEffectFont* Alloc_MaterialEffectFont(const D3DDriver *pInterface)
        {
	        return new CMaterialEffectFont(pInterface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_MaterialEffectFont(CMaterialEffectFont *pPtr)
        {
	        pPtr->Release();
	        delete pPtr;
        }

        //----------------------------------------------------------------------------------------------
        D3DRenderTarget * Alloc_D3DRenderTarget(const D3DDriver * Interface)
        {
	        return new D3DRenderTarget(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_D3DRenderTarget(D3DRenderTarget* Ptr)
        {
	        Ptr->Release();
        }

        //----------------------------------------------------------------------------------------------
        CFont2D* Alloc_Font2D(const D3DDriver * Interface)
        {
	        return new CFont2D(Interface);
        }

        //----------------------------------------------------------------------------------------------
        void Release_Font2D(CFont2D* Ptr)
        {
	        Ptr->Release();
	        delete Ptr;
        }

    }
}