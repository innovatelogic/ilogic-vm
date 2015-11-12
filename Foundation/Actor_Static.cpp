#include "Actor.h"

//----------------------------------------------------------------------------------------------
bool CActor::LoadFromFile(const char *filename, bool bLoadAsExternal /*= false*/)
{
	bool bResult = false;

	SetExternal(bLoadAsExternal);

	std::string sFilename;  

	if (FindFile(filename, &sFilename))
	{
		SetFilenameTag(filename);

		XML_PARSE_FILE(sFilename)
		{
			XML_CHECK_NODE(GetType())
			{
				SuperDeserializer(XML_CURRENT_NODE);
			}	
		}
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CActor::SaveToFile(const char *filename, bool bSaveAsExternal /*= false*/)
{
	bool bResult = false;

	// write out to disk O
	std::ofstream file(filename, std::ios::out);

	if (file.is_open())
	{
		SuperSerializer(file, bSaveAsExternal);

		file.close();
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CActor::SaveToBuffer(std::stringstream &stream, bool bSaveAsExternal /*= false*/)
{
	SuperSerializer(stream, bSaveAsExternal);
	return true;
}

//----------------------------------------------------------------------------------------------
bool CActor::FindCommonParent(CActor** OutParent, const CActor * ObjectA, const CActor * ObjectB)
{
	// common parent object
	(*OutParent) = nullptr;

	// try to find common root node
	CActor * ParentB = const_cast<CActor*>(ObjectB);

	while (ParentB && !(*OutParent))
	{
		CActor * ParentA = const_cast<CActor*>(ObjectA);

		while (ParentA != nullptr)
		{
			if (ParentB == ParentA)
			{
				(*OutParent) = ParentA;
				break;
			}
			ParentA = ParentA->GetParent();
		}
		ParentB = ParentB->GetParent();
	}
	return ((*OutParent) != nullptr);
}

//----------------------------------------------------------------------------------------------
bool CActor::GetRelativePath(std::string& OutPath, const CActor * SourceObject, const CActor * ObjectB)
{
	// common parent object
	CActor * ObjectC = nullptr;

	// common root node found
	if (FindCommonParent(&ObjectC, SourceObject, ObjectB))
	{
		std::vector<const CActor*> TmpActorListA, TmpActorListB;

		// build A-way array
		CActor * IterActor = const_cast<CActor*>(SourceObject);
		while (IterActor != ObjectC) 
		{
			TmpActorListA.push_back(IterActor);
			IterActor = IterActor->GetParent();
		}

		// build B-way array 
		IterActor = const_cast<CActor*>(ObjectB);
		while (IterActor != ObjectC) 
		{
			TmpActorListB.push_back(IterActor);
			IterActor = IterActor->GetParent();
		}

		OutPath.empty();
		for (std::vector<const CActor*>::iterator Iter = TmpActorListA.begin(); Iter != TmpActorListA.end(); ++Iter)
		{
			OutPath += "../";
		}

		for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorListB.rbegin(); Iter != TmpActorListB.rend(); ++Iter)
		{
			OutPath +=(*Iter)->GetName();

			std::vector<const CActor*>::reverse_iterator IterNext = Iter;
			if (++IterNext != TmpActorListB.rend()){
				OutPath += "/";
			}
		}
	}
	return (ObjectC != nullptr);
}



//----------------------------------------------------------------------------------------------
std::string CActor::ResolveName(std::string& PrefferName, const CActor * InPlaceObject)
{
	std::string PreffixValue(PrefferName);
	int CounterIndex = 1;

	int CopyCounter = atoi(PreffixValue.c_str());
	while (CopyCounter == 0 && PreffixValue.length() > 0)
	{
		PreffixValue = PreffixValue.substr(1);
		CopyCounter = atoi(PreffixValue.c_str());
	}

	if (CopyCounter != 0) // add number copy
	{
		PreffixValue = PrefferName.substr(0, PrefferName.length() - PreffixValue.length());
		CounterIndex = CopyCounter;
	}
	else
	{
		PreffixValue = PrefferName; // use preffer as a prefix
	}

	std::string FindName = PreffixValue;// +boost::lexical_cast<std::string>(++CounterIndex);
	while (InPlaceObject->FindActor(FindName.c_str()))
	{
		// @TODO: add break
		FindName = PreffixValue;// +boost::lexical_cast<std::string>(++CounterIndex);
	}
	return PreffixValue;// +boost::lexical_cast<std::string>(CounterIndex);
}

//----------------------------------------------------------------------------------------------
bool CActor::MoveObjectHierarchy(CActor * ActorMove, bool Up /*= true*/, bool bMoveEditorVisible /*= false*/)
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
// 			if (bMoveEditorVisible) // go throw editor invisible objects
// 			{
// 				while (!(*IterActorMoveTo)->IsEditorVisible())
// 				{
// 					if (*IterActorMoveTo == *ParentObject->ChildNodes.begin() ||
// 						*IterActorMoveTo == *ParentObject->ChildNodes.rbegin())
// 					{
// 						return false;
// 					}
// 					Up ? IterActorMoveTo-- : IterActorMoveTo++;
// 				}
// 			}

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

//----------------------------------------------------------------------------------------------
bool CActor::MoveObjectHierarchyBound(CActor * ActorMove, bool Up /*= true*/)
{
	// very unoptimized version
	while (CActor::MoveObjectHierarchy(ActorMove, Up, true))
	{
	}
	return true;
}

//----------------------------------------------------------------------------------------------
bool CActor::IsChildOf(const CActor *pParent, const CActor *pChild)
{
	// is child of
	bool bChild = false;
	const CActor *pIter = pChild;

	while (pIter)
	{
		if (pIter == pParent){
			bChild = true;
			break;
		}
		pIter = pIter->GetParent();
	}
	return bChild;
}

//----------------------------------------------------------------------------------------------
bool CActor::IsChildOf(const CActor *pParent1, const CActor *pParent2, const CActor *pChild)
{
	// is child of
	bool bChild = false;
	const CActor *pIter = pChild;

	while (pIter)
	{
		if (pIter == pParent1 || pIter == pParent2){
			bChild = true;
			break;
		}
		pIter = pIter->GetParent();
	}
	return bChild;
}