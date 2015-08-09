#ifndef __ilistener_h__
#define __ilistener_h__

#ifdef _WIN32
#pragma once
#endif

class IListener
{
public:
	virtual void OnOperationComplete(class CObjectAbstract*) = 0;
	virtual void OnOperationFailed(class CObjectAbstract*) = 0;
};

#endif