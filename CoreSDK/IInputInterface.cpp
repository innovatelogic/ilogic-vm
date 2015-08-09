#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
IInputInterface::IInputInterface(const CObjectAbstract *pParent)
: m_pNode(0)
{
	if (pParent) 
	{
 		m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IInputInterface::IInputInterface(const IInputInterface &Source)
{
	if (this != &Source)
	{
		m_pCoreSDK = Source.m_pCoreSDK;
	}
}

//----------------------------------------------------------------------------------------------
IInputInterface::~IInputInterface()
{
	for_each(m_EventsInput.begin(), m_EventsInput.end(), DeleteVectorFntor());

	m_EventsInput.clear();

	UnregisterInputInterface();
}

//----------------------------------------------------------------------------------------------
void IInputInterface::RegisterInputInterface(const CActor *Src, int SrcParent /*= 1*/)
{
	if (m_pCoreSDK)
	{
		m_pNode = m_pCoreSDK->GetInputManager()->RegisterObject(this, Src, (SrcParent == 1) ? Src->GetParent() : 0 );
	}
}

//----------------------------------------------------------------------------------------------
void IInputInterface::UnregisterInputInterface()
{
	if (m_pCoreSDK && m_pNode)
	{
		m_pCoreSDK->GetInputManager()->UnregisterObject(m_pNode);
	}
}

//----------------------------------------------------------------------------------------------
void IInputInterface::ProcessInputKey(const EventInput &InputData)
{
	CALL_EVENT_DELEGATE_PROXY_INPUT(InputData);
}

/*
//----------------------------------------------------------------------------------------------
void IInputInterface::SetFocus()
{

}

//----------------------------------------------------------------------------------------------
void IInputInterface::KillFocus()
{

}

//----------------------------------------------------------------------------------------------
bool IInputInterface::ProcessInputKey(const EventInput &InputData)
{
	bool bResult = true;
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IInputInterface::OnEventPressed(const MouseInputData &InputData)
{
	bool bResult = true;
	return bResult;
}

//----------------------------------------------------------------------------------------------
void IInputInterface::OnEventReleased(Event code, int x, int y)
{

}

//----------------------------------------------------------------------------------------------
bool IInputInterface::DoEventPressed(const MouseInputData &InputData)
{
	bool bResult = true;
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool IInputInterface::DoEventReleased(const MouseInputData &InputData)
{
	bool bResult = true;
	return bResult;
}*/