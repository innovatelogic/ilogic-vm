#ifndef __scriptdriver_h__
#define __scriptdriver_h__

#ifdef WIN32
#pragma once
#endif

class CORESDK_API ScriptDriver
{
	typedef std::vector<class ActionBase*> TVecActions;

public:
	ScriptDriver(class CCoreSDK * pSDK);
	virtual ~ScriptDriver();

	bool	RegisterAction(class ActionBase * Action);
	bool	UnRegisterAction(class ActionBase * Action);

	void	Render(class CActor* Actor, HDC hdc, const Matrix &ViewMatrix);

	class ActionBase* GetActionByActor(class CActor *Actor);

	struct SocketAction* ProcessClickSocket(class CActor* OwnerActor, const Vector2f &Point, bool link = false) const;
	const class ActionBase* ProcessClickAction(class CActor* OwnerActor, const Vector2f &Point) const;

protected:
private:
	class CCoreSDK *pCoreSDK;
	TVecActions		VecActions;
};

#endif//__scriptdriver_h__