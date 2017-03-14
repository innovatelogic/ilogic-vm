/*#include "coresdkafx.h"
#include "physicsafx.h"

//----------------------------------------------------------------------------------------------
TriggerCallbackReport::TriggerCallbackReport(CCoreSDK *pSDK)
: pCoreSDK(pSDK)
{
	;//
}

//----------------------------------------------------------------------------------------------
void TriggerCallbackReport::onTrigger(NxShape & triggerShape, NxShape & otherShape, NxTriggerFlag status)
{
	NxActor & TriggerActorNx = triggerShape.getActor();
	NxActor & OtherActorNx = otherShape.getActor();

	TriggerObject * Trigger = (TriggerObject*)TriggerActorNx.userData;
	CActor * OtherActor = (CActor*) OtherActorNx.userData;
	
	if (status & NX_TRIGGER_ON_ENTER)
	{
		pCoreSDK->AddCommandLock(new Command_Val2<TriggerObject, TriggerObject, CActor, TriggerShapeFlag>
												(&TriggerObject::OnTriggerEvent, Trigger, OtherActor, PHYS_TRIGGER_ON_ENTER));
		return;
	}

	if (status & NX_TRIGGER_ON_LEAVE)
	{
		pCoreSDK->AddCommandLock(new Command_Val2<TriggerObject, TriggerObject, CActor, TriggerShapeFlag>
												(&TriggerObject::OnTriggerEvent, Trigger, OtherActor, PHYS_TRIGGER_ON_LEAVE));
		return;
	}

	if (status & NX_TRIGGER_ON_STAY)
	{
		pCoreSDK->AddCommandLock(new Command_Val2<TriggerObject, TriggerObject, CActor, TriggerShapeFlag>
												(&TriggerObject::OnTriggerEvent, Trigger, OtherActor, PHYS_TRIGGER_ON_STAY));
	}
}*/