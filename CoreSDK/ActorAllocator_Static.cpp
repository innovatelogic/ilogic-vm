#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
bool ActorAllocator::MoveObjectHierarchy(CActor * ActorMove, const Registry * Reg, bool Up /*= true*/, bool bMoveEditorVisible /*= false*/)
{
	bool bResult = false;

	CActor * ParentObject = ActorMove->GetParent();

	if (ParentObject)
	{
		TVecActorChildIterator IterActorMove = std::find(ParentObject->m_ChildNodes.begin(), ParentObject->m_ChildNodes.end(), ActorMove);

		// check object already in extremum position
		if ((Up && *IterActorMove == *ParentObject->m_ChildNodes.begin()) || 
			(!Up && *IterActorMove == *ParentObject->m_ChildNodes.rbegin()))
		{
			return false;
		}

		TVecActorChildIterator IterActorMoveTo = (Up == true) ? IterActorMove - 1 : IterActorMove + 1;

		if (IterActorMove != ParentObject->m_ChildNodes.end() && IterActorMoveTo != ParentObject->m_ChildNodes.end())
		{
			if (bMoveEditorVisible) // go throw editor invisible objects
			{
				while (!Reg->IsEditorVisible((*IterActorMoveTo)->GetType()) /*!(*IterActorMoveTo)->IsEditorVisible()*/)
				{
					if (*IterActorMoveTo == *ParentObject->m_ChildNodes.begin() ||
						*IterActorMoveTo == *ParentObject->m_ChildNodes.rbegin())
					{
						return false;
					}
					Up ? IterActorMoveTo-- : IterActorMoveTo++;
				}
			}

			// swap pointers in parant object array vector
			std::swap(*IterActorMove, *IterActorMoveTo);

			// signal call
			for (TVecEventsIterator Iter = ActorMove->m_VEvents.begin(); Iter != ActorMove->m_VEvents.end(); ++Iter)
			{
				if ((Up == true ? Event_MoveObjectUp : Event_MoveObjectDown) == (*Iter)->EventId)
				{
					(*Iter)->CallEvent(ActorMove);
				}
			}
			bResult = true;
		}
	}
	return bResult;
}