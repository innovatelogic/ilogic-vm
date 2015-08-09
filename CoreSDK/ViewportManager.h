#ifndef __viewportmanager_h__
#define __viewportmanager_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

class EXPORT CViewportManager
{
	friend class CInputControllerImpl;
	friend class CInputControllerEdImpl;

public:
	typedef TTreeMap<class CActor, class ViewportInterface>		TTreeMapViewport;
	typedef TTreeMap<class CActor, class IDrawInterface>		TTreeMapDraw;

	CViewportManager(class CCoreSDK* pSDK);
	virtual ~CViewportManager();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }

	TNodeMap<class CActor, class IDrawInterface>* RegisterObject(const IDrawInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
	void UnregisterObject(class TNodeMap<CActor, IDrawInterface> *pNode);

	TNodeMap<class CActor, class ViewportInterface>* RegisterViewport(const ViewportInterface *pSrc, const CActor *pKey, const CActor *pKeyParent);
	void UnregisterViewport(class TNodeMap<CActor, ViewportInterface> *pNode);

	ViewportInterface* GetViewportInterface(const IDrawInterface *pIObject) const;

	bool IsRenderable(const CActor *Key);

	void Draw();
	void DrawNode(TNodeMap<CActor, IDrawInterface> *pNode);

	// controller input
	void ProcessInputMouse(const MouseInputData &InputData, IDrawInterface *pIObjectMask = 0);
	void ProcessInputMouse(const MouseMoveInputData &InputData, IDrawInterface *pIObjectMask = 0);
	void ProcessMouseWheel(float ds, class SRenderContext *pRenderContext = 0);

	int				SetFocus(IDrawInterface *pIObject, bool bFlag = true);
	int				SetFocusActor(CActor *pAObject, bool bFlag = true);

	IDrawInterface* GetFocused() const;
	CActor*			GetFocusedActor() const;

	int				SetMouseOver(IDrawInterface *pIObject, bool bFlag = true);
	IDrawInterface* GetMouseOver() const;

	IDrawInterface* GetByActor(CActor *pAObject) const;

	void			RebildWorldTransform();
	void			RebuildSizeTransform();

	void			RebuildTransform(CActor *pAObject = NULL);

protected:
	TNodeMap<CActor, IDrawInterface>* GetNodeByKey(CActor *pAObject) const;

private:
	TTreeMapViewport m_VecViewports;
	TTreeMapDraw	 m_VecDrawList;

	class CInputControllerImpl	 *m_pInputControllerImpl;
	class CInputControllerEdImpl *m_pInputControllerEdImpl;

	class CCoreSDK* m_pCoreSDK;
};

#endif//__viewportmanager_h__