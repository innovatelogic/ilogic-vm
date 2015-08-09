#ifndef __inputmanager_h__
#define __inputmanager_h__

#ifdef WIN32
#pragma once
#endif

#include "../Foundation/StdafxFoundation.h"

class EXPORT CInputManager
{
	typedef TTreeMap<class CActor, class IInputInterface>	TTreeMapInput;

public:
	CInputManager(class CCoreSDK* pSDK);
	virtual ~CInputManager();

	TNodeMap<class CActor, class IInputInterface>* RegisterObject(const IInputInterface *Src,
																 const CActor *Key,
																 const CActor *KeyParent);

	void UnregisterObject(class TNodeMap<CActor, IInputInterface> *pNode);

	void ProcessInputKey(const EventInput &InputData);
	void ProcessMouseWheel(float ds);

protected:
private:
	TTreeMapInput m_VecInputList;
	class CCoreSDK* m_pCoreSDK;
};

#endif//__inputmanager_h__