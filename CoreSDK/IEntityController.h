#ifndef __ientitycontroller_h__
#define __ientitycontroller_h__

class CORESDK_API IEntityController
{
public:
	IEntityController(const CObjectAbstract *pParent);
	IEntityController(const IEntityController &Source);
	virtual ~IEntityController();

	void RegisterEntityInterface(const CActor *pSrc, bool bPlayer, int SrcParent = 1);
	void UnregisterEntityInterface();

	inline_ class CCoreSDK*	GetCoreSDK() const { return m_pCoreSDK; }
	void					SetCoreSDK(class CCoreSDK* pCoreSDK) const { m_pCoreSDK = pCoreSDK; }

	virtual bool ProcessInputMouseEnt(const MouseInputData &InputData) = 0;
	virtual bool ProcessInputMouseEnt(const MouseMoveInputData &InputData) = 0;
	virtual bool ProcessButton(const EventInput& InputData) = 0;

protected:
private:
	TNodeMap<class CActor, struct SPlayerEntityInfo> *m_pNode;
	mutable class CCoreSDK *m_pCoreSDK;
};

#endif//__ientitycontroller_h__
