
#pragma once

#ifdef _WIN32
#pragma warning(disable: 4251)
#endif

#include "dxstdafx.h"
#include "RenderContext.h"

#define SOFTWARE_PROCESSSING
#define DEBUG_VS
#define DEBUG_PS

namespace oes
{
    namespace d3d
    {
        class RenderDriver::RenderTargetNode;
        class TextureNode;
        class CFontNode;
        class SRenderContext;
        class MaterialEffectNode;
        class CSceneMeshNode;
        class SkeletonNode;
        class SubMeshAgregator;
        class Texture2D;
        class D3DRenderTarget;
        class D3DMesh;
        class MaterialEffect_ButtonStates;
        class D3DSkeleton;
        class MaterialEffect;
        class MaterialEffectUI;
        class CMaterialEffectFont;
        class CFont2D;
        class SubMeshNode;
        class VolumeTextureNode;

        //----------------------------------------------------------------------------------------------
        // Base command
        //----------------------------------------------------------------------------------------------
        class CommandBaseRI
        {
        public:
            CommandBaseRI()
            {
            }

            virtual ~CommandBaseRI()
            {
            }

            virtual void Execute() = 0;
        };

        typedef std::map<std::string, TextureNode*> TMapTextureNode;
        
        typedef std::map<std::string, VolumeTextureNode*> TMapVolumeTextureNode;
        typedef std::map<std::string, MaterialEffectNode*> TMapMaterialEffects;
        
        typedef std::map<std::string, SubMeshNode*> TSubMeshNodes;
        typedef std::map<std::string, CSceneMeshNode*> TSceneMeshNodes;
        
        typedef std::map<std::string, SubMeshAgregator*> TSubMeshAgregatorNodes;
        typedef std::map<std::string, SkeletonNode*> TSkeletonNodes;
        
        typedef std::vector<CommandBaseRI*>	TVCommandListRI;
        typedef TVCommandListRI::iterator			TVCommandListIterRI;
        typedef TVCommandListRI::const_iterator		TVCommandListConstIterRI;

        class D3DDRIVER_API D3DDriver
        {
        public:
            D3DDriver();
            ~D3DDriver();

            void	ReleaseDriver(void);

            /** update driver thread for streaming etc. */
            void	UpdateDriverThread();

            /** init render context */
            BOOL	InitRenderDriver(void *canvas, unsigned int width, unsigned int height, SRenderContext *pContext = nullptr);
            bool	InitDefaultResources();

            __forceinline D3DDriver*	GetRenderDriver() { return this; }

            void	ProcessCommands();
            size_t	GetNumCommands() const { return ListCommands.size(); }

            void	AddCommandRI(CommandBaseRI *Command);
            void	AddCommandLockRI(CommandBaseRI *Command);
            void	AddCommandsArrayRI(std::vector<CommandBaseRI*> &ArrayCmd);

            /*!
            *	Loaders: render resource primitives
            */
            TextureNode*    LoadTexture(const char *pURL);
            TextureNode*    LoadTextureW(const wchar_t *pURL);
            
            TextureNode*    LoadTextureFromPixels32(
                unsigned int *pixels,
                unsigned int imgWidth,
                unsigned int imgHeight,
                unsigned int texWidth,
                unsigned int texHeight);

            bool    UnregisterTexture(TextureNode *pNode);

            // TODO
            //	VolumeTextureNode*	LoadVolumeTexture(const char *pURL, UINT width, UINT height, UINT depth, D3DFORMAT format = D3DFMT_A8R8G8B8, D3DPOOL pool = D3DPOOL_DEFAULT, bool ImmidiateLoad = true);
            //	bool						UnregisterVolumeTexture(VolumeTextureNode* Node);

            CFontNode*			        LoadFont(const char *Name, int Size);
            bool						UnregisterFont(const CFontNode *pFont);

            MaterialEffectNode*	LoadMaterialEffect(const char *pURL);
            MaterialEffectNode*	LoadMaterialEffectW(const wchar_t *pURL);
            bool				UnregisterMaterialEffect(MaterialEffectNode *Node);

            CSceneMeshNode*		GetSceneMeshNode(const char *pURL = 0);
            CSceneMeshNode*		GetSceneMeshNodeW(const wchar_t *pURL = 0);
            CSceneMeshNode*		AllocSceneMeshNode(const char *pURL = 0);
            CSceneMeshNode*		AllocSceneMeshNodeW(const wchar_t *pURL = 0);
            bool				UnregisterSceneMeshNode(const CSceneMeshNode* pNode);

            SkeletonNode*			GetSkeletonNode(const char *pURL);

            RenderDriver::RenderTargetNode*	CreateRenderTarget(unsigned int width, unsigned int height);
            void	ReleaseRenderTarget(RenderDriver::RenderTargetNode *rt);

            SubMeshAgregator*		GetD3DMeshNode(const char *filename);

        private:
            bool	LoadUncompressedTGA(TextureNode *pOut, const wchar_t *pURL, FILE *file);	// Load an Uncompressed file
            bool	LoadCompressedTGA(TextureNode *pOut, const wchar_t *pURL, FILE *file);	// Load a Compressed file

        public:
            UINT	GetWidth() const { return m_pDefaultRenderContext->m_displayModeWidth; }
            UINT	GetHeight() const { return m_pDefaultRenderContext->m_displayModeHeight; }

            bool	ResizeWindow(unsigned int width, unsigned int height, SRenderContext *pContext = 0);

            bool	GetWireframeMode() const;
            void	SetWireframeMode(bool flag);

            void	SetRenderContext(SRenderContext *pContext);

            void	PushContext(SRenderContext *pContext);
            void	PopContext();

            SRenderContext* GetDefaultContext() const { return m_pDefaultRenderContext; }

            SRenderContext* GetCurrentContext() const;

            void	SetWorldMatrix(const float *mat) { memcpy(m_world, mat, sizeof(Matrix4f)); }
            float*	GetWorldMatrix() const { return (float*)&m_world; }

            void	SetViewMatrix(const float *mat) { memcpy(m_view, mat, sizeof(Matrix4f)); }
            float*	GetViewMatrix() const { return (float*)&m_view; }

            void	SetProjMatrix(const float* mat) { memcpy(m_proj, mat, sizeof(Matrix4f)); }
            float*	GetProjMatrix() const { return (float*)&m_proj; }

            void	SetNearPlane(float fValue) { m_fNearPlane = fValue; }
            float	GetNearPlane() const { return m_fNearPlane; }

            void	SetFarPlane(float fValue) { m_fFarPlane = fValue; }
            float	GetFarPlane() const { return m_fFarPlane; }

            void	SetViewPos(float *pVal) { memcpy(m_fViewPos, pVal, sizeof(float) * 3); }
            float*	GetViewPos() const { return (float*)&m_fViewPos; }

            void				SetObjectFlags(unsigned int *pValue) { m_ObjectFlags = *pValue; }
            const unsigned int*	GetObjectFlags() const { return &m_ObjectFlags; }

            void	SetProjCropMatrix(const float* mat);
            float*	GetProjCropMatrix() const;

            void	SetBlendOpacity(float Opacity);
            float	GetBlendOpacity() const;

            void	SetViewport(float x, float y, float ext_x, float ext_y);

            void	SetLight(const float *position,
                const float *direction,
                const float *LightView,
                const float *LightProj,
                unsigned int color,
                const float *SplitDistances,
                const float *pCropMatrixes,
                int Num);

            void	ClearBackBuffer();
            void	DriverBeginDraw();
            void	DriverEndDraw();

            void	Present(const SRenderContext *pContext = 0);

            BOOL	DrawRectangle2D(Texture2D* texture,
                const float *position1,
                const float* position2,
                DWORD Color = 0xffffffff,
                float U = 0.f,
                float V = 0.f,
                float UL = 1.f,
                float VL = 1.f);

            BOOL	DrawTriangle2D(const float* position0,
                const float* position1,
                const float* position2,
                DWORD Color = 0xffffffff,
                bool RHW = false);

            BOOL	DrawTriangle2D(Texture2D* texture,
                const float* position0,
                const float* position1,
                const float* position2,
                const float* UV0,
                const float* UV1,
                const float* UV2,
                DWORD Color = 0xffffffff,
                bool RHW = false);

            void	DrawDebugInfo();
            void	DrawDebugLines();
            void	DrawDebugTriangles();
            void	DrawFPS();
            void	DrawPerfInfo();

            void	AddDot(const float *point, unsigned int color, bool bZEnable = true);
            void	AddLine(const float *start, const float* end, unsigned int color, bool bZEnable = true);
            void	AddTriangle2D(const float *p0, const float *p2, const float *p3, unsigned int Color);
            void	AddTriangle(const float *p0, const float *p2, const float *p3, unsigned int Color, float tu = 1.f, float tv = 1.f, BOOL bZenable = TRUE);
            void	AddSphere(const float *pos, const float rad, const unsigned int color, const unsigned int segments = 16);

            /** slow functions. */
            void	_DrawText2D(const wchar_t *text, float x, float y, DWORD color = 0xffffffff, bool shadowed = false);
            void	_DrawText2D(const wchar_t *text, float x, float y, float ext_x, float ext_y, DWORD color = 0xffffffff, bool shadowed = false);

            void	RenderCircle(const float *pWTM, float rad, unsigned int color, unsigned int segments = 16);
            void	RenderDebugSphere(const float *pos, const float rad, const unsigned int color, const unsigned short segments = 16);

            void	UnprojectScreenToWorld(float *v_out, const float *mat_view, const float *mat_proj, const float *screen_pos);
            void	ProjectWorldToScreen(float *v_out, const float *position);

            void	SetRenderTarget(const D3DRenderTarget *rt, bool clear = false, DWORD color = 0x00000000);
            void	EndRenderTarget(const D3DRenderTarget *rt);

            const D3DRenderTarget*	GetActiveRenderTarget() const;

            void	SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float MinZ = 0.f, float MaxZ = 1.f);

            void	DebugRenderShadowmap();
            bool	IsRenderShadowmapPass() const { return bRenderShadowmap; }

            void	SetBias(float DepthBias, float SlopeDepthBias);

            void	SetDebugInfoCPS(bool bParallel, int eventCPS, int updateCPS, int renderCPS, int timerCPS, int streamCPS);

            void	SetDeltaTime(float DeltaTime) { fDeltaTime = DeltaTime; fAppPassTime += DeltaTime; }

            TMapTextureNode& GetTextureNodesCache() { return m_MapTextureNodes; }
            TMapVolumeTextureNode& GetVolumeTextureNodesCache() { return m_MapVolumeTextureNodes; }
            TMapMaterialEffects& GetMaterialEffectsNodesCache() { return m_MapMaterialEffects; }

            MaterialEffectNode* GetEffectDefault(EEffectDefault effect) const;

            unsigned int GetLodBenefitByBBox(float *min_x, float *min_y, float *min_z,
                float *max_x, float *max_y, float *max_z);

            bool LoadDDS(TextureNode *node, const wchar_t *filename);
            bool LoadPNG(TextureNode *node, const char *filename);

            bool LoadTextureFromPixels32(
                TextureNode *node,
                unsigned int *pixels,
                unsigned int imgWidth,
                unsigned int imgHeight,
                unsigned int texWidth,
                unsigned int texHeight);

        private:
            SDDS_IMAGE_DATA* LoadDDSData(const wchar_t *filename);

            BOOL m_bValidDevice;

            TMapTextureNode					m_MapTextureNodes;
            TMapTextureNodeW				m_MapTextureNodesW;
            TMapVolumeTextureNode			m_MapVolumeTextureNodes;
            TMapMaterialEffects				m_MapMaterialEffects;
            TSubMeshNodes					SubMeshNodes;
            TSceneMeshNodes					m_MapSceneMeshNodes;
            TSkeletonNodes					SkeletonNodes;
            TVecFontNodes					m_VecFontNodes;
            TSubMeshAgregatorNodes			SubMeshAgregatorNodes;

            RECT							m_rcWindowClient;    // Saved client area size for mode switches

            // Internal variables for the state of the app
            bool							m_bWindowed;
            bool							m_bActive;
            bool							m_bDeviceLost;
            bool							m_bMinimized;
            bool							m_bMaximized;
            bool							m_bIgnoreSizeChange;
            bool							m_bDeviceObjectsInited;
            bool							m_bDeviceObjectsRestored;

            // Internal variables used for timing
            bool							m_bFrameMoving;
            bool							m_bSingleStep;

            SRenderContext			*m_pDefaultRenderContext;
            std::stack<SRenderContext*>		m_stackContext;

            std::vector<D3DVertexDbg>		m_3DDotsList;
            std::vector<D3DVertexDbg>		m_3DDotsListNoZ;
            std::vector<D3DVertexDbg>		m_3DPointList;
            std::vector<D3DVertexDbg>		m_PointListNoZ;
            std::vector<D3DVertexDbg>		m_PointsTriangle;

        public:
            SRenderContext					*m_pActiveRenderContext;

            Matrix4f						m_world;
            Matrix4f						m_view;
            Matrix4f						m_proj;
            float							m_fNearPlane;
            float							m_fFarPlane;
            Point3f							m_fViewPos;

            unsigned int					m_ObjectFlags;

            bool							m_bFog;
            float							m_fFogMin;
            float							m_fFogMax;
            float							m_fFogDensity;
            unsigned int					m_FogColor;

            /* Light states */
            SVector3f						m_vDiffusePoint;
            SVector3f						m_vLightDirection;
            Matrix4f						m_mLightView;
            Matrix4f						m_mLightProj;
            Matrix4f						m_mLightProjCrop[3];
            SVector4f						m_vFrustrumSplitDistances;
            unsigned int					m_fLightColor;

            TextureNode				*m_pTextureDef;
            TextureNode				*m_pTextureNormalDef;
            TextureNode				*m_pTextureShadowMap;

            MaterialEffectNode		*m_pDefaultDiffuse[EEF_MAX];

            float							m_fBlendOpacity;
            bool							bForceSoftware;
            bool							bRenderShadowmap;

            /** autoclear's flag*/
            BOOL							m_bAutoclearDebugTriangles;
            float							fAppPassTime;
            float							fDeltaTime;

        private:
            const D3DRenderTarget		*ActiveRenderTarget;

            TVCommandListRI					ListCommands;
            CRITICAL_SECTION				CommandsCS;

            // debug info
            bool m_bParalell;
            int m_eventCPS;
            int m_updateCPS;
            int m_renderCPS;
            int m_timerCPS;
            int m_streamCPS;
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        extern "C"
        {
            extern D3DDRIVER_API D3DMesh* AllocD3DMesh(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	ReleaseD3DMesh(D3DMesh *Ptr);

            extern D3DDRIVER_API MaterialEffect_ButtonStates* Alloc_MaterialEffect_ButtonStates(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_MaterialEffect_ButtonStates(MaterialEffect_ButtonStates *Ptr);

            extern D3DDRIVER_API D3DSkeleton* Alloc_D3DSkeleton(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_D3DSkeleton(D3DSkeleton *Ptr);

            extern D3DDRIVER_API MaterialEffect* Alloc_MaterialEffect(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_MaterialEffect(MaterialEffect *Ptr);

            extern D3DDRIVER_API MaterialEffectUI* Alloc_MaterialEffectUI(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_MaterialEffectUI(MaterialEffectUI *Ptr);

            extern D3DDRIVER_API CMaterialEffectFont* Alloc_MaterialEffectFont(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_MaterialEffectFont(CMaterialEffectFont *pPtr);

            extern D3DDRIVER_API D3DRenderTarget* Alloc_D3DRenderTarget(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_D3DRenderTarget(D3DRenderTarget *Ptr);

            extern D3DDRIVER_API CFont2D* Alloc_Font2D(const D3DDriver *pInterface);
            extern D3DDRIVER_API void	Release_Font2D(CFont2D *Ptr);
        }
    }
}