
#pragma once

#include "ObjectAbstract.h"
#include "RenderSDKInterface.h"
#include "RenderAdjacency.h"

#define TRIGGERED_VALUE 2

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class EXPORT CRenderSDK : public CObjectAbstract
{
	DECLARE_CLASS_SIMPLE(CRenderSDK, CObjectAbstract);

	struct SRenderAtom
	{
		const class CRenderObject* pRenderObject;
		unsigned int			   uFlags;
	};

	typedef std::stack<Matrix>		   TTransformStack;
	typedef std::stack<float>		   TAlphaBlendStack;

	typedef std::vector<class CommandBase*>			TVRenderStack;
	typedef std::vector<SRenderAtom>				TVRenderQuevue;
	typedef std::vector<RenderQuevueAdjacency>		TRenderAdjacencyQuevue;

public:
	CRenderSDK(const CObjectAbstract *pParent = NULL);
	virtual ~CRenderSDK();
	
	__forceinline class CFXMatManager*	GetFXManager() const { return m_pFXManager; }
	__forceinline class D3DDriver*		GetRenderDriver() const { return m_pRenderDriver; }

	bool					EnterCS() { return m_CriticalSection.enter(); }
	bool					LeaveCS() { return m_CriticalSection.leave(); }

	NObjectFactory::TClassFactory* GetClassFactory();

	void					Initialize(HWND hWnd, unsigned int width, unsigned int height, void *driver = NULL);
	void					ProcessStreaming();

	void					SetViewport(float x, float y, float ext_x, float ext_y);
	
	bool					ResizeWindow(unsigned int width, unsigned int height, class SRenderContext *pContext = 0);

	bool					GetWireframeMode() const;
	void					SetWireframeMode(bool flag);

	unsigned int			GetViewportWidth() const;
	unsigned int			GetViewportHeight() const;

	void					SetRenderTarget(const class Comp_RenderTarget *pRenderTarget, bool bClearTarget = false);
	void					EndRenderTarget();

	void					SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, float MinZ = 0.f, float MaxZ = 1.f);

	void					DrawObject(const class CRenderObject *pObject);
	void					DrawObject(const class CRenderObject *pObject, const Matrix &WTM);

	void					DrawDot(const Vector &point, unsigned int color, bool bZEnable = true);
	void					DrawLine(const Vector &start, const Vector &end, unsigned int color, bool bZEnable = true);
	void					DrawLine(const Vector2f &start, const Vector2f &end, unsigned int color, bool bZEnable = true);

	BOOL					DrawBox2D(Vector2f &min, Vector2f &max, unsigned int color);
	BOOL					DrawAABox(Vector &min, Vector &max, DWORD color, BOOL zEnable = TRUE, BOOL bWired = TRUE);
	void					RenderPoint(Vector &point);

	void					DrawRectangle(Vector2f &start, Vector2f &end, unsigned int color);
	void					DrawCircle(const Matrix &WTM, float rad, unsigned int color, unsigned int segments = 16);
	void					DrawSphere(Vector &Pos, float rad, unsigned int color, unsigned short segments = 16);

	void					DrawText(wchar_t *text, const Vector &position, unsigned int color = 0xffffffff, bool shadowed = false);
	void					DrawText2D(const wchar_t *text, const Vector2f &position, unsigned int color = 0xffffffff, bool shadowed = false);
	void					DrawText2D(const wchar_t *text, const Vector2f &position, const Vector2f &extension, unsigned int color = 0xffffffff, bool shadowed = false);
	
	void					DrawTriangle(const Vector &P0, const Vector &P1, const Vector &P2, unsigned int color = 0xffffffff); 
	void					DrawTriangle2D(const Vector2f &P0, const Vector2f &P1, const Vector2f &P2, unsigned int color = 0xffffffff);

	void					DrawBounds(const Matrix &WTM, const Bounds3f &Bound, unsigned int Color = 0xffffffff);
	void					DrawBounds2f(const Matrix &WTM, const Bounds3f &Bound, unsigned int Color = 0xffffffff);
	
	void					SwapBuffer();
	void					Render(SRenderContext *pContext, int cps = 0);

	void					Present(const SRenderContext *pContext);

	void					UpdateResources(bool bLock = false);

	inline_ void			SetViewMatrix(const Matrix &M);
	inline_ Matrix			GetViewMatrix();

	inline_ void			SetProjMatrix(const Matrix &M);
	inline_ Matrix			GetProjMatrix();

	inline_ void			SetProjCropMatrix(const Matrix &M);
	inline_ Matrix			GetProjCropMatrix();

	inline_ void			SetTransform(const Matrix &M);

	inline_ void			SetBlendOpacity(float bland);
	inline_ void			PushBlendOpacity(float bland);
	inline_ void			PopBlendOpacity();
	inline_ void			ResetBlend();

	void					GetViewport(Vector4f& OutViewport);

	/** 
	* Return render adjacency object
	*/
	RenderSDK::RenderAdjacency& GetRenderAdjacency() { return *m_pRenderAdjacency; }

	void					PushRenderQuevueAdjaency();
	void					PopRenderQuevueAdjaency();
	RenderSDK::SRTVariant_Adjacency&  GetCurrQuevueAdjaency();

	inline_ void			PushToRenderStack(CommandBase* Chunk, bool Post = false);
	inline_ void			PushObjectToQuevue(const CRenderObject *pObject, unsigned int Flags = 0);

	inline_ void			SetSwapFlag();
	inline_ bool			IsSwapFlag() { return bFlagSwapStack; }

	//[DEBUG]{
	virtual void			_DebugDrawRectangle2D(Vector2f& P0, Vector2f& P1, unsigned int color = 0xffffffff);
	virtual void			_DebugDrawRectangle2D(Vector2f& P0, Vector2f& P1, Vector2f& P2, Vector2f& P3, unsigned int color = 0xffffffff, bool bZEnable = true);
	Vector2f				ProjectViewport(Vector2f &v_out, const Vector& InPos);
	Vector					UnprojectScreenToWorld(const Vector2f& InValue, const Matrix& ViewM, const Matrix &ProjM);
	
	//}[DEBUG]

	void					SetPointLight(const Vector& position, const Matrix& LightView, const Matrix& LightProj, unsigned int color);
	
	void					SetProjectedLight(const Vector &position,
												const Vector &direction,
												const Matrix& LightView,
												const Matrix& LightProj,
												unsigned int color,
												const Vector4f& SplitDistances,
												const Matrix *MatrixArray,
												unsigned int Num);

	inline_ unsigned int	GetRenderCPS() { return RenderCPS; }

	void					SetFakeRender(bool Value) { bFakeDraw = Value; }
	bool					GetFakeRender() { return bFakeDraw; }

	void					SetRenderInfoMask(unsigned int Value) { RenderInfoMask = Value; }
	void					ResetRenderInfoData();

	// Render Information data accessors
	unsigned long			GetRI_NumObjects() const { return RI_NumObjects; }
	const Bounds3f&			GetRI_GetObjectsBound() const { return RI_RenderObjectsBound; }
	const Bounds3f&			GetRI_GetObjectsShadowCasterBound() const { return RI_ShadowCastersBound; }
	const Bounds3f&			GetRI_GetObjectsShadowReceiversBound() const { return RI_ShadowRecieversBound; }

	void					SetBias(float DepthBias, float SlopeDepthBias);

	void					SetDebugInfoCPS(bool bParallel, 	
											int eventCPS,
											int updateCPS,
											int renderCPS,
											int timerCPS,
											int streamCPS);

	void					SetDeltaTime(float DeltaTime);

	void					SetFreezeRender(bool bValue) { bFreezeRender = bValue; }
	inline_ bool			GetFreezeRender() const { return bFreezeRender; }

	void					SetDrawHelpers(bool bValue) { bDrawHelpers = bValue; }
	inline_ bool			GetDrawHelpers() const { return bDrawHelpers; }

protected:
	void					CollectRenderInfoData(const class CRenderObject * Object);

private:
	bool					bFlagSwapStack;
	volatile byte			ActiveStack;
	
	RenderSDK::RenderAdjacency	*m_pRenderAdjacency;

	TRenderAdjacencyQuevue	RenderAdjacencyQuevue[TRIGGERED_VALUE];
	TVRenderStack			RenderStack[TRIGGERED_VALUE];
	TVRenderStack			RenderStackPost[TRIGGERED_VALUE];
	TVRenderQuevue			m_RenderQuevue[TRIGGERED_VALUE];

	TTransformStack			TransformStack;
	TAlphaBlendStack		AlphaBlendStack;
		
	Matrix					World;
	Matrix					View;
	Matrix					Proj;
	Matrix					ProjCrop;

	CriticalSection			m_CriticalSection;
	unsigned int			RenderCPS;
	bool					bFakeDraw;

	unsigned int			RenderInfoMask;

	unsigned long			RI_NumObjects;
	Bounds3f				RI_RenderObjectsBound;
	Bounds3f				RI_ShadowCastersBound;
	Bounds3f				RI_ShadowRecieversBound;

	bool					bFreezeRender;
	bool					bDrawHelpers;

	class CFXMatManager		*m_pFXManager;
	class D3DDriver			*m_pRenderDriver;

public:
	int						ThreadRenderID;
};